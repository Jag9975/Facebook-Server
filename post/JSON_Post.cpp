#include <arpa/inet.h>
#include "Post.h"

// keeps track of the keys in posts
std::map<std::string, OKey *> All_Keys;

int Json_Post_Common (Json::Value myv, Post * my_post_ptr);

// returns a all the posts that match the key 
std::vector<Post *> *
getPostbyKey
(std::string key)
{
  if (All_Keys.find(key) == All_Keys.end())
    return NULL;

  OKey *okey_ptr = All_Keys[key];
  return okey_ptr->posts;
}

// return Okey object pointer for the key if it already exists else create a new one
OKey *
getOKey
(std::string key)
{
  if (All_Keys.find(key) != All_Keys.end())
    return All_Keys[key];

  OKey *new_okey_ptr = new OKey(key);
  new_okey_ptr->posts = new std::vector<Post *>();
  All_Keys[key] = new_okey_ptr;
  return new_okey_ptr;
}

// returns post ID
std::string
Json_2_PostID
(Json::Value myv)
{
  return ((myv["id"]).asString());
}

// converts json into a Post object
Post *
Json_2_Post
(Json::Value myv)
{
  if (myv.isArray() == true)
    {
      return NULL;
    }

  Post * my_post_ptr = new Post((Person *) NULL, (Message *) NULL);
  bzero(my_post_ptr, sizeof(class Post));

  // Retrive the ID and store it in the empty post object my_post_ptr
  if ((myv["id"]).isNull() != true)
  {
    char idstr[1024];
    bzero(idstr, 1024);
    sprintf(idstr, "%s", ((myv["id"]).asString()).c_str());
    if (idstr[0] != '\0')
    {
	    char c_prof_buf[256];
	    char c_post_buf[256];
	    bzero(c_prof_buf, 256);
	    bzero(c_post_buf, 256);

	    sscanf(idstr, "%[^_]_%s", c_prof_buf, c_post_buf);

	    std::string profile_id = { c_prof_buf };
	    std::string post_id = { c_post_buf };

	    // put id attributes to the Post object
	    my_post_ptr->profile_id = profile_id;
	    my_post_ptr->post_id = post_id;
	  }
    else
	  {
	    std::cerr << "No Post ID presented" << std::endl;
	    exit(-1);
	  }
  }
  else
  {
    std::cerr << "No Post ID presented" << std::endl;
    exit(-2);
  }
  
  // converts the json into a post object
  int rc = Json_Post_Common(myv, my_post_ptr);
  if (rc != 0)
  {
    std::cerr << "Error Json_Post_Common " << rc << std::endl;
    exit(-3);
  }

  // pointer to the post object
  return my_post_ptr;
}

// merge the Post object and json if their ID's match
int
Json_Post_Merging
(Json::Value myv, Post * my_post_ptr)
{
  char idstr[1024];
  bzero(idstr, 1024);

  if ((myv).isArray() == true) return -3;
  if ((myv["id"]).isNull() == true) return -1;

  // compare ID's to check if they match
  sprintf(idstr, "%s", ((myv["id"]).asString()).c_str());

  if (idstr[0] != '\0')
  {
    char c_prof_buf[256];
    char c_post_buf[256];
    bzero(c_prof_buf, 256);
    bzero(c_post_buf, 256);

    sscanf(idstr, "%[^_]_%s", c_prof_buf, c_post_buf);

    std::string profile_id = { c_prof_buf };
    std::string post_id = { c_post_buf };

    if ((my_post_ptr->profile_id != profile_id) || (my_post_ptr->post_id    != post_id))
	  {	  
	    std::cerr << "Post IDs mismatched!!" << std::endl;
	    return -1;
	  }
  }
  else
  {
    std::cerr << "No Post ID presented for the second JSON" << std::endl;
    return -2;
  }

  // they matched, proceed with the merge action
  return Json_Post_Common(myv, my_post_ptr);
}

