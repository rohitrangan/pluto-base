# Created By - Rohit Rangan
# Created On - 02-06-2012
# List of all Sensor files.

SENSORSRC = ${SENSORDIR}/IMU/hld/MPU60X0.c \
			${SENSORDIR}/IMU/IMUData.c \
	    	${SENSORDIR}/BarometricPressure/hld/BMP180.c \
	    	${SENSORDIR}/BarometricPressure/BarometerData.c \
	    	${SENSORDIR}/Magnetometer/hld/hmc5883.c \
	    	${SENSORDIR}/Magnetometer/MagnetometerData.c \
	    	${SENSORDIR}/DifferentialPressure/hld/ms4515.c
			
# Include Directories.

SENSORINC = ${SENSORDIR}/IMU \
			${SENSORDIR}/IMU/hld \
			${SENSORDIR}/BarometricPressure/ \
	    	${SENSORDIR}/BarometricPressure/hld \
	    	${SENSORDIR}/Magnetometer/ \
	    	${SENSORDIR}/Magnetometer/hld \
	    	${SENSORDIR}/DifferentialPressure/ \
	    	${SENSORDIR}/DifferentialPressure/hld
