#include "AHRS.h"
#include <math.h>

#define Kp 2.0f * 5.0f // these are the free parameters in the Mahony filter and fusion scheme, Kp for proportional feedback, Ki for integral
#define Ki 0.0f


AHRS::AHRS()
{
	GyroMeasError = M_PI * (40.0f / 180.0f); // gyroscope measurement error in rads/s (start at 40 deg/s)
	GyroMeasDrift = M_PI * (0.0f / 180.0f); // gyroscope measurement drift in rad/s/s (start at 0.0 deg/s/s)

	// There is a tradeoff in the beta parameter between accuracy and response speed.
	// In the original Madgwick study, beta of 0.041 (corresponding to GyroMeasError of 2.7 degrees/s) was found to give optimal accuracy.
	// However, with this value, the LSM9SD0 response time is about 10 seconds to a stable initial quaternion.
	// Subsequent changes also require a longish lag time to a stable output, not fast enough for a quadcopter or robot car!
	// By increasing beta (GyroMeasError) by about a factor of fifteen, the response time constant is reduced to ~2 sec
	// I haven't noticed any reduction in solution accuracy. This is essentially the I coefficient in a PID control sense;
	// the bigger the feedback coefficient, the faster the solution converges, usually at the expense of accuracy.
	// In any case, this is the free parameter in the Madgwick filtering and fusion scheme.

	beta = sqrt(3.0f / 4.0f) * GyroMeasError;
	zeta = sqrt(3.0f / 4.0f) * GyroMeasDrift;
}


void AHRS::getEuler (float* y, float* p, float* r)
{
    // Define output variables from updated quaternion---these are Tait-Bryan angles, commonly used in aircraft orientation.
    // In this coordinate system, the positive z-axis is down toward Earth.
    // Yaw is the angle between Sensor x-axis and Earth magnetic North (or true North if corrected for local declination, looking down on the sensor positive yaw is counterclockwise.
    // Pitch is angle between sensor x-axis and Earth ground plane, toward the Earth is positive, up toward the sky is negative.
    // Roll is angle between sensor y-axis and Earth ground plane, y-axis up is positive roll.
    // These arise from the definition of the homogeneous rotation matrix constructed from quaternions.
    // Tait-Bryan angles as well as Euler angles are non-commutative; that is, the get the correct orientation the rotations must be
    // applied in the correct order which for this configuration is yaw, pitch, and then roll.
    // For more see http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles which has additional links.
    *y = atan2(2.0f * (q[1] * q[2] + q[0] * q[3]),  q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);
    *p = -asin(2.0f * (q[1] * q[3] - q[0] * q[2]));
    *r = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]),  q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);

    *y *= 180.0f / M_PI;
    *p *= 180.0f / M_PI;
    *r *= 180.0f / M_PI;

    //y -= 13.8; // Declination at Danville, California is 13 degrees 48 minutes and 47 seconds on 2014-04-04
    *y -= 4.39; //Indy

}



