#include <iostream>
#include <fstream>
#include <streambuf>
#include <string.h>

// JSON RPC part
#include <stdlib.h>
#include "fbclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

#include "Post.h"

using namespace jsonrpc;
using namespace std;

int
main(int argc, char *argv[])
{
  if (argc != 2)
  {
    cout << "Usage: ./fbsearch \"search.json\"" << endl;
    return -1;
  }
  // copy the content file into string str
  std::ifstream t(argv[1]);
  std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

  HttpClient httpclient("http://localhost:8384");
  fbClient myClient(httpclient, JSONRPC_CLIENT_V2);

  Json::Value result_json;

  // search
  try {
    
    result_json = myClient.search(str);
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }

  // print the empty array if nothing is returned, else save the posts in memory
  if (result_json.isMember("count") == NULL)
  {
      std::cout << result_json.toStyledString() << endl;
  } else
  {
    // print the posts in command line and save it in a file
    std::cout << result_json.toStyledString() << endl;
    for (int i = 0; i < result_json["count"].asInt(); i++)
    {
      char fname_buf[1024];
      bzero(fname_buf, 1024);
      sprintf(fname_buf, "post_%s.json", ((result_json["data"][i]["id"]).asString()).c_str());

      char post_json_buf[8192]; // limited size
      bzero(post_json_buf, 8192);
      sprintf(post_json_buf, "%s", (result_json["data"][i].toStyledString()).c_str());

      std::string str(fname_buf);
      std::ofstream out(str);
      out << post_json_buf;
      out.close();
      std::cout << fname_buf << " created" << std::endl; 
      
    } 
  }
  
  return 0;
}
