/*TODO 最低限の機能しかつけれませんでした。ほかの機能についてはよくわからないか、必要ないと判断しました。*/

#include "MPU9250_NH.h"

MPU9250 mpu9250(AD0_LOW_);

float accel[3], gyro[3], mag[3];
float accel_offset[] = {0, 0, 0};
float gyro_offset[] = {0, 0, 0};
float mag_offset[] = {0, 0, 0};

void setup() {
  Serial.begin(9600);
  mpu9250.begin();
  mpu9250.set(a_4g_, g_1000dps_);
  mpu9250.offset(accel_offset, gyro_offset, mag_offset);
}

void loop() {
  if (mpu9250.available()) {
    mpu9250.get(accel, gyro, mag);
    Serial.print(accel[0]);
    Serial.print(",");
    Serial.print(accel[1]);
    Serial.print(",");
    Serial.print(accel[2]);
    Serial.print(",");
    Serial.print(gyro[0]);
    Serial.print(",");
    Serial.print(gyro[1]);
    Serial.print(",");
    Serial.print(gyro[2]);
    Serial.print(",");
    Serial.print(mag[0]);
    Serial.print(",");
    Serial.print(mag[1]);
    Serial.print(",");
    Serial.print(mag[2]);
    Serial.println();
  }
  
  else {
    Serial.println("error");
  }
}
