#pragma once

#include <sstream>
#include <iomanip>
#include <chrono>

class DateTimeController {
public:
	static std::string GetCurrentTimeString();
	static std::string GetCurrentDateString();
private:
};