// Implementation of Sebastian Madgwick's "...efficient orientation filter for... inertial/magnetic sensor arrays"
// (see http://www.x-io.co.uk/category/open-source/ for examples and more details)
// which fuses acceleration, rotation rate, and magnetic moments to produce a quaternion-based estimate of absolute
// device orientation -- which can be converted to yaw, pitch, and roll. Useful for stabilizing quadcopters, etc.
// The performance of the orientation filter is at least as good as conventional Kalman-based filtering algorithms
// but is much less computationally intensive---it can be performed on a 3.3 V Pro Mini operating at 8 MHz!
void AHRS::madgwickQuaternionUpdate(float deltat, float* a, float* g, float* m)
{
	float ax = a[0], ay = a[1], az = a[2];
    // Convert gyro rate to rad/s
	float gx = g[0] * M_PI / 180.0f, gy = g[1] * M_PI / 180.0f , gz = g[2] * M_PI / 180.0f;
	float mx = m[0], my = m[1], mz = m[2];

	float q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3]; // short name local variable for readability
	float norm;
	float hx, hy, _2bx, _2bz;
	float s1, s2, s3, s4;
	float qDot1, qDot2, qDot3, qDot4;

	// Auxiliary variables to avoid repeated arithmetic
	float _2q1mx;
	float _2q1my;
	float _2q1mz;
	float _2q2mx;
	float _4bx;
	float _4bz;
	float _2q1 = 2.0f * q1;
	float _2q2 = 2.0f * q2;
	float _2q3 = 2.0f * q3;
	float _2q4 = 2.0f * q4;
	float _2q1q3 = 2.0f * q1 * q3;
	float _2q3q4 = 2.0f * q3 * q4;
	float q1q1 = q1 * q1;
	float q1q2 = q1 * q2;
	float q1q3 = q1 * q3;
	float q1q4 = q1 * q4;
	float q2q2 = q2 * q2;
	float q2q3 = q2 * q3;
	float q2q4 = q2 * q4;
	float q3q3 = q3 * q3;
	float q3q4 = q3 * q4;
	float q4q4 = q4 * q4;

	// Normalise accelerometer measurement
	norm = sqrtf(ax * ax + ay * ay + az * az);
	if (norm == 0.0f)
		return; // handle NaN
	norm = 1.0f / norm;
	ax *= norm;
	ay *= norm;
	az *= norm;

	// Normalise magnetometer measurement
	norm = sqrtf(mx * mx + my * my + mz * mz);
	if (norm == 0.0f)
		return; // handle NaN
	norm = 1.0f / norm;
	mx *= norm;
	my *= norm;
	mz *= norm;

	// Reference direction of Earth's magnetic field
	_2q1mx = 2.0f * q1 * mx;
	_2q1my = 2.0f * q1 * my;
	_2q1mz = 2.0f * q1 * mz;
	_2q2mx = 2.0f * q2 * mx;
	hx = mx * q1q1 - _2q1my * q4 + _2q1mz * q3 + mx * q2q2 + _2q2 * my * q3
			+ _2q2 * mz * q4 - mx * q3q3 - mx * q4q4;
	hy = _2q1mx * q4 + my * q1q1 - _2q1mz * q2 + _2q2mx * q3 - my * q2q2
			+ my * q3q3 + _2q3 * mz * q4 - my * q4q4;
	_2bx = sqrtf(hx * hx + hy * hy);
	_2bz = -_2q1mx * q3 + _2q1my * q2 + mz * q1q1 + _2q2mx * q4 - mz * q2q2
			+ _2q3 * my * q4 - mz * q3q3 + mz * q4q4;
	_4bx = 2.0f * _2bx;
	_4bz = 2.0f * _2bz;

	// Gradient decent algorithm corrective step
	s1 = -_2q3 * (2.0f * q2q4 - _2q1q3 - ax)
			+ _2q2 * (2.0f * q1q2 + _2q3q4 - ay)
			- _2bz * q3
					* (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx)
			+ (-_2bx * q4 + _2bz * q2)
					* (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my)
			+ _2bx * q3
					* (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
	s2 = _2q4 * (2.0f * q2q4 - _2q1q3 - ax) + _2q1 * (2.0f * q1q2 + _2q3q4 - ay)
			- 4.0f * q2 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az)
			+ _2bz * q4
					* (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx)
			+ (_2bx * q3 + _2bz * q1)
					* (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my)
			+ (_2bx * q4 - _4bz * q2)
					* (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
	s3 = -_2q1 * (2.0f * q2q4 - _2q1q3 - ax)
			+ _2q4 * (2.0f * q1q2 + _2q3q4 - ay)
			- 4.0f * q3 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az)
			+ (-_4bx * q3 - _2bz * q1)
					* (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx)
			+ (_2bx * q2 + _2bz * q4)
					* (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my)
			+ (_2bx * q1 - _4bz * q3)
					* (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
	s4 = _2q2 * (2.0f * q2q4 - _2q1q3 - ax) + _2q3 * (2.0f * q1q2 + _2q3q4 - ay)
			+ (-_4bx * q4 + _2bz * q2)
					* (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx)
			+ (-_2bx * q1 + _2bz * q3)
					* (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my)
			+ _2bx * q2
					* (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
	norm = sqrtf(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4); // normalise step magnitude
	norm = 1.0f / norm;
	s1 *= norm;
	s2 *= norm;
	s3 *= norm;
	s4 *= norm;

	// Compute rate of change of quaternion
	qDot1 = 0.5f * (-q2 * gx - q3 * gy - q4 * gz) - beta * s1;
	qDot2 = 0.5f * (q1 * gx + q3 * gz - q4 * gy) - beta * s2;
	qDot3 = 0.5f * (q1 * gy - q2 * gz + q4 * gx) - beta * s3;
	qDot4 = 0.5f * (q1 * gz + q2 * gy - q3 * gx) - beta * s4;

	// Integrate to yield quaternion
	q1 += qDot1 * deltat;
	q2 += qDot2 * deltat;
	q3 += qDot3 * deltat;
	q4 += qDot4 * deltat;
	norm = sqrtf(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4); // normalise quaternion
	norm = 1.0f / norm;
	q[0] = q1 * norm;
	q[1] = q2 * norm;
	q[2] = q3 * norm;
	q[3] = q4 * norm;

}

// Similar to Madgwick scheme but uses proportional and integral filtering on the error between estimated reference vectors and
// measured ones.
void AHRS::mahonyQuaternionUpdate(float deltat, float* a, float* g, float* m)
{
	float ax = a[0], ay = a[1], az = a[2];
    // Convert gyro rate to rad/s
	float gx = g[0] * M_PI / 180.0f, gy = g[1] * M_PI / 180.0f , gz = g[2] * M_PI / 180.0f;
	float mx = m[0], my = m[1], mz = m[2];

	float q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3]; // short name local variable for readability
	float norm;
	float hx, hy, bx, bz;
	float vx, vy, vz, wx, wy, wz;
	float ex, ey, ez;
	float pa, pb, pc;

	// Auxiliary variables to avoid repeated arithmetic
	float q1q1 = q1 * q1;
	float q1q2 = q1 * q2;
	float q1q3 = q1 * q3;
	float q1q4 = q1 * q4;
	float q2q2 = q2 * q2;
	float q2q3 = q2 * q3;
	float q2q4 = q2 * q4;
	float q3q3 = q3 * q3;
	float q3q4 = q3 * q4;
	float q4q4 = q4 * q4;

	// Normalise accelerometer measurement
	norm = sqrtf(ax * ax + ay * ay + az * az);
	if (norm == 0.0f)
		return; // handle NaN
	norm = 1.0f / norm;        // use reciprocal for division
	ax *= norm;
	ay *= norm;
	az *= norm;

	// Normalise magnetometer measurement
	norm = sqrtf(mx * mx + my * my + mz * mz);
	if (norm == 0.0f)
		return; // handle NaN
	norm = 1.0f / norm;        // use reciprocal for division
	mx *= norm;
	my *= norm;
	mz *= norm;

	// Reference direction of Earth's magnetic field
	hx = 2.0f * mx * (0.5f - q3q3 - q4q4) + 2.0f * my * (q2q3 - q1q4)
			+ 2.0f * mz * (q2q4 + q1q3);
	hy = 2.0f * mx * (q2q3 + q1q4) + 2.0f * my * (0.5f - q2q2 - q4q4)
			+ 2.0f * mz * (q3q4 - q1q2);
	bx = sqrtf((hx * hx) + (hy * hy));
	bz = 2.0f * mx * (q2q4 - q1q3) + 2.0f * my * (q3q4 + q1q2)
			+ 2.0f * mz * (0.5f - q2q2 - q3q3);

	// Estimated direction of gravity and magnetic field
	vx = 2.0f * (q2q4 - q1q3);
	vy = 2.0f * (q1q2 + q3q4);
	vz = q1q1 - q2q2 - q3q3 + q4q4;
	wx = 2.0f * bx * (0.5f - q3q3 - q4q4) + 2.0f * bz * (q2q4 - q1q3);
	wy = 2.0f * bx * (q2q3 - q1q4) + 2.0f * bz * (q1q2 + q3q4);
	wz = 2.0f * bx * (q1q3 + q2q4) + 2.0f * bz * (0.5f - q2q2 - q3q3);

	// Error is cross product between estimated direction and measured direction of gravity
	ex = (ay * vz - az * vy) + (my * wz - mz * wy);
	ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
	ez = (ax * vy - ay * vx) + (mx * wy - my * wx);
	if (Ki > 0.0f)
	{
		eInt[0] += ex;      // accumulate integral error
		eInt[1] += ey;
		eInt[2] += ez;
	}
	else
	{
		eInt[0] = 0.0f;     // prevent integral wind up
		eInt[1] = 0.0f;
		eInt[2] = 0.0f;
	}

	// Apply feedback terms
	gx = gx + Kp * ex + Ki * eInt[0];
	gy = gy + Kp * ey + Ki * eInt[1];
	gz = gz + Kp * ez + Ki * eInt[2];

	// Integrate rate of change of quaternion
	pa = q2;
	pb = q3;
	pc = q4;
	q1 = q1 + (-q2 * gx - q3 * gy - q4 * gz) * (0.5f * deltat);
	q2 = pa + (q1 * gx + pb * gz - pc * gy) * (0.5f * deltat);
	q3 = pb + (q1 * gy - pa * gz + pc * gx) * (0.5f * deltat);
	q4 = pc + (q1 * gz + pa * gy - pb * gx) * (0.5f * deltat);

	// Normalise quaternion
	norm = sqrtf(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);
	norm = 1.0f / norm;
	q[0] = q1 * norm;
	q[1] = q2 * norm;
	q[2] = q3 * norm;
	q[3] = q4 * norm;

}

