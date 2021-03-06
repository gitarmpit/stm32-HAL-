/*
 * This file is part of baseflight
 * Licensed under GPL V3 or modified DCL - see https://github.com/multiwii/baseflight/blob/master/README.md
 */

#include "board.h"
#include "mw.h"
#include "drv_mpu.h"
#include "utils.h"

uint16_t calibratingA = 0;      // the calibration is done is the main loop. Calibrating decreases at each cycle down to 0, then we enter in a normal mode.
uint16_t calibratingB = 0;      // baro calibration = get new ground pressure value
uint16_t calibratingG = 0;
uint16_t acc_1G = 256;          // this is the 1G measured acceleration.
int16_t heading, magHold;

extern uint16_t InflightcalibratingA;
extern bool AccInflightCalibrationMeasurementDone;
extern bool AccInflightCalibrationSavetoEEProm;
extern bool AccInflightCalibrationActive;
extern uint16_t batteryWarningVoltage;
extern uint16_t batteryCriticalVoltage;
extern uint8_t batteryCellCount;
extern float magneticDeclination;

sensor_t acc;                       // acc access functions
sensor_t gyro;                      // gyro access functions
sensor_t mag;                       // mag access functions
baro_t baro;                        // barometer access functions
//uint8_t accHardware = ACC_DEFAULT;  // which accel chip is used/detected
uint8_t magHardware = MAG_DEFAULT;

//TODO
static bool has_baro, has_mag, has_gps;

bool sensors_has_baro()
{
    return has_baro;
}

bool sensors_has_mag()
{
    return has_mag;
}


bool sensors_has_gps()
{
    return has_gps;
}


bool init_sensors(void)
{
    int16_t deg, min;
    mpu_params_t mpu_config;
    bool haveMpu = false;

    // mpu driver parameters
    mpu_config.lpf = mcfg.gyro_lpf;
    // Autodetect Invensense acc/gyro hardware
    if (!mpuDetect(&acc, &gyro, &mpu_config))
    {
        return false;
    }

    // MPU6500 on I2C bus

    //TODO

#ifdef BARO
    // Detect what pressure sensors are available. baro->update() is set to sensor-specific update function
    if (!bmp280Detect(&baro)) {
        if (!bmp085Detect(&baro)) {
            // ms5611 disables BMP085, and tries to initialize + check PROM crc.
            // moved 5611 init here because there have been some reports that calibration data in BMP180
            // has been "passing" ms5611 PROM crc check
            if (!ms5611Detect(&baro)) {
                // if both failed, we don't have anything
                sensorsClear(SENSOR_BARO);
            }
        }
    }
#endif

    // Now time to init things, acc first
    acc.init(mcfg.acc_align);
    // this is safe because either mpu6050 or mpu3050 or lg3d20 sets it, and in case of fail, we never get here.
    gyro.init(mcfg.gyro_align);

    //TODO
    //bool rc = hmc5883lDetect(&mag);
    //bool rc = ak8975detect(&mag);

    // calculate magnetic declination
    deg = cfg.mag_declination / 100;
    min = cfg.mag_declination % 100;

    //TODO
    //magneticDeclination = (deg + ((float)min * (1.0f / 60.0f))) * 10; // heading is in 0.1deg units
    magneticDeclination = 0.0f;

    return true;
}

uint16_t RSSI_getValue(void)
{
    uint16_t value = 0;

    if (mcfg.rssi_aux_channel > 0) {
        const int16_t rssiChannelData = rcData[AUX1 + mcfg.rssi_aux_channel - 1];
        // Range of rssiChannelData is [1000;2000]. rssi should be in [0;1023];
        value = (uint16_t)((constrain(rssiChannelData - 1000, 0, mcfg.rssi_aux_max) / (float) mcfg.rssi_aux_max) * 1023.0f);
    } else if (mcfg.rssi_adc_channel > 0) {
        //TODO
        const int16_t rssiData = 0; //(((int32_t)(adcGetChannel(ADC_RSSI) - mcfg.rssi_adc_offset)) * 1023L) / mcfg.rssi_adc_max;
        // Set to correct range [0;1023]
        value = constrain(rssiData, 0, 1023);
    }

    // return range [0;1023]
    return value;
}

