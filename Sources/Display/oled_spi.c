/*
 * oled_spi.c
 *
 *  Created on: Feb 8, 2018
 *      Author: Eivind Roson Eide
 */

#include "fsl_debug_console.h"
#include "fsl_spi_master_driver.h"
#include "fsl_clock_manager.h"
#include "board.h"


/********************************** low level pin interface */

static volatile spi_master_state_t			spiMasterState;
static volatile spi_master_user_config_t	spiUserConfig;

static volatile uint8_t inBuffer[32];
static volatile uint8_t payloadBytes[32];



void oled_spi_init(void){
	CLOCK_SYS_EnableSpiClock(0);

	/*
	 *	Initialize SPI master. See KSDK13APIRM.pdf Section 70.4
	 *
	 */
	uint32_t			calculatedBaudRate;
	spiUserConfig.polarity		= kSpiClockPolarity_ActiveHigh;
	spiUserConfig.phase		= kSpiClockPhase_FirstEdge;
	spiUserConfig.direction		= kSpiMsbFirst;
	spiUserConfig.bitsPerSec	= 50000;
	SPI_DRV_MasterInit(0 /* SPI master instance */, (spi_master_state_t *)&spiMasterState);
	SPI_DRV_MasterConfigureBus(0 /* SPI master instance */, (spi_master_user_config_t *)&spiUserConfig, &calculatedBaudRate);
	printf("Calculated baud rate is %ld\n", calculatedBaudRate);

	//Reseting OLED screen
	/*
	 *	RST (PTA2) high->low->high
	 */
	GPIO_DRV_SetPinOutput(kGpioRST);
	OSA_TimeDelay(1000);
	GPIO_DRV_ClearPinOutput(kGpioRST);
	OSA_TimeDelay(1000);
	GPIO_DRV_SetPinOutput(kGpioRST);
    OSA_TimeDelay(1000);
}


static void oled_spiWrite(uint8_t commandByte){
	spi_status_t status;
	payloadBytes[0] = commandByte;

	status = SPI_DRV_MasterTransferBlocking(0	/* master instance */,
					NULL		/* spi_master_user_config_t */,
					(const uint8_t * restrict)&payloadBytes[0],
					(uint8_t * restrict)&inBuffer[0],
					1		/* transfer size */,
					1000		/* timeout in microseconds (unlike I2C which is ms) */);
}



void oled_writeData(uint8_t dataByte){

	/*
	 *	DC (PTB13) High
	 *
	 *	TODO: Should it be digitalWrite(_rs, LOW)?
	 *
	 */
	GPIO_DRV_SetPinOutput(kGpioDC);

	/*
	 *	/CS (PTA12) Low
	 */
	GPIO_DRV_ClearPinOutput(kGpioOC);


	//Transfer byte
	oled_spiWrite(dataByte);

	/*
	 *	/CS (PTA12) High
	 */
	GPIO_DRV_SetPinOutput(kGpioOC);

}

void oled_delay(uint32_t milli){
	OSA_TimeDelay(milli);
}

void oled_writeCommand(uint8_t commandByte)
{
	spi_status_t status;

	/*
	 *	/CS (PTA12) low
	 */
	GPIO_DRV_ClearPinOutput(kGpioOC);

	/*
	 *	DC (PTB13) low
	 */
	GPIO_DRV_ClearPinOutput(kGpioDC);

	oled_spiWrite(commandByte);

	/*
	 *	/CS (PTA12) high
	 */
	GPIO_DRV_SetPinOutput(kGpioOC);

	return;
}

/***********************************/
