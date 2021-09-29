#include "MPU9250_NH.h"

MPU9250::MPU9250(i2c_address_t i2c_address) {
    address = i2c_address;
}

void MPU9250::begin() {
    Wire.begin();

    Wire.beginTransmission(address);
    Wire.write(INT_PIN_CFG);
    Wire.write(0x02);
    Wire.endTransmission();

    Wire.beginTransmission(address);
    Wire.write(PWR_MGMT_1);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.beginTransmission(AK8963_ADDR);
    Wire.write(AK8963_CNTL1);
    Wire.write(0x12);
    Wire.endTransmission();

    set(a_16g_, g_2000dps_);
    offset(accel_offset, gyro_offset, mag_offset);
}

bool MPU9250::available() {
    Wire.beginTransmission(address);
    Wire.write(WHO_AM_I);
    Wire.endTransmission();
    Wire.requestFrom(address, 1);
    who_am_i = Wire.read();

    Wire.beginTransmission(AK8963_ADDR);
    Wire.write(AK8963_WIA);
    Wire.endTransmission();
    Wire.requestFrom(AK8963_ADDR, 1);
    ak8963_wia = Wire.read();

    if ((who_am_i == 0x71) && (ak8963_wia == 0x48)) {
        return true;
    }
    else{
        return false;
    }
}

void MPU9250::set(accel_fs_sel_t accel_fs_sel, gyro_fs_sel_t gyro_fs_sel) {
    setAccel(accel_fs_sel);
    setGyro(gyro_fs_sel);
    setMag();
}

void MPU9250::setAccel(accel_fs_sel_t accel_fs_sel) {
    Wire.beginTransmission(address);
    Wire.write(ACCEL_CONFIG);
    Wire.write(accel_fs_sel);
    Wire.endTransmission();

    switch (accel_fs_sel) {
    case a_2g_:
        accel_sensitivity = 4.0 / 65536.0;
        break;
    case a_4g_:
        accel_sensitivity = 8.0 / 65536.0;
        break;
    case a_8g_:
        accel_sensitivity = 16.0 / 65536.0;
        break;
    case a_16g_:
        accel_sensitivity = 32.0 / 65536.0;
        break;
    }
}

void MPU9250::setGyro(gyro_fs_sel_t gyro_fs_sel) {
    Wire.beginTransmission(address);
    Wire.write(GYRO_CNFIG);
    Wire.write(gyro_fs_sel);
    Wire.endTransmission();

    switch (gyro_fs_sel) {
    case g_250dps_:
        gyro_sensitivity = 500.0 / 65536.0;
        break;
    case g_500dps_:
        gyro_sensitivity = 1000.0 / 65536.0;
        break;
    case g_1000dps_:
        gyro_sensitivity = 2000.0 / 65536.0;
        break;
    case g_2000dps_:
        gyro_sensitivity = 4000.0 / 65536.0;
        break;
    }
}

void MPU9250::setMag() {
    mag_sensitivity = 0.15;
}

void MPU9250::offset(float *accel, float *gyro, float *mag) {
    offsetAccel(accel);
    offsetGyro(gyro);
    offsetMag(mag);
}

void MPU9250::offsetAccel(float *accel) {
    accel_offset[0] = accel[0];
    accel_offset[1] = accel[1];
    accel_offset[2] = accel[2];
}

void MPU9250::offsetGyro(float *gyro) {
    gyro_offset[0] = gyro[0];
    gyro_offset[1] = gyro[1];
    gyro_offset[2] = gyro[2];
}

void MPU9250::offsetMag(float *mag) {
    mag_offset[0] = mag[0];
    mag_offset[1] = mag[1];
    mag_offset[2] = mag[2];
}

void MPU9250::get(float *accel, float *gyro, float *mag) {
    getAccel(accel);
    getGyro(gyro);
    getMag(mag);
}

void MPU9250::getAccel(float *accel) {
    Wire.beginTransmission(address);
    Wire.write(ACCEL_XOUT_H);
    Wire.endTransmission();
    Wire.requestFrom(address, 6);
    while (Wire.available()) {
        accel[0] = int16_t(Wire.read() << 8 | Wire.read()) * accel_sensitivity - accel_offset[0];
        accel[1] = int16_t(Wire.read() << 8 | Wire.read()) * accel_sensitivity - accel_offset[1];
        accel[2] = int16_t(Wire.read() << 8 | Wire.read()) * accel_sensitivity - accel_offset[2];
    }
}

void MPU9250::getGyro(float *gyro) {
    Wire.beginTransmission(address);
    Wire.write(GYRO_XOUT_H);
    Wire.endTransmission();
    Wire.requestFrom(address, 6);
    while (Wire.available()) {
        gyro[0] = int16_t(Wire.read() << 8 | Wire.read()) * gyro_sensitivity - gyro_offset[0];
        gyro[1] = int16_t(Wire.read() << 8 | Wire.read()) * gyro_sensitivity - gyro_offset[1];
        gyro[2] = int16_t(Wire.read() << 8 | Wire.read()) * gyro_sensitivity - gyro_offset[2];
    }
}

void MPU9250::getMag(float *mag) {
    Wire.beginTransmission(AK8963_ADDR);
    Wire.write(AK8963_HXL);
    Wire.endTransmission();
    Wire.requestFrom(AK8963_ADDR, 7);
    while (Wire.available()) {
        mag[0] = int16_t(Wire.read() | Wire.read() << 8) * mag_sensitivity - mag_offset[0];
        mag[1] = int16_t(Wire.read() | Wire.read() << 8) * mag_sensitivity - mag_offset[1];
        mag[2] = int16_t(Wire.read() | Wire.read() << 8) * mag_sensitivity - mag_offset[2];
        Wire.read();
    }
}
