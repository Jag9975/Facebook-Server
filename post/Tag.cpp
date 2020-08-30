#include "Tag.h"

// empty Tag constructor
Tag::Tag (void) 
{

}

// place all the information in Json::Value object and return a pointer that points to it
Json::Value * Tag::dumpJ (void)
{
    Json::Value * result_ptr = new Json::Value;

    // add avatar_name to the Json if it was provided
    if (avatar_name != "")
    {
        (*result_ptr)["avatar_name"] = avatar_name;
    }
    
    // add comment to the Json if it was provided
    if (comment != "")
    {
        (*result_ptr)["comment"] = comment;
    }
    
    // add index to the Json if it was provided
    if (index != 0)
    {
        (*result_ptr)["index"] = index;
    }
    
    // add vsID to the Json if it was provided
    if (vsID != "")
    {
        (*result_ptr)["vsID"] = vsID;
    }

    return result_ptr;
}