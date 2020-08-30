#include "Link.h"

// Link Constructor
Link::Link ( std::string arg_url, std::string arg_class_id, std::string arg_object_id)
{
    url = arg_url;
    class_id = arg_class_id;
    object_id = arg_object_id;
}

// copy all the information in this link object into Json::Value and return a pointer that points to it
Json::Value * Link::dumpJ (void)
{
    Json::Value * result_ptr = new Json::Value;

    (*result_ptr)["url"] = this->url;
    (*result_ptr)["class_id"] = this->class_id;
    (*result_ptr)["object_id"] = this->object_id;

    return result_ptr;
}