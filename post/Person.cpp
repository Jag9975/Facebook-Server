#include "Person.h"

unsigned int Person::person_count { };

Person::Person(std::string core_arg_host_url, std::string core_arg_owner_vsID,
	       std::string core_arg_class_id, std::string core_arg_object_id)
  : Core { core_arg_host_url, core_arg_owner_vsID, core_arg_class_id, core_arg_object_id }
{
  person_count++;
  std::cout << "Core Constructor" << std::endl;
}

Person::Person()
{
}

Person::Person(std::string core_arg_host_url, std::string core_arg_owner_vsID,
	       std::string core_arg_class_id, std::string core_arg_object_id,
	       std::string arg_avatar_name, std::string arg_vsID)
  : Core { core_arg_host_url, core_arg_owner_vsID, core_arg_class_id, core_arg_object_id }
{
  this->avatar_name = arg_avatar_name;
  this->vsID = arg_vsID;
}

Person::Person
(std::string arg_avatar_name, std::string arg_vsID)
{
  this->avatar_name = arg_avatar_name;
  this->vsID = arg_vsID;
}

bool
Person::operator==
(Person& aPerson)
{
  return (this->vsID == aPerson.vsID && 
	  this->avatar_name == aPerson.avatar_name);
}

Json::Value * Person::dumpJ
(void)
{
  Json::Value *result_ptr = new Json::Value();
  (*result_ptr)["avatar_name"] = this->avatar_name;
  (*result_ptr)["vsID"] = this->vsID;
  return result_ptr;
}

