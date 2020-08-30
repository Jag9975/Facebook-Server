#ifndef _OKEY_H_
#define _OKEY_H_

#include "Core.h"
#include "Post.h"

class Post;

// stores the keys provided in the posts
class OKey
{
 private:
 protected:
 public:
  std::string okey;
  std::vector<Post *> * posts;
  OKey(std::string arg_okey) { okey = arg_okey; }
};

#endif 