uint16_t batteryAdcToVoltage(uint16_t src)
{
    // calculate battery voltage based on ADC reading
    // result is Vbatt in 0.1V steps. 3.3V = ADC Vref, 4095 = 12bit adc, 110 = 11:1 voltage divider (10k:1k) * 10 for 0.1V
    return (((src) * 3.3f) / 4095) * mcfg.vbatscale;
}

#define ADCVREF 33L
int32_t currentSensorToCentiamps(uint16_t src)
{
    int32_t millivolts;

    millivolts = ((uint32_t)src * ADCVREF * 100) / 4095;
    millivolts -= mcfg.currentoffset;

    return (millivolts * 1000) / (int32_t)mcfg.currentscale; // current in 0.01A steps
}

//TODO
//void batteryInit(void)
//{
//    uint32_t i;
//    uint32_t voltage = 0;
//
//    // average up some voltage readings
//    for (i = 0; i < 32; i++) {
//        voltage += adcGetChannel(ADC_BATTERY);
//        delay(10);
//    }
//
//    voltage = batteryAdcToVoltage((uint16_t)(voltage / 32));
//
//    // autodetect cell count, going from 2S..8S
//    for (i = 1; i < 8; i++) {
//        if (voltage < i * mcfg.vbatmaxcellvoltage)
//            break;
//    }
//    batteryCellCount = i;
//    batteryWarningVoltage = i * mcfg.vbatwarningcellvoltage; // 3.5V per cell minimum, configurable in CLI
//    batteryCriticalVoltage = i * mcfg.vbatmincellvoltage; // 3.3V per cell minimum, configurable in CLI
//}

static void ACC_Common(void)
{
    static int32_t a[3];
    int axis;

    if (calibratingA > 0) {
        for (axis = 0; axis < 3; axis++) {
            // Reset a[axis] at start of calibration
            if (calibratingA == CALIBRATING_ACC_CYCLES)
                a[axis] = 0;
            // Sum up CALIBRATING_ACC_CYCLES readings
            a[axis] += accADC[axis];
            // Clear global variables for next reading
            accADC[axis] = 0;
            mcfg.accZero[axis] = 0;
        }
        // Calculate average, shift Z down by acc_1G and store values in EEPROM at end of calibration
        if (calibratingA == 1) {
            mcfg.accZero[ROLL] = (a[ROLL] + (CALIBRATING_ACC_CYCLES / 2)) / CALIBRATING_ACC_CYCLES;
            mcfg.accZero[PITCH] = (a[PITCH] + (CALIBRATING_ACC_CYCLES / 2)) / CALIBRATING_ACC_CYCLES;
            mcfg.accZero[YAW] = (a[YAW] + (CALIBRATING_ACC_CYCLES / 2)) / CALIBRATING_ACC_CYCLES - acc_1G;
            cfg.angleTrim[ROLL] = 0;
            cfg.angleTrim[PITCH] = 0;
            writeEEPROM(1, true);      // write accZero in EEPROM
        }
        calibratingA--;
    }

//TODO
#ifdef FEATURE_INFLIGHT_ACC_CAL
    {
        static int32_t b[3];
        static int16_t accZero_saved[3] = { 0, 0, 0 };
        static int16_t angleTrim_saved[2] = { 0, 0 };
        // Saving old zeropoints before measurement
        if (InflightcalibratingA == 50) {
            accZero_saved[ROLL] = mcfg.accZero[ROLL];
            accZero_saved[PITCH] = mcfg.accZero[PITCH];
            accZero_saved[YAW] = mcfg.accZero[YAW];
            angleTrim_saved[ROLL] = cfg.angleTrim[ROLL];
            angleTrim_saved[PITCH] = cfg.angleTrim[PITCH];
        }
        if (InflightcalibratingA > 0) {
            for (axis = 0; axis < 3; axis++) {
                // Reset a[axis] at start of calibration
                if (InflightcalibratingA == 50)
                    b[axis] = 0;
                // Sum up 50 readings
                b[axis] += accADC[axis];
                // Clear global variables for next reading
                accADC[axis] = 0;
                mcfg.accZero[axis] = 0;
            }
            // all values are measured
            if (InflightcalibratingA == 1) {
                AccInflightCalibrationActive = false;
                AccInflightCalibrationMeasurementDone = true;
                buzzer(BUZZER_ACC_CALIBRATION);      // buzzer for indicatiing the end of calibration
                // recover saved values to maintain current flight behavior until new values are transferred
                mcfg.accZero[ROLL] = accZero_saved[ROLL];
                mcfg.accZero[PITCH] = accZero_saved[PITCH];
                mcfg.accZero[YAW] = accZero_saved[YAW];
                cfg.angleTrim[ROLL] = angleTrim_saved[ROLL];
                cfg.angleTrim[PITCH] = angleTrim_saved[PITCH];
            }
            InflightcalibratingA--;
        }
        // Calculate average, shift Z down by acc_1G and store values in EEPROM at end of calibration
        if (AccInflightCalibrationSavetoEEProm) {      // the copter is landed, disarmed and the combo has been done again
            AccInflightCalibrationSavetoEEProm = false;
            mcfg.accZero[ROLL] = b[ROLL] / 50;
            mcfg.accZero[PITCH] = b[PITCH] / 50;
            mcfg.accZero[YAW] = b[YAW] / 50 - acc_1G;    // for nunchuk 200=1G
            cfg.angleTrim[ROLL] = 0;
            cfg.angleTrim[PITCH] = 0;
            writeEEPROM(1, true);          // write accZero in EEPROM
        }
    }
#endif

    accADC[ROLL] -= mcfg.accZero[ROLL];
    accADC[PITCH] -= mcfg.accZero[PITCH];
    accADC[YAW] -= mcfg.accZero[YAW];
}

