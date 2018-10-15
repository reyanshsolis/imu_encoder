## imu_encoder
# ODOMETRY USING A WHEEL MOUNTED INERTIAL SENSOR 

_________________________________________________________________
Reference ::

Link to git_repo ::
 [inertial_sensor](https://github.com/reyanshsolis/imu_encoder.git)
 [gyro_filter](https://github.com/reyanshsolis/gyro_filter.git)

[Link](https://drive.google.com/drive/folders/1lIECSTog7db8Fpb0fIVhElLElcQSb51u) : RAW BAG Files 

[Link](https://drive.google.com/open?id=1SxP2YKimKd4hqK0ZQKdy-GLh4WCUiB7t) to Google Drive::contains bag files, research papers, results

Credits: Rishabh Singh and Shubham Sahoo
__________________________________________________________________


## WHEEL MOUNTED INERTIAL SENSOR :

### Transmition Module: 
   ### 1.Using nRF: 
   Initially nRF was being used for transmitting data from Encoder 
   to Microprocessor which then publishes RAW data on a topic /enc_left (left 
   encoder values) and /enc_right (right encoder values) on ROS which is 
   suscribed by kalman.cpp and noise.py 
   nRF was used in different modes to observe and obtain the most efficient 
   mode of communication:
	
	Basic data sent by nRF : 
		identifier bit
		 (to inform if the data is coming from Left or Right Encoder)
		dc_offset
		 (calculated by mean avg. by keeping the encoder at  rest for 5sec.)
		gyro data : gx gy gz(used for calculating yaw)
 
	
	Mode 0: 
		data was sent as an array, with data[0]->identifier, data[1]-> yaw
		(after processing raw data, subtracting dc_offset and taking the 
		cosine component to reduce angular installation correction***(1))
	Mode 1:
		As for further experimentaion all gx gy gz was needed,
		data was sent as data[0]->Identifier+dc_offset
		data[1]->gx data[2]->gy data[3]->gz
		 To Identify Left and Right Side and get DC Offset: float data[4]
		  is transmitted from Encoder, 
		  data[0] contains the information of side and dc_offset.
		  For example: data[0] = 8500+dc_offset for Left Encoder. 
		 	(So int(data[0])/1000==8 for Left) data[0] = 6500+dc_offset for 
		 	Right Encoder. (So int(data[0])/1000==6 for Right)
	Mode 2:
		Using Multiple Pipes of nRF
			Left and Right Encoder data was simultaneously published to 
			different channel of nRF.
			This however didn't show any significant improvement as even for
			this the code takes the address of the pipe as Identifier.
	Mode 3: uses Mode 1 with Mapped output. 
	Mode 4: uses Mode 3: plus using Separate reciever for separate
			transciever for	improving data transmision rate (2Rx for 2Tx)
			This is the final code that is to be used for transmitting raw 
			data, if nRF is used for wireless communication


	The data rate from nRF was observed to be very poor with frequent breaks 
	in connections.
	So it was decided to move to Zigbee for further experimentation.
	(All the bag files/ data/ observations till 16th October are based on 
	reading with nRF used for wireless communication)	
	
   ### 2. Using Zigbee: <Yet to be tested>

## ISSUES AND SOLUTION : 
	
	***(1): Encoder if not placed Verticle to the Ground, The reading has an 
	angular component, which adds to significant error specially at turning 
	and on high speeds.
	Presently the problem is avoided for a while by keeping it parallel to 
	the wheel.rrallelallel
		The problem is yet to be solved by considering Angular Component and 
		changes due to it.

	**There were many issues with Multiple Arduino Serial Communication in ROS
    	It was solved using working by creating a new workspace in ROS.


DATA BIAS CORRECTION AND NOISE REMOVAL : [LINK](https://github.com/reyanshsolis/gyro_filter.git)
_________________________________________________________________