// merge the content of myv with the post object my_post_ptr
// merged content is stored my_post_ptr
int
Json_Post_Common
(Json::Value myv, Post * my_post_ptr)
{
  // under Json_Post_Common, we assume that the ID already matched for both Post and myv

  // merge the actions section
  if (((myv["actions"]).isNull() != true) && ((myv["actions"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->actions == NULL)
    {
      my_post_ptr->actions = new vector<Action *>();
    }
	      
    for ( int i = 0; i < (myv["actions"]["count"]).asInt(); i++)
	  {
	    Json::Value l_jv = myv["actions"]["data"][i];

      // Merge action if it's valid and doesn't exist in the previous post
      if (((l_jv["owner_vsID"]).isNull() != true) && ((l_jv["class_id"]).isNull() != true) &&
	        ((l_jv["object_id"]).isNull() != true) && ((l_jv["name"]).isNull() != true))
      {
        // set merge to 0 if action is present in previous post
        int merge = 1;   
         
        for (int t = 0; t < (*(my_post_ptr->actions)).size(); t++)
        {
          if ((*((*(my_post_ptr->actions))[t])).name == (l_jv["name"]).asString() &&
            (*((*(my_post_ptr->actions))[t])).object_id == (l_jv["object_id"]).asString())
          {
            merge = 0;
            break;
          }  
        }
        
        // merge the action if it doesn't exist
        if (merge == 1)
        {
          Action *action_ptr = new Action((l_jv["name"]).asString(), (l_jv["host_url"]).asString(),
                              (l_jv["owner_vsID"]).asString(), (l_jv["class_id"]).asString(), 
                              (l_jv["object_id"]).asString());
        
          (*(my_post_ptr->actions)).push_back(action_ptr);
        }
      }
	  }
  }

  // merge the comments
  if (((myv["comments"]).isNull() != true) && ((myv["comments"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->comments == NULL)
    {
      my_post_ptr->comments = new vector<Comment *>();
    }
	      
    Comment * comment_ptr;
    vector<Comment *> vec_comments;
    vector<Reaction *> * vec_c_reactions_ptr;
    for ( int i = 0; i < (myv["comments"]["count"]).asInt(); i++)
	  {
	    Json::Value l_jv = myv["comments"]["data"][i];

      // checks if it's a valid comment
      if (((l_jv["id"]).isNull() != true) && ((l_jv["from"]["avatar_name"]).isNull() != true) &&
	        ((l_jv["from"]["vsID"]).isNull() != true) && ((l_jv["created_time"]).isNull() != true) &&
	        ((l_jv["message"]).isNull() != true))
      {
        // intialize and set profile, post, and comment ids
        char profile_id[40];
        bzero(profile_id, 40);
        char post_id[16];
        bzero(profile_id, 16);
        char comment_id[16];
        bzero(profile_id, 16);
        sscanf(((l_jv["id"]).asString()).c_str(),"%[^_]_%[^_]_%s", profile_id, post_id, comment_id);

        // set merge to 0 if action is present in previous post
        int merge = 1;   
         
        for (int t = 0; t < (*(my_post_ptr->comments)).size(); t++)
        {
          if ((*((*(my_post_ptr->comments))[t])).post_id == post_id &&
            (*((*(my_post_ptr->comments))[t])).comment_id == comment_id)
          {
            merge = 0;
            break;
          }  
        }
        
        // merge the action if it doesn't exist
        if (merge == 1)
        {
          // set author
          Person *author_ptr = new Person((l_jv["from"]["avatar_name"]).asString(), (l_jv["from"]["vsID"]).asString());

          // set Jvtime
          JvTime *jvtime_ptr = new JvTime(((l_jv["created_time"]).asString()).c_str());

          // set message
          Message *msg_ptr = new Message(l_jv["message"].asString());
            
          // assign the pointers 
          comment_ptr = new Comment(profile_id, post_id, comment_id, author_ptr, msg_ptr, jvtime_ptr);

          // append it to comments vector
          (*(my_post_ptr->comments)).push_back(comment_ptr);
        }
      }
    }
  }

  // merge the keys
  if (((myv["keys"]).isNull() != true) && ((myv["keys"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->keys == NULL)
    {
      my_post_ptr->keys = new vector<OKey *>;
    }

    for (int i = 0; i < (myv["keys"]["count"]).asInt(); i++)
    {
      Json::Value l_jv = myv["keys"]["data"][i];
      if (l_jv.isNull() != true)
      {
        OKey * okey_ptr = getOKey(l_jv.asString());
        (*(okey_ptr->posts)).push_back(my_post_ptr);
        (*(my_post_ptr->keys)).push_back(okey_ptr);
      }
    }    
  }

  // merge the links
  if (((myv["links"]).isNull() != true) && ((myv["links"]["count"]).asInt() > 0))
  {   
    if (my_post_ptr->links == NULL)
    {
      my_post_ptr->links = new vector<Link *>();
    }
    // add all the links
    for (int i = 0; i < myv["links"]["count"].asInt(); i++)
    {
      Json::Value l_jv = myv["links"]["data"][i];

      if (((l_jv["url"]).isNull() != true) && ((l_jv["class_id"]).isNull() != true) && ((l_jv["object_id"]).isNull() != true))
      {
        // set merge to 0 if action is present in previous post
        int merge = 1;   
          
        for (int t = 0; t < (*(my_post_ptr->links)).size(); t++)
        {
          if ((*((*(my_post_ptr->links))[t])).object_id == (l_jv["object_id"]).asString())
          {
            merge = 0;
            break;
          }  
        }
          
        // merge the action if it doesn't exist
        if (merge == 1)
        {
          Link * link_ptr = new Link((l_jv["url"]).asString(),
          (l_jv["class_id"]).asString(), 
          (l_jv["object_id"]).asString());

          (*(my_post_ptr->links)).push_back(link_ptr);
        }
      }
    }
  }

  // "message"
  if ((myv["message"]).isNull() != true)
  {
    if (my_post_ptr->msg == NULL) 
	  {
      my_post_ptr->msg = new Message(myv["message"].asString());
    }
  }

  // merge the reactions
  if (((myv["reactions"]).isNull() != true) && ((myv["reactions"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->reactions == NULL)
    {
      my_post_ptr->reactions = new vector<Reaction *>();
    }
    
    for ( int i = 0; i < (myv["reactions"]["count"]).asInt(); i++)
	  {
	    Json::Value l_jv = myv["reactions"]["data"][i];

	    if (((l_jv["avatar_name"]).isNull() != true) && ((l_jv["vsID"]).isNull() != true) &&
	       ((l_jv["created_time"]).isNull() != true) && ((l_jv["type"]).isNull() != true))
	    {
        // set merge to 0 if action is present in previous post
        int merge = 1;   
          
        for (int t = 0; t < (*(my_post_ptr->reactions)).size(); t++)
        {
          if ((*(*((*(my_post_ptr->reactions))[t])).author).vsID == (l_jv["vsID"]).asString() &&
            (*((*(my_post_ptr->reactions))[t])).type == (l_jv["type"]).asString())
          {
            merge = 0;
            break;
          }  
        }
          
        // merge the action if it doesn't exist
        if (merge == 1)
        {
          Person *react_p_ptr = new Person((l_jv["avatar_name"]).asString(), (l_jv["vsID"]).asString()); 

	        JvTime *react_cr_time_ptr = new JvTime(((l_jv["created_time"]).asString()).c_str());

	        Reaction * react_ptr = new Reaction(l_jv["type"].asString(), react_p_ptr, react_cr_time_ptr);

	        (*(my_post_ptr->reactions)).push_back(react_ptr);
        }

	    }
	  }
  }

  // "from"
  if ((myv["from"]).isNull() != true)
  {
    if (my_post_ptr->author == NULL) //might need to change
	  {
      my_post_ptr->author = new Person((myv["from"]["avatar_name"]).asString(), (myv["from"]["vsID"]).asString());
    }
  }

  // "to"
    if (((myv["to"]).isNull() != true) && ((myv["to"]["count"]).asInt() > 0))
    {
      if (my_post_ptr->receivers == NULL)
      {
        my_post_ptr->receivers = new vector<Person *>();
      }
	
      for ( int i = 0; i < (myv["to"]["count"]).asInt(); i++)
	    {
	      Json::Value l_jv = myv["to"]["data"][i];

        // set merge to 0 if action is present in previous post
        int merge = 1;   
          
        for (int t = 0; t < (*(my_post_ptr->receivers)).size(); t++)
        {
          if ((*((*(my_post_ptr->receivers))[t])).avatar_name == (l_jv["avatar_name"]).asString() &&
            (*((*(my_post_ptr->receivers))[t])).vsID == (l_jv["vsID"]).asString())
          {
            merge = 0;
            break;
          }  
        }
          
        // merge the action if it doesn't exist
        if (merge == 1)
        {
	        Person * to_ptr = new Person((l_jv["avatar_name"]).asString(), (l_jv["vsID"]).asString());

	        (*(my_post_ptr->receivers)).push_back(to_ptr);
        }
	    }
    }

  // "created_time"
  if ((myv["created_time"]).isNull() != true)
  {
    my_post_ptr->created = new JvTime(((myv["created_time"]).asString()).c_str());
  }

  // "updated_time"
  if ((myv["updated_time"]).isNull() != true)
  {
    my_post_ptr->updated = new JvTime(((myv["updated_time"]).asString()).c_str());
  }

  // "is_published"
  my_post_ptr->is_published = (myv["is_published"]).asBool();

  return 0;
  
}
