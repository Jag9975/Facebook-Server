#ifndef _TAG_H_
#define _TAG_H_

#include "Core.h"

class Tag
{
 private:
 protected:
 public:
  int index;
  std::string avatar_name;
  std::string vsID;
  std::string comment;

  Tag();
  Json::Value * dumpJ();
};

#endif 
