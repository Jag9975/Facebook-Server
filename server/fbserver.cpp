// for Json::value
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <string.h>

// for JsonRPCCPP
#include <iostream>
#include "fbserver.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdio.h>

#include "Post.h"

// keeps tracks of post objects in memory
std::map<std::string, Post *> Post_Map;

using namespace jsonrpc;
using namespace std;

class MyfbServer : public fbServer
{
public:
  MyfbServer(AbstractServerConnector &connector, serverVersion_t type);
  virtual Json::Value update(const std::string& updating_json);
  virtual Json::Value search(const std::string& search_clause);
};

MyfbServer::MyfbServer(AbstractServerConnector &connector, serverVersion_t type)
  : fbServer(connector, type)
{
  std::cout << "MyfbServer Object created" << std::endl;
}

// update the post if it exists or save it as a new file 
// the post is also converted into a post object and it's pointer in saved in Post_Map
Json::Value
MyfbServer::update(const std::string& updating_json)
{
  Json::Value result_json;
  char fname_buf[1024];
  FILE * post_f_ptr;

  int rc = 0;

  std::cout << Post_Map.size() << std::endl;

  Post * post_ptr = NULL;

  Json::CharReaderBuilder builder;
  Json::CharReader* reader;
  std::string errors;
  bool parsingSuccessful;

  // save the post received in myv_message
  Json::Value myv_message;
  reader = builder.newCharReader();
  parsingSuccessful = reader->parse(updating_json.c_str(),
				    updating_json.c_str() + updating_json.size(),
				    &myv_message, &errors);
  delete reader; 

  if (!parsingSuccessful) {
    std::cout << "Failed to parse the content of the first JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
  }
   
  // check for ID
  if ((myv_message["id"]).isNull() == true)
  {
    std::cerr << "id field missing\n";
    exit(-1);
  }

  bzero(fname_buf, 1024);
  sprintf(fname_buf, "post_%s.json", ((myv_message["id"]).asString()).c_str());

  long lSize;
  size_t lresult;
  
  // if a post object exists with the same id, merge the new post with it
  if (Post_Map.find((myv_message["id"]).asString()) != Post_Map.end())
  {
    rc = 0;
    post_ptr = Post_Map[(myv_message["id"]).asString()];
    rc = Json_Post_Merging(myv_message, post_ptr);
  }
  else
  {
    rc = 0;
    post_f_ptr = fopen(fname_buf, "r");

    // check if a file is in memory with the same ID, if it exists: open the file and convert it into a post object
    if (post_f_ptr != NULL)
    {
      // get file size
      fseek(post_f_ptr, 0, SEEK_END);
      lSize = ftell(post_f_ptr);        // store the file size into lSize
      rewind(post_f_ptr);

      // allocate memory to store store the file
      char *pjf_ptr = (char *) malloc(sizeof(char)*lSize);
      // copy file into buffer
      lresult = fread(pjf_ptr, 1, lSize, post_f_ptr);
      if(lresult != lSize)
      {
        fputs("Reading error", stderr);
        exit (-3);
      }

      Json::Value pjf_v;
      reader = builder.newCharReader();
      parsingSuccessful = reader->parse(pjf_ptr, pjf_ptr + lSize, &pjf_v, &errors);

      delete reader;
      if (!parsingSuccessful)
      {
        std::cout << "Failed to parse the content of the first JSON, errors:" << std::endl;
        std::cout << errors << std::endl;
      }
      
      //convert the file into a post object
      post_ptr = Json_2_Post(pjf_v);
      if (post_ptr == NULL)
      {
        rc = -1;
      } else
      {
        Post_Map[(myv_message["id"]).asString()] = post_ptr;
        rc = Json_Post_Merging( myv_message, post_ptr);
      }
        
      fclose(post_f_ptr);
    } 
    else      // convert the post into a new post object
    {
      post_ptr = Json_2_Post(myv_message);
      if (post_ptr == NULL)
      {
        rc = -1;
      } else
      {
        Post_Map[(myv_message["id"]).asString()] = post_ptr;
      }
    }  
  }

  if (rc < 0)
  {
    std::string strJson;
    strJson += "{\"status\" : \"failed\"}";
    reader = builder.newCharReader();
    parsingSuccessful = reader->parse
	  (strJson.c_str(), strJson.c_str() + strJson.size(), &result_json, &errors);
    delete reader;

    if (!parsingSuccessful) {
	    std::cout << "Failed to parse the JSON, errors:" << std::endl;
      std::cout << errors << std::endl;
    }
    return result_json;
  }

  // write it back -- Persistent Object
  post_f_ptr = fopen(fname_buf, "w");
  if (post_f_ptr != NULL)
  {
    char post_json_buf[8192]; // limited size
    bzero(post_json_buf, 8192);
    sprintf(post_json_buf, "%s", ((*(post_ptr->dumpJ())).toStyledString()).c_str());
    lSize = strlen(post_json_buf);
    std::cout << fname_buf << " " << lSize <<  std::endl;
    lresult = fwrite(post_json_buf, 1, lSize, post_f_ptr);    
    if (lresult != lSize)
	  {
	    fputs("Reading error", stderr);
	    exit (-3);
	  }
    fclose(post_f_ptr);
  }

  std::cout << Post_Map.size() << std::endl;
  return (*(post_ptr->dumpJ()));
}

// returns all the posts that include the requested keyword
Json::Value
MyfbServer::search
(const std::string& search_clause)
{
  Json::Value result_json;
  int rc = 0;           // rc is set to a negative number if it fails any condition

  Json::CharReaderBuilder builder;
  Json::CharReader* reader;
  std::string errors;
  bool parsingSuccessful;

  // read the string and save it in myv_message
  Json::Value myv_message;
  reader = builder.newCharReader();
  parsingSuccessful = reader->parse(search_clause.c_str(),
				    search_clause.c_str() + search_clause.size(),
				    &myv_message, &errors);
  delete reader;

  if (!parsingSuccessful) {
    std::cout << "Failed to parse the content of the first JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
  }

  Json::Value json_data;
  // if no keyword is entered, set rc to a negative number
  if ((myv_message["keyword"]).isNull() == true)
  {
    rc = -1;
  }
  else   // return all the posts that match the key
  {
    std::vector<Post *> *pv_ptr = getPostbyKey((myv_message["keyword"]).asString());
    // set rc to -2 if no matching keyword is found
    if (pv_ptr == NULL)
    {
      rc = -2;
    }else
    {
      Json::Value *dumpjv_ptr;
      for (int i = 0; i < pv_ptr->size(); i++)
      {
        dumpjv_ptr = ((*pv_ptr)[i])->dumpJ();
        json_data[i] = (*dumpjv_ptr);
        delete dumpjv_ptr;
      }
      result_json["data"] = json_data;
      result_json["count"] = ((int) pv_ptr->size());
    }  
  }

  // returns an error message if any problem is encountered in the previous step that set rc < 0
  if (rc < 0)
  {
    std::string strJson;
    strJson += "{\"status\" : \"failed\"}";
    reader = builder.newCharReader();
    parsingSuccessful = reader->parse (strJson.c_str(), strJson.c_str() + strJson.size(), &result_json, &errors);
    delete reader;

    if (!parsingSuccessful) 
    {
	  std::cout << "Failed to parse the JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
    }
  }

  return result_json;
}

int
main(void)
{
  HttpServer httpserver(8384);
  MyfbServer s(httpserver, JSONRPC_SERVER_V1V2);
  s.StartListening();
  std::cout << "Hit enter to stop the server" << endl;
  getchar();

  s.StopListening();
  return 0;
}