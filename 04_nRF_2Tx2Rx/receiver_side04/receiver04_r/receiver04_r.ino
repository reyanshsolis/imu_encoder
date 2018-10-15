/*This is the arduino code for the nRF module at the receiving end. This modules 
  receives the velocities of the left and right wheels and publishes it on different
  ROS topics using ROS Serial.  
*/

    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>
    #include <ros.h>
    #include <std_msgs/Float32.h>
    #include <geometry_msgs/Twist.h>

    ros::NodeHandle nh;

    geometry_msgs::Twist gyro_data_r;

    float data[4]={0,0,0,0};

    ros::Publisher chatter1("enc_right", &gyro_data_r);

    float dc=0.0;
    
    RF24 radio(9,10)  ; // CNS, CE
    const byte address = 0x7878787878LL;
      
    void setup(){      
      nh.initNode();
      nh.advertise(chatter1);
      Serial.begin(57600);
      radio.begin();
      radio.openReadingPipe(1, address);
      radio.setPALevel(RF24_PA_MIN);
      
      radio.startListening();
    }
    
    void loop() 
    { 
      if (radio.available()) 
      { 
          radio.read(&data, sizeof(data));
          gyro_data_r.linear.x=data[0];
          gyro_data_r.angular.x=data[1];
          gyro_data_r.angular.y=data[2];
          gyro_data_r.angular.z=data[3];
          
          chatter1.publish(&gyro_data_r);
        }

      nh.spinOnce();
   }

