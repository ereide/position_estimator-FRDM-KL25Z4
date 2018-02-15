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


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SPI_MASTER_INSTANCE         BOARD_SPI_INSTANCE  /*! User change define to choose SPI instance */
#define TRANSFER_SIZE               (1)
#define TRANSFER_BAUDRATE           (500000U)           /*! Transfer baudrate - 500k */
#define MASTER_TRANSFER_TIMEOUT     (1000U)             /*! Transfer timeout of master - 10ms */



/********************************** low level pin interface */

static spi_master_user_config_t				spiUserConfig;
static spi_master_state_t			spiMasterState;

static volatile uint8_t inBuffer[32];
static volatile uint8_t payloadBytes[32];



#define OLED_SPI_INSTANCE 0

static void set_spi_userconfig(void){
	spiUserConfig.polarity		= kSpiClockPolarity_ActiveHigh;
	spiUserConfig.phase			= kSpiClockPhase_FirstEdge;
	spiUserConfig.direction		= kSpiMsbFirst;
	spiUserConfig.bitsPerSec	= TRANSFER_BAUDRATE;
}


void oled_spi_init(void){
	CLOCK_SYS_EnableSpiClock(0);

	/*
	 *	Initialize SPI master. See KSDK13APIRM.pdf Section 70.4
	 *
	 */
	uint32_t			calculatedBaudRate;

	//Init SPI driver
    SPI_DRV_MasterInit(SPI_MASTER_INSTANCE, &spiMasterState);

    //Set SPI configuration
	set_spi_userconfig();
    SPI_DRV_MasterConfigureBus(SPI_MASTER_INSTANCE, &spiUserConfig, &calculatedBaudRate);


	// Check if the configuration is correct
    if (calculatedBaudRate > spiUserConfig.bitsPerSec)
    {
        PRINTF("\r**Something failed in the master bus config \r\n");
        return;
    }
    else
    {
        PRINTF("- SPI: Baud rate in Hz is: %d\r\n", calculatedBaudRate);
    }


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

    PRINTF("- SPI: OLED Init complete \n\r");
}


static void oled_spiWrite(uint8_t commandByte){
	//spi_status_t status;
	payloadBytes[0] = commandByte;

    // Start transfer data to slave
    if (SPI_DRV_MasterTransferBlocking( SPI_MASTER_INSTANCE,
    									NULL /* spi_master_user_config_t */,
    									(const uint8_t * restrict)&payloadBytes[0],
										NULL,
										TRANSFER_SIZE,
										MASTER_TRANSFER_TIMEOUT) == kStatus_SPI_Timeout)
    {
        PRINTF("\r\n**Sync transfer timed-out \r\n");
        return;
    }
}



void oled_writeData(uint8_t dataByte){

	/*
	 *	DC (PTB13) High
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
	/*
	 *	/CS (PTA12) low
	 */
	GPIO_DRV_ClearPinOutput(kGpioOC);

	/*
	 *	DC (PTB13) low
	 */
	GPIO_DRV_ClearPinOutput(kGpioDC);

	// Write output
	oled_spiWrite(commandByte);

	/*
	 *	/CS (PTA12) high
	 */
	GPIO_DRV_SetPinOutput(kGpioOC);

	return;
}

/***********************************/
