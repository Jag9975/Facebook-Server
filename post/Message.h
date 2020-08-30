#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "Core.h"

// stores the message provided in the posts
class Message
{
 private:
 protected:
 public:
  std::string content;
  Message(std::string arg_s) { content = arg_s; }
  void setContent(std::string arg_s) { content = arg_s; }
  std::string getContent(void) { return content; }
};

#endif 
