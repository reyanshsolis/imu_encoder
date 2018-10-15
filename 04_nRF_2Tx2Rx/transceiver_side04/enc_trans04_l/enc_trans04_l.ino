// Keep the circuit stable on a surface for atleast 10 seconds to set offset
// enc_trans03_r - transmits RAW data from IMU : ax ay az gx gy gz dc_offset 
// LEFT SIDE WHEEL


#include <Wire.h>  
#include <LSM6.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

long int sampleNum = 1000; 
float dc_offset = 0;
const byte address = 0xf0f0f0f0c3;
long int predata[5] = {0,0,0,0,0};

RF24 radio(9, 10); // CE, CSN Pins

//unsigned long microsPerReading, microsPrevious;
//float accelScale, gyroScale;

LSM6 imu;

char report[80];

void setup()
{
  delay(5000);
  
  Serial.begin(57600);
  Wire.begin();
  Serial.println("Code has started");
  if (!imu.init())
  {
    Serial.println("Failed to detect and initialize IMU!");
    while (1);
  }
  
  imu.writeReg(LSM6::CTRL9_XL, 0x38);  // Acc X, Y, Z axes enabled
  imu.writeReg(LSM6::CTRL1_XL, 0x70);  // Acc = 833Hz (High-Performance mode)
  imu.writeReg(LSM6::INT1_CTRL, 0x01); // Acc Data Ready interrupt on INT1
  imu.writeReg(LSM6::CTRL10_C, 0x38);  // Gyro X, Y, Z axes enabled
  imu.writeReg(LSM6::CTRL2_G, 0x7C);   // Gyro = 833Hz (High-Performance mode) and 2000dps
  imu.writeReg(LSM6::INT2_CTRL, 0x02); // Gyro Data Ready interrupt on INT1
  
  for(int n=0;n<sampleNum;n++)
  { 
    imu.writeReg(LSM6::STATUS_REG, 0x0B); //this hex number represents the status register
    imu.read(); 
    snprintf(report, sizeof(report), "A: %6d %6d %6d    G: %6d %6d %6d", imu.a.x, imu.a.y, imu.a.z, imu.g.x, imu.g.y, imu.g.z);
    dc_offset += imu.g.z;
  } 
  
  //Serial.println(dc_offset);
  dc_offset =dc_offset/sampleNum;
  //Serial.println(dc_offset); 
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  //radio.write(&dc_offset, sizeof(dc_offset));
  Serial.println(dc_offset);
  
  //ok = radio.write(&dc_offset, sizeof(dc_offset));
  //delay(2000);
}

void loop()
{ 
  float yaw0;
  float yaw, gz;
  //double aix, aiy, aiz;
  double gix, giy, giz;
  //unsigned long microsNow;
  //double ai, cos_theta_z, sin_theta_z;
    
  imu.writeReg(LSM6::STATUS_REG, 0x0B); //this hex number represents the status register
  imu.read();
  snprintf(report, sizeof(report), "A: %6d %6d %6d    G: %6d %6d %6d", imu.a.x, imu.a.y, imu.a.z, imu.g.x, imu.g.y, imu.g.z);

  //aix = imu.a.x;
  //aiy = imu.a.y;
  //aiz = imu.a.z;
  
  gix = convertRawGyro(imu.g.x);//6.0;
  giy = convertRawGyro(imu.g.y);//6.0;
  
  //giz = 1.126*(imu.g.z - dc_offset);
  
  giz = convertRawGyro(imu.g.z);//6.0;
  
  float data[4];

  data[0]=6500.0+dc_offset; //data[0]%1000=6 for LEFT
  data[1]=gix;
  data[2]=giy;
  data[3]=giz;
 
  bool ok = radio.write(&data, sizeof(data));

 
  if (ok)
      Serial.println("ok...\n\r");
  else
      Serial.println("failed.\n\r");
 
 }


float convertRawGyro(int gRaw)
{ 
  // since we are using 2000 degrees/seconds range
  // -2000 maps to a raw value of -32768
  // +2000 maps to a raw value of 32767
  
  //float g = (gRaw * 2000.0) / 32768.0;
  return gRaw*0.010172526;
}


/*
   ai = sqrt(sq(aix)+sq(aiy)+sq(aiz));
  sin_theta_z = (-aiz/ai);
  cos_theta_z = sqrt(1 - (sin_theta_z*sin_theta_z)) ;

  giz = giz / cos_theta_z;
*/
  
/*
float convertRawAcceleration(int aRaw) 
{
  // since we are using 2G range
  // -2g maps to a raw value of -32768
  // +2g maps to a raw value of 32767
  
  float a = (aRaw * 2.0) / 32768.0;
  return a;
}
*/

    

 

