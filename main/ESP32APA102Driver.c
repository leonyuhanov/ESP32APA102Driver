#include "ESP32APA102Driver.h"

unsigned char colourList[9*3]={maxValuePerColour,0,0, maxValuePerColour,maxValuePerColour,0, 0,maxValuePerColour,0, 0,maxValuePerColour,maxValuePerColour, 0,0,maxValuePerColour, maxValuePerColour,0,maxValuePerColour, maxValuePerColour,maxValuePerColour,maxValuePerColour, maxValuePerColour,0,0, 0,0,0};

struct apa102LEDStrip leds;
struct colourObject dynColObject;

//SPI Vars
spi_device_handle_t spi;
spi_transaction_t spiTransObject;
esp_err_t ret;
spi_bus_config_t buscfg;
spi_device_interface_config_t devcfg;

void app_main()
{
	unsigned char tmpColour[3];
	unsigned short int cIndex=0, cnt=0;
	
    printf("\r\n\r\n\r\nHello Pixels!\n");
	
	//Set up SPI
	printf("Setting up SPI now\t[%d]\r\n", setupSPI());
	
	//set up LED object
	printf("Creating led object...\t");
	initLEDs(&leds, totalPixels, bytesPerPixel, 255);
	printf("Frame Length\t%d\r\n", leds._frameLength);

	//set up colours
	initComplexColourObject(&dynColObject, maxValuePerColour, 9, colourList);
		
	//Set up SPI tx/rx storage Object
	memset(&spiTransObject, 0, sizeof(spiTransObject));
	spiTransObject.length = leds._frameLength*8;
	spiTransObject.tx_buffer = leds.LEDs;
	printf("SPI Object Initilized...\r\n");


	printf("Sending SPI data block to clear all pixels....\r\n");
	spi_device_queue_trans(spi, &spiTransObject, portMAX_DELAY);
	printf("Pixels Cleared!\r\n");

	while(1)
	{
		getColour(&dynColObject, cIndex%dynColObject._bandWidth, tmpColour);
		setPixel(&leds, 0, tmpColour);
		for(cnt=totalPixels-1; cnt>0; cnt--)
		{
			getPixel(&leds, cnt-1, tmpColour);
			setPixel(&leds, cnt, tmpColour);
		}
		renderLEDs();
		cIndex+=2;
        vTaskDelay(10 / portTICK_PERIOD_MS);		
    }
}

void renderLEDs()
{
	spi_device_queue_trans(spi, &spiTransObject, portMAX_DELAY);
}

int setupSPI()
{
	//Set up the Bus Config struct
	buscfg.miso_io_num=-1;
	buscfg.mosi_io_num=PIN_NUM_MOSI;
	buscfg.sclk_io_num=PIN_NUM_CLK;
	buscfg.quadwp_io_num=-1;
	buscfg.quadhd_io_num=-1;
	buscfg.max_transfer_sz=maxSPIFrameInBytes;
	
	//Set up the SPI Device Configuration Struct
	devcfg.clock_speed_hz=maxSPIFrequency;
	devcfg.mode=0;                        
	devcfg.spics_io_num=-1;             
	devcfg.queue_size=1;

	//Initialize the SPI driver
	ret=spi_bus_initialize(VSPI_HOST, &buscfg, 1);
    ESP_ERROR_CHECK(ret);	
	//Add SPI port to bus
	ret=spi_bus_add_device(VSPI_HOST, &devcfg, &spi);
	ESP_ERROR_CHECK(ret);
	return ret;
}

