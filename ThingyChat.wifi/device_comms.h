// File: device_comms.h
#ifndef DEVICE_COMMS_H
#define DEVICE_COMMS_H

#include <Arduino.h>

// Asks the connected Arduino for its name and waits for a response.
// Returns the device name on success, or an empty String on failure/timeout.
String getDeviceNameFromArduino(unsigned long timeoutMillis);

#endif