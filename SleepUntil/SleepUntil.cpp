// SleepMS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <stdexcept>
#include <Windows.h>

// TODO: Make it so that the argument can be a commaseparated list of numbers 0->59.
// Then it will sleep until the first of those intervals is reached.
int main(int argc, char *argv[])
{
	if (argc < 2) {
		std::string appName = argv[0];
		std::cout << "Usage:" << std::endl;
		std::cout << appName << " [seconds]" << std::endl;
		std::cout << "Default for seconds is 0, meaning the app will sleep until seconds equals 0." << std::endl;
		std::cout << "If f.ex the app was started with \"" << appName << " 5\" at 16:01:27.512, it would sleep for 37 seconds and 488 milliseconds until 16:02:05.000" << std::endl;
		std::cout << "This enables more finetuned timing when used with scheduling tools like Windows Scheduled Tasks, that inherently only supports 1 minute as the smallest scheduling interval" << std::endl;
		exit(0);
	}

	std::string arg = argv[1];

	SYSTEMTIME stNow;
	GetSystemTime(&stNow);
	FILETIME ftNow;
	SystemTimeToFileTime(&stNow, &ftNow);

	int sNow = stNow.wSecond;
	int mNow = stNow.wMilliseconds;

	try {
		std::size_t pos;
		int x = std::stoi(arg, &pos);
		int tsm = (x * 1000 + 60000 - sNow * 1000 - mNow) % 60000; 
		
		//Attempting to sleep for tsm milliseconds
		if (pos < arg.size()) {
			std::cerr << "Trailing characters after number: " << arg << std::endl;
		}
		else std::this_thread::sleep_for(std::chrono::milliseconds(tsm));
	}
	catch (std::invalid_argument const &ex) {
		std::cerr << "Invalid number: " << arg << std::endl;
	}
	catch (std::out_of_range const &ex) {
		std::cerr << "Number out of range: " << arg << std::endl;
	}
}


