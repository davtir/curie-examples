/*
 * Author: Davide Tiriticco
 */
 
#include <CurieIMU.h>
#include <MadgwickAHRS.h>

#define CONVERT(RAW, RANGE) (RAW * RANGE / float(__INT16_MAX__))

Madgwick filter; // initialise Madgwick object
int ax, ay, az;
int gx, gy, gz;
int a_range, g_range;
float scaled_gx, scaled_gy, scaled_gz;
float scaled_ax, scaled_ay, scaled_az;
float yaw, pitch, roll;
int factor = 6;

void setup() {
  // initialize Serial communication
  Serial.begin(9600);

  // initialize device
  CurieIMU.begin();
  
  Serial.print("Starting Gyroscope calibration...");
  CurieIMU.autoCalibrateGyroOffset();
  Serial.println(" Done");
  
  Serial.print("Starting Acceleration calibration...");
  CurieIMU.autoCalibrateAccelerometerOffset(X_AXIS, 0);
  CurieIMU.autoCalibrateAccelerometerOffset(Y_AXIS, 0);
  CurieIMU.autoCalibrateAccelerometerOffset(Z_AXIS, 1);
  Serial.println(" Done");

  a_range = CurieIMU.getAccelerometerRange();
  g_range = CurieIMU.getGyroRange();
}

void loop() {
  // read raw accel/gyro measurements from device
  CurieIMU.readMotionSensor(ax, ay, az, gx, gy, gz); 

  scaled_ax = CONVERT(ax, a_range);
  scaled_ay = CONVERT(ay, a_range);
  scaled_az = CONVERT(az, a_range);
  scaled_gx = CONVERT(gx, g_range);
  scaled_gy = CONVERT(gy, g_range);
  scaled_gz = CONVERT(gz, g_range);
  
  // use function from MagdwickAHRS.h to return quaternions
  filter.updateIMU(scaled_gx / factor, scaled_gy / factor, scaled_gz / factor, scaled_ax, scaled_ay, scaled_az);

  /*
  Serial.print(scaled_ax); Serial.print("\t");
  Serial.print(scaled_ay); Serial.print("\t");
  Serial.print(scaled_az); Serial.print("\t");
  Serial.print(scaled_gx); Serial.print("\t");
  Serial.print(scaled_gy); Serial.print("\t");
  Serial.print(scaled_gz); Serial.print("\t");
  Serial.println("");
  */


  // functions to find yaw roll and pitch from quaternions
  yaw = filter.getYaw();
  roll = filter.getRoll();
  pitch = filter.getPitch();
  
  // print gyro and accel values for debugging only, comment out when running Processing
  if (Serial.available() > 0) {
    int val = Serial.read();
    if (val == 's') { // if incoming serial is "s"
      Serial.print(yaw);    // Rotation on the Z axis
      Serial.print(","); // print comma so values can be parsed
      Serial.print(pitch);  // Rotation on the Y Axis
      Serial.print(","); // print comma so values can be parsed
      Serial.println(roll); // Rotation on the X Axis
    }
  }
}
