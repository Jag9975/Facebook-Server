#ifndef _JVTIME_H_
#define _JVTIME_H_

// for Json::value
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>

#include <strings.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <exception> 
#include <time.h>

using namespace std;

// stores the time provided in the posts
class JvTime
{
 private:
 protected:
 public:
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  char tail4[16];

  JvTime(const char *);
  struct std::tm * getStdTM(void);
  std::string * getTimeString(void);
};

#endif 
