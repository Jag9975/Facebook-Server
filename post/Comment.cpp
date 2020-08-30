#include "Comment.h"

// Comment Constructor
Comment::Comment
(std::string arg_profile_id, std::string arg_post_id, std::string arg_comment_id,
 Person * arg_author, Message * arg_msg, JvTime * arg_created)
{
    profile_id = arg_profile_id;
    post_id = arg_post_id;
    comment_id = arg_comment_id;
    author = arg_author;
    msg = arg_msg;
    created = arg_created;

    reactions = NULL;
    tags = NULL;
}

// store all the information stored in Comment object into Json::Value object and return a pointer to it
Json::Value * Comment::dumpJ
(void)
{

    Json::Value *result_ptr = new Json::Value();

    std::string l_id = this->profile_id + "_" + this->post_id + "_" + this->comment_id;

    (*result_ptr)["id"] = l_id;
    (*result_ptr)["from"] = (*(this->author)->dumpJ());
    (*result_ptr)["message"] = (this->msg)->content;
    (*result_ptr)["created_time"] = *((this->created)->getTimeString());
    
    return result_ptr;

}
