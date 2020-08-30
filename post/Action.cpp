#include "Action.h"

// Action Constructor
Action::Action (std::string arg_name, std::string arg_core_host_url, std::string arg_core_owner_vsID,
                std::string arg_core_class_id, std::string arg_core_object_id)
                : Core {arg_core_host_url, arg_core_owner_vsID, arg_core_class_id, arg_core_object_id}
{
    name = arg_name;
}

// loads the information stored in Action object into Json::Value
Json::Value * Action::dumpJ ( void )
{

    // place all the information into a Json::Value object and return a pointer that points to it

    Json::Value *result_ptr = new Json::Value();

    (*result_ptr)["name"] = name;
    (*result_ptr)["host_url"] = host_url;
    (*result_ptr)["owner_vsID"] = owner_vsID;
    (*result_ptr)["class_id"] = class_id;
    (*result_ptr)["object_id"] = object_id;

    return result_ptr;
}