void ACC_getADC(void)
{
    acc.read(accADC);
    ACC_Common();
}

#ifdef BARO
void Baro_Common(void)
{
    static int32_t baroHistTab[BARO_TAB_SIZE_MAX];
    static int baroHistIdx;
    int indexplus1;

    indexplus1 = (baroHistIdx + 1);
    if (indexplus1 == cfg.baro_tab_size)
        indexplus1 = 0;
    baroHistTab[baroHistIdx] = baroPressure;
    baroPressureSum += baroHistTab[baroHistIdx];
    baroPressureSum -= baroHistTab[indexplus1];
    baroHistIdx = indexplus1;
}

int Baro_update(void)
{
    static uint32_t baroDeadline = 0;
    static int state = 0;

    if ((int32_t)(currentTime - baroDeadline) < 0)
        return 0;

    baroDeadline = currentTime;

    if (state) {
        baro.get_up();
        baro.start_ut();
        baroDeadline += baro.ut_delay;
        baro.calculate(&baroPressure, &baroTemperature);
        state = 0;
        return 2;
    } else {
        baro.get_ut();
        baro.start_up();
        Baro_Common();
        state = 1;
        baroDeadline += baro.up_delay;
        return 1;
    }
}
#endif /* BARO */

typedef struct stdev_t {
    float m_oldM, m_newM, m_oldS, m_newS;
    int m_n;
} stdev_t;

static void devClear(stdev_t *dev)
{
    dev->m_n = 0;
}

static void devPush(stdev_t *dev, float x)
{
    dev->m_n++;
    if (dev->m_n == 1) {
        dev->m_oldM = dev->m_newM = x;
        dev->m_oldS = 0.0f;
    } else {
        dev->m_newM = dev->m_oldM + (x - dev->m_oldM) / dev->m_n;
        dev->m_newS = dev->m_oldS + (x - dev->m_oldM) * (x - dev->m_newM);
        dev->m_oldM = dev->m_newM;
        dev->m_oldS = dev->m_newS;
    }
}

