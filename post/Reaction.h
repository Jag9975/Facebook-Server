#ifndef _REACTION_H_
#define _REACTION_H_

#include "Core.h"
#include "Person.h"
#include "JvTime.h"

// stores the reactions provided in the posts
class Reaction
{
 private:
 protected:
 public:
  std::string type;
  Person * author;
  JvTime * created;
  Reaction(std::string arg_type, Person * arg_author, JvTime * arg_created);
  Json::Value * dumpJ();
};

#endif 
