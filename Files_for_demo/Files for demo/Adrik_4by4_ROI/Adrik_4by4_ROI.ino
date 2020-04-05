#include <Wire.h>
#include "vl53l1_api.h"

VL53L1_Dev_t                   dev;   //Dev           : Device handleDev          
VL53L1_DEV                     Dev = &dev;

int status, i, x, y, R;
float distance[25], INTdistance[64];

// 16 ROI configurations
VL53L1_UserRoi_t  roiConfig[25]; //For definig 25 ROI configurations



void setup() {

  uint8_t byteData;
  uint16_t wordData;

  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(115200);

  Dev->I2cDevAddr = 0x52; //Declaring the I2C address of the sensor

  VL53L1_software_reset(Dev);

  VL53L1_RdByte(Dev, 0x010F, &byteData);
  //Serial.print(F("VL53L1X Model_ID: "));
  //Serial.println(byteData, HEX);
  VL53L1_RdByte(Dev, 0x0110, &byteData);
  //Serial.print(F("VL53L1X Module_Type: "));
  //Serial.println(byteData, HEX);
  VL53L1_RdWord(Dev, 0x010F, &wordData);
  //Serial.print(F("VL53L1X: "));
  //Serial.println(wordData, HEX);

  //Serial.println(F("Autonomous Ranging Test"));
  status = VL53L1_WaitDeviceBooted(Dev);
  status = VL53L1_DataInit(Dev);
  status = VL53L1_StaticInit(Dev);
  status = VL53L1_SetDistanceMode(Dev, VL53L1_DISTANCEMODE_LONG);
  status = VL53L1_SetMeasurementTimingBudgetMicroSeconds(Dev, 50000);
  status = VL53L1_SetInterMeasurementPeriodMilliSeconds(Dev, 50); // reduced to 50 ms from 500 ms in ST example
  status = VL53L1_StartMeasurement(Dev);

  if (status)
  {
    //Serial.println(F("VL53L1_StartMeasurement failed"));
    while (1);
  }


  // Creating 16 ROI definition
  i = 0;
  for (y = 0; y < 5; y++) {
    for (x = 0; x < 5; x++) {
      roiConfig[i] = {3 * x, (15 - 3 * y), (3 * x + 3), (15 - 3 * y - 3)}; //{TopLeftX,TopLeftY,BotRightX,BotRightY} 
                                                                           // Y sets at top first X scans from left to right by scale of 4
      /*                                                                    // Y comes down by scale of four
      Serial.print("i: "); 
      Serial.println(i); 
      Serial.print(roiConfig[i].TopLeftX);
      Serial.print(", ");
      Serial.print(roiConfig[i].TopLeftY);
      Serial.print(", ");
      Serial.print(roiConfig[i].BotRightX);
      Serial.print(", ");
      Serial.println(roiConfig[i].BotRightY);
    */
      
      i++;
    }
  }


}

void loop() {
  static VL53L1_RangingMeasurementData_t RangingData;

  status = VL53L1_WaitMeasurementDataReady(Dev);
  if (!status)
  {
    status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);
    if (status == 0)
    {
      for (i = 0; i < 25; i++) {
        // switching ROI configs
        status = VL53L1_SetUserROI(Dev, &roiConfig[i]);
        //    while (digitalRead(INT)); // slightly faster
        status = VL53L1_WaitMeasurementDataReady(Dev);
        if (!status) status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);  //4mS
        VL53L1_clear_interrupt_and_enable_next_range(Dev, VL53L1_DEVICEMEASUREMENTMODE_SINGLESHOT); //2mS
        if (status == 0) distance[i] = RangingData.RangeMilliMeter;
      }
    }
    status = VL53L1_ClearInterruptAndStartMeasurement(Dev);
  }
  else
  {
    //Serial.print(F("error waiting for data ready: "));
    //Serial.println(status);
  }
  //Serial.println("New distances:");
  for (i = 0; i < 25 ; i++)
  {
    //Serial.print(i);
    Serial.print(distance[i]);
    Serial.print(" , ");
  }
  Serial.println();
}