static float devVariance(stdev_t *dev)
{
    return ((dev->m_n > 1) ? dev->m_newS / (dev->m_n - 1) : 0.0f);
}

static float devStandardDeviation(stdev_t *dev)
{
    return sqrtf(devVariance(dev));
}

static void GYRO_Common(void)
{
    int axis;
    static int32_t g[3];
    static stdev_t var[3];

    if (calibratingG > 0) {
        for (axis = 0; axis < 3; axis++) {
            // Reset g[axis] at start of calibration
            if (calibratingG == CALIBRATING_GYRO_CYCLES) {
                g[axis] = 0;
                devClear(&var[axis]);
            }
            // Sum up 1000 readings
            g[axis] += gyroADC[axis];
            devPush(&var[axis], gyroADC[axis]);
            // Clear global variables for next reading
            gyroADC[axis] = 0;
            gyroZero[axis] = 0;
            if (calibratingG == 1) {
                float dev = devStandardDeviation(&var[axis]);
                // check deviation and startover if idiot was moving the model
                if (mcfg.moron_threshold && dev > mcfg.moron_threshold) {
                    calibratingG = CALIBRATING_GYRO_CYCLES;
                    devClear(&var[0]);
                    devClear(&var[1]);
                    devClear(&var[2]);
                    g[0] = g[1] = g[2] = 0;
                    continue;
                }
                gyroZero[axis] = (g[axis] + (CALIBRATING_GYRO_CYCLES / 2)) / CALIBRATING_GYRO_CYCLES;
            }
        }
        calibratingG--;
    }
    for (axis = 0; axis < 3; axis++)
        gyroADC[axis] -= gyroZero[axis];
}

void Gyro_getADC(void)
{
    // range: +/- 8192; +/- 2000 deg/sec
    gyro.read(gyroADC);
    GYRO_Common();
}

static uint8_t magInit = 0;

void Mag_init(void)
{
    // initialize and calibration. turn on led during mag calibration (calibration routine blinks it)
    mag.init(mcfg.mag_align);
    magInit = 1;
}

int Mag_getADC(void)
{
    static uint32_t t, tCal = 0;
    static int16_t magZeroTempMin[3];
    static int16_t magZeroTempMax[3];
    uint32_t axis;

    if ((int32_t)(currentTime - t) < 0)
        return 0;                 //each read is spaced by 100ms
    t = currentTime + 100000;

    // Read mag sensor
    mag.read(magADC);

    if (flags.CALIBRATE_MAG) {
        tCal = t;
        for (axis = 0; axis < 3; axis++) {
            mcfg.magZero[axis] = 0;
            magZeroTempMin[axis] = magADC[axis];
            magZeroTempMax[axis] = magADC[axis];
        }
        flags.CALIBRATE_MAG = 0;
    }

    if (magInit) {              // we apply offset only once mag calibration is done
        magADC[X] -= mcfg.magZero[X];
        magADC[Y] -= mcfg.magZero[Y];
        magADC[Z] -= mcfg.magZero[Z];
    }

    if (tCal != 0) {
        if ((t - tCal) < 30000000) {    // 30s: you have 30s to turn the multi in all directions
            for (axis = 0; axis < 3; axis++) {
                if (magADC[axis] < magZeroTempMin[axis])
                    magZeroTempMin[axis] = magADC[axis];
                if (magADC[axis] > magZeroTempMax[axis])
                    magZeroTempMax[axis] = magADC[axis];
            }
        } else {
            tCal = 0;
            for (axis = 0; axis < 3; axis++)
                mcfg.magZero[axis] = (magZeroTempMin[axis] + magZeroTempMax[axis]) / 2; // Calculate offsets
            writeEEPROM(1, true);
        }
    }

    return 1;
}

//TODO
#ifdef SONAR

void Sonar_init(void)
{
    hcsr04_init(sonar_rc78);
    sensorsSet(SENSOR_SONAR);
    sonarAlt = 0;
}

void Sonar_update(void)
{
    hcsr04_get_distance(&sonarAlt);
}

#endif
