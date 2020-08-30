#ifndef _PERSON_H_
#define _PERSON_H_

#include "Core.h"

// stores the information of people in the posts
class Person : public Core
{
 private:
 public:
  static unsigned int person_count;
  std::string vsID;
  std::string avatar_name;
  std::string hometown;
  Person(std::string, std::string, std::string, std::string);
  Person();
  Person(std::string, std::string, std::string, std::string, std::string, std::string);
  Person(std::string, std::string);
  virtual bool operator==(Person& aPerson);
  Json::Value * dumpJ();
};

#endif 
