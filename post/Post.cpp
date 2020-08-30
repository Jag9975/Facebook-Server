#include "Post.h"

// Post constructor 
Post::Post (Person* arg_author, Message* arg_msg)
{
    author = arg_author;
    msg = arg_msg;

    author = NULL;
    receivers = NULL;
    msg = NULL;
    links = NULL;
    actions = NULL;
    reactions = NULL;
    comments = NULL;
    keys = NULL;
    created = NULL;
    updated = NULL;
    is_published = false;
    
}


// place all the information from the post object into Json::Value object and return a pointer that points to it
Json::Value * Post::dumpJ (void)
{
    Json::Value *result_ptr = new Json::Value();

    // "id"
    (*result_ptr)["id"] = (this->profile_id + "_" + this->post_id);

    // "from"
    if (this->author != NULL)
    {
      (*result_ptr)["from"] = (*(this->author)->dumpJ());
    }

    // "to"
    Json::Value json_c_receivers;       // stores pointers to an array of data objects
    Json::Value json_c_receivers_array; // stores pointer to json_c_receivers and the value of count

    if ((this->receivers != NULL) && (((this->receivers)->size() > 0)))
    {
      for (int i = 0; i < (this->receivers)->size(); i++)
	    {
        Json::Value * dumpjv_ptr = ((*(this->receivers))[i])->dumpJ();
        json_c_receivers[i] = (*dumpjv_ptr);
        delete dumpjv_ptr;
	    }
      json_c_receivers_array["data"] = json_c_receivers;
      json_c_receivers_array["count"] = ((int) (*(this->receivers)).size());
      (*result_ptr)["to"] = json_c_receivers_array;
    }

    // "message"
    if (this->msg != NULL)
    {
      (*result_ptr)["message"] = (this->msg)->content;
    }

    // "links"
    Json::Value json_c_links;           // stores pointers to an array of data objects
    Json::Value json_c_links_array;     // stores pointer to json_c_links and the value of count

    if ((this->links != NULL) && (((this->links)->size() > 0)))
    {
      for (int i = 0; i < (this->links)->size(); i++)
	    {
        Json::Value * dumpjv_ptr = ((*(this->links))[i])->dumpJ();
        json_c_links[i] = (*dumpjv_ptr);
        delete dumpjv_ptr;
	    }
      json_c_links_array["data"] = json_c_links;
      json_c_links_array["count"] = ((int) (*(this->links)).size());
      (*result_ptr)["links"] = json_c_links_array;
    }

    // "action"
    Json::Value json_c_actions;         // stores pointers to an array of data objects
    Json::Value json_c_actions_array;   // stores pointer to json_c_actions and the value of count

    if ((this->actions != NULL) && ((this->actions)->size() > 0))
    {
      for (int i = 0; i < (this->actions)->size(); i++)
	    {
        Json::Value * dumpjv_ptr = ((*(this->actions))[i])->dumpJ();
        json_c_actions[i] = (*dumpjv_ptr);
        delete dumpjv_ptr;
	    }
      json_c_actions_array["data"] = json_c_actions;
      json_c_actions_array["count"] = ((int) (*(this->actions)).size());
      (*result_ptr)["actions"] = json_c_actions_array;
    }

    // "created_time"
    if (this->created != NULL)
    {
      (*result_ptr)["created_time"] = *((this->created)->getTimeString());
    }

    // "updated_time"
    if (this->updated != NULL)
    {
      (*result_ptr)["updated_time"] = *((this->updated)->getTimeString());
    }

    // "reactions"
    Json::Value json_c_reactions;           // stores pointers to an array of data objects
    Json::Value json_c_reactions_array;     // stores pointer to json_c_reactions and the value of count

    if ((this->reactions != NULL) && (((this->reactions)->size() > 0)))
    {
      for (int i = 0; i < (this->reactions)->size(); i++)
	    {
        Json::Value * dumpjv_ptr = ((*(this->reactions))[i])->dumpJ();
        json_c_reactions[i] = (*dumpjv_ptr);
        delete dumpjv_ptr;
	    }
      json_c_reactions_array["data"] = json_c_reactions;
      json_c_reactions_array["count"] = ((int) (*(this->reactions)).size());
      (*result_ptr)["reactions"] = json_c_reactions_array;
    }
    
    // "comments"
    Json::Value json_c_comments;        // stores pointers to an array of data objects
    Json::Value json_c_comments_array;  // stores pointer to json_c_comments and the value of count

    if ((this->comments != NULL) && (((this->comments)->size() > 0)))
    {
      for (int i = 0; i < (this->comments)->size(); i++)
	    {
        Json::Value * dumpjv_ptr = ((*(this->comments))[i])->dumpJ();
        json_c_comments[i] = (*dumpjv_ptr);
        delete dumpjv_ptr;
	    }
      json_c_comments_array["data"] = json_c_comments;
      json_c_comments_array["count"] = ((int) (*(this->comments)).size());
      (*result_ptr)["comments"] = json_c_comments_array;
    }

    // OKey
    Json::Value keys_arr;
    Json::Value keys_full;
    if ((this->keys != NULL) && (((this->keys)->size() > 0)))
    {
      for (int i = 0; i < (this->keys)->size(); i++)
	    {
        keys_arr[i] = (*keys)[i]->okey;
	    }
      keys_full["data"] = keys_arr;
      keys_full["count"] = ((int) (*(this->keys)).size());
      (*result_ptr)["keys"] = keys_full;
    }
        
    // "is_published"
    (*result_ptr)["is_published"] = is_published;

    return result_ptr;

}