/*
 * icm20948_def.h
 *
 * Created: 7/6/2021 3:41:04 PM
 *  Author: VenkatesanMuthukumar
 */ 


#ifndef ICM20948_DEF_H_
#define ICM20948_DEF_H_

/*Constants*/
#define ICM20948 (0x69<<1)			// (1101001 << 1) I2C slave address when AD0=1
#define ICM20948_DEVICE_ID 0xEA		//Device ID value



#define I2C_READ    1		//data direction for I2C
#define I2C_WRITE   0		//data direction for I2C

#define ICM20948_BANK_0 (0 << 7) //register bank 0
/*registers used from bank 0*/
#define ICM20948_REG_WHO_AM_I               (ICM20948_BANK_0 | 0x00)    // Device ID register
#define ICM20948_REG_PWR_MGMT_1             (ICM20948_BANK_0 | 0x06)    // Power Management 1 register

#define ICM20948_REG_ACCEL_XOUT_H_SH        (ICM20948_BANK_0 | 0x2D)    // Accelerometer X-axis data high byte
#define ICM20948_REG_ACCEL_XOUT_L_SH        (ICM20948_BANK_0 | 0x2E)    // Accelerometer X-axis data low byte
#define ICM20948_REG_ACCEL_YOUT_H_SH        (ICM20948_BANK_0 | 0x2F)    // Accelerometer Y-axis data high byte
#define ICM20948_REG_ACCEL_YOUT_L_SH        (ICM20948_BANK_0 | 0x30)    // Accelerometer Y-axis data low byte
#define ICM20948_REG_ACCEL_ZOUT_H_SH        (ICM20948_BANK_0 | 0x31)    // Accelerometer Z-axis data high byte
#define ICM20948_REG_ACCEL_ZOUT_L_SH        (ICM20948_BANK_0 | 0x32)    // Accelerometer Z-axis data low byte

#define ICM20948_REG_GYRO_XOUT_H_SH         (ICM20948_BANK_0 | 0x33)    // Gyroscope X-axis data high byte
#define ICM20948_REG_GYRO_XOUT_L_SH         (ICM20948_BANK_0 | 0x34)    // Gyroscope X-axis data low byte
#define ICM20948_REG_GYRO_YOUT_H_SH         (ICM20948_BANK_0 | 0x35)    // Gyroscope Y-axis data high byte
#define ICM20948_REG_GYRO_YOUT_L_SH         (ICM20948_BANK_0 | 0x36)    // Gyroscope Y-axis data low byte
#define ICM20948_REG_GYRO_ZOUT_H_SH         (ICM20948_BANK_0 | 0x37)    // Gyroscope Z-axis data high byte
#define ICM20948_REG_GYRO_ZOUT_L_SH         (ICM20948_BANK_0 | 0x38)    // Gyroscope Z-axis data low byte




#endif /* ICM20948_DEF_H_ */