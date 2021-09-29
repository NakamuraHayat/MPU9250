#ifndef MPU9250_NH_H
#define MPU9250_NH_H

#include "Arduino.h"
#include <Wire.h>

#define ADDR_HIGH       0b1101001
#define ADDR_LOW        0b1101000
#define ACCEL_CONFIG    0x1C
#define GYRO_CNFIG      0x1B
#define INT_PIN_CFG     0x37
#define ACCEL_XOUT_H    0x3B
#define GYRO_XOUT_H     0x43
#define PWR_MGMT_1      0x6B
#define WHO_AM_I        0x75

#define AK8963_ADDR     0x0C
#define AK8963_WIA      0x00
#define AK8963_HXL      0x03
#define AK8963_CNTL1    0x0A

typedef enum {
    AD0_LOW_  = ADDR_LOW, 
    AD0_HIGH_ = ADDR_HIGH
} i2c_address_t;

typedef enum {
    a_2g_  = 0b00000000,
    a_4g_  = 0b00001000,
    a_8g_  = 0b00010000,
    a_16g_ = 0b00011000
} accel_fs_sel_t;

typedef enum {
    g_250dps_  = 0b00000000,
    g_500dps_  = 0b00001000,
    g_1000dps_ = 0b00010000,
    g_2000dps_ = 0b00011000
} gyro_fs_sel_t; 


class MPU9250 {
    private:
        int address = AD0_LOW_;
        int who_am_i;
        int ak8963_wia;

        float accel_sensitivity;
        float gyro_sensitivity;
        float mag_sensitivity;

        float accel_offset[3] = {0, 0, 0};
        float gyro_offset[3]  = {0, 0, 0};
        float mag_offset[3]   = {0, 0, 0};

    public:
        MPU9250(i2c_address_t i2c_address);
        void begin();
        bool available();

        void set(accel_fs_sel_t accel_fs_sel, gyro_fs_sel_t gyro_fs_sel);
        void setAccel(accel_fs_sel_t accel_fs_sel);
        void setGyro(gyro_fs_sel_t gyro_fs_sel);
        void setMag();

        void offset(float *accel_offset, float *gyro_offset, float *mag_offset);
        void offsetAccel(float *accel);
        void offsetGyro(float *gyro);
        void offsetMag(float *mag);

        void get(float *accel, float *gyro, float *mag);
        void getAccel(float *accel);
        void getGyro(float *gyro);
        void getMag(float *mag);  
};

#endif