#ifndef MDNS_SERVICE_H
#define MDNS_SERVICE_H

#include <vector>
#include <string>

void initMDNS(const char* hostname);
std::vector<std::string> getDiscoveredDevices();

#endif