/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GYRO_H
#define _GYRO_H
#include <stdint.h>
#include "sensors/sensors.h"
#include "filter.h"

#define GYRO_LPF_256HZ      0
#define GYRO_LPF_188HZ      1
#define GYRO_LPF_98HZ       2
#define GYRO_LPF_42HZ       3
#define GYRO_LPF_20HZ       4
#define GYRO_LPF_10HZ       5
#define GYRO_LPF_5HZ        6
#define GYRO_LPF_NONE       7

typedef enum {
    GYRO_NONE = 0,
    GYRO_DEFAULT,
    GYRO_MPU6050,
    GYRO_L3G4200D,
    GYRO_MPU3050,
    GYRO_L3GD20,
    GYRO_MPU6000,
    GYRO_MPU6500,
    GYRO_FAKE
} gyroSensor_e;

struct gyroConfig_t
{
    uint8_t gyroMovementCalibrationThreshold;   // people keep forgetting that moving model while init results in wrong gyro offsets. and then they never reset gyro. so this is now on by default.
    uint8_t gyro_lpf;                           // gyro LPF setting - values are driver specific, in case of invalid number, a reasonable default ~30-40HZ is chosen.
    uint8_t gyro_soft_type;                     // Gyro Filter Type
    uint16_t gyro_soft_lpf_hz;                  // Software based gyro filter in hz
    uint16_t gyro_soft_notch_hz;                // Biquad gyro notch hz
    uint16_t gyro_soft_notch_cutoff_hz;         // Biquad gyro notch low cutoff in hz
    uint8_t gyro_sync;                          // Enable interrupt based loop
    uint16_t gyro_sample_hz;                    // The desired gyro sample frequency.
} ;




#define CALIBRATING_GYRO_CYCLES             1000

class MPU;

class Gyro
{
private:
    MPU* _mpu;
    sensor_align_e gyroAlign;
    int32_t gyroZero[3] = { 0, 0, 0 };

    int32_t gyroADC[3];
    float   gyroADCf[3];

    biquadFilter_t gyroFilterLPF[3];
    biquadFilter_t gyroFilterNotch[3];
    pt1Filter_t gyroFilterPt1[3];

    gyroConfig_t* gyroConfig;

    uint16_t calibratingG;


    void initGyroFilterCoefficients(void);

    void applyFilter();
    void performCalibration();

public:
    Gyro(MPU* mpu);

    void init();
    void startCalibration();
    void update();

    int32_t* getADC() { return gyroADC; }
    float* getADCf() { return gyroADCf; }

    gyroConfig_t* getGyroConfig() { return gyroConfig; }

};

#endif

