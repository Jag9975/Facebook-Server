#include <iostream>
#include <thread>
#include <chrono>

// JSON RPC part
#include <stdlib.h>
#include "fbclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

#include "Post.h"

using namespace jsonrpc;
using namespace std;

// sends the json file provided in the command line to the server
int
main(int argc, char *argv[])
{
  if (argc != 2) return -1;

  FILE *jf1 = fopen(argv[1], "r");
  if (jf1 == NULL) return -1;

  Json::CharReaderBuilder builder;
  Json::CharReader* reader;
  std::string errors;
  bool parsingSuccessful;

  // process JSON File
  long lSize1;
  size_t lresult1;

  // obtain file size:
  fseek(jf1, 0 , SEEK_END);
  lSize1 = ftell(jf1);
  rewind(jf1);

  // allocate memory to contain the whole file:
  char *jf1_ptr = (char *) malloc(sizeof(char)*lSize1);
  // copy the file into the buffer:
  lresult1 = fread(jf1_ptr, 1, lSize1, jf1);    
  if (lresult1 != lSize1)
    {
      fputs("Reading error", stderr);
      exit (-3);
    }

  Json::Value myv_message;
  reader = builder.newCharReader();
  parsingSuccessful = reader->parse(jf1_ptr, jf1_ptr + lSize1, &myv_message, &errors);
  delete reader;

  if (!parsingSuccessful) {
    std::cout << "Failed to parse the content of the first JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
  }
  fclose(jf1);
  
  Post * my_post_ptr = Json_2_Post(myv_message);
  
  HttpClient httpclient("http://localhost:8384");
  fbClient myClient(httpclient, JSONRPC_CLIENT_V2);

  Json::Value result_json;

  bool is_running = true;

  // keep trying to connect to the server every 5 seconds if it fails to connect
  while (is_running)
  {
    try {
      result_json = myClient.update((my_post_ptr->dumpJ())->toStyledString());
      is_running = false;
      std::cout << result_json.toStyledString() << endl;
    } catch (JsonRpcException &e) {
      cerr << e.what() << endl;
      cout << "Attempting to Connect with server again in 5 seconds" << endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(5000));      // pauses the code execution for 5 seconds
    }
  }

  return 0;
}
