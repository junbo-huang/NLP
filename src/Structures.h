#pragma once
#include <time.h>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>


#ifndef DEFAULT_OUTPATH
#define DEFAULT_OUTPATH "../output/"
#endif


//defined structures
struct report {
	std::string message_type;
	std::string report_time;
	std::string sensor_id;
};


struct Info {
	std::string infoType;
	std::string info;
};

struct Message {
	struct report reportInfo;
	std::vector<Info> eventInfos;
};

typedef std::vector<Message> Messages;

struct variation {
	unsigned int possibility;
	std::string content;
};

struct messageType {
	bool isVector;
	std::vector<variation> variations;
};
struct templateInfo {
	std::map<std::string, std::string> templateAttr;
	std::map<std::string, messageType> Messagelibrary;
};

//struct structTag2 {
//
//	std::string location;
//	std::string start_time;
//	std::vector<std::string> brand;
//	std::vector<std::string> color;
//	std::vector<std::string> plateNumber;
//	std::vector<std::string> speed;
//	unsigned int totalNumberOfPassengers;
//	unsigned int totalPeopleNumberInAccident;
//	unsigned int carNumberInAccident;
//	std::vector<unsigned int> numberOfPassengers;
//
//	bool isBrandKnown = false;
//	bool isColorKnown = false;
//	bool isPlateNumKnown = false;
//	bool isNumOfPaxsKnown = false;
//	bool isTotNumOfPaxsKnown = false;
//	bool isTotPplNumInAccKnown = false;
//	bool isCarNumInAccKnown = false;
//	bool isLocationKnown = false;
//	bool isSpeedKnown = false;
//	bool isStartTimeKnown = false;
//};
//
//
//struct structTag3 {
//
//	std::string location;
//	std::string color;
//	std::string start_time;
//	std::vector<std::string> suitNumber;
//	unsigned int buildingNumber;
//	unsigned int buildingNumberInAccident;
//	std::vector<unsigned int> numberOfDeath;
//
//	bool isLocationKnown = false;
//	bool isColorKnown = false;
//	bool isBuildingNumKnown = false;
//	bool isBuildingNumInAccKnown = false;
//	bool isNumOfDeathKnown = false;
//	bool isSuitNumKnown = false;
//	bool isStartTimeKnown = false;
//};


