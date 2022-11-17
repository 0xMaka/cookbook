#include <iostream>                                                                                                                          
#include <curl/curl.h>                                                                                                                       
                                                                                                                                             
struct Endpoint {                                                                                                                            
  std::string url;                                                                                                                           
  int port;                                                                                                                                  
  Endpoint(std::string _url, int _port) {                                                                                                    
    url = _url;                                                                                                                              
    port = _port;                                                                                                                            
  }                                                                                                                                          
  std::string endpoint(void) {                                                                                                               
    return url + ":" + std::to_string(port);                                                                                                 
  }                                                                                                                                          
};                                                                                                                                           
                                                                                                                                             
struct Method {                                                                                                                              
  std::string name;                                                                                                                          
  std::string params = "";                                                                                                                   
  Method(std::string _name) {                                                                                                                
    name = _name;                                                                                                                            
  }                                                                                                                                          
  std::string str() {                                                                                                                        
    return "\"jsonrpc\":\"2.0\",\"method\":\"" + name + "\",\"params\":[" + params + "]";                                                    
  }                                                                                                                                          
};                                                                                                                                           
                                                                                                                                             
struct Call {                                                                                                                                
  std::string *endpoint;                                                                                                                     
  std::string *header;                                                                                                                       
  std::string *method;                                                                                                                       
  int *id;                                                                                                                                   
  Call (                                                                                                                                     
    std::string *_endpoint_ptr,                                                                                                              
    std::string *_header_ptr,                                                                                                                
    std::string *_method_ptr,                                                                                                                
    int *_id_ptr                                                                                                                             
  ) {                                                                                                                                        
    endpoint = _endpoint_ptr;                                                                                                                
    header = _header_ptr;                                                                                                                    
    method = _method_ptr;                                                                                                                    
    id = _id_ptr;                                                                                                                            
  }                                                                                                                                          
};                                                                                                                                           
                                                                                                                                             
int get_id(int *idp) {                                                                                                                       
  return ++*idp;                                                                                                                             
}                                                                                                                                            
                                                                                                                                             
size_t write_callback(void *buffer, size_t size, size_t nmemb, std::string *data) {                                                          
  data->append((char *) buffer, size * nmemb);                                                                                               
  return size * nmemb;                                                                                                                       
}                                                                                                                                            
                                                                                                                                             
void request(Call *call, std::string *response) {                                                                                            
  CURL *curl;                                                                                                                                
  CURLcode res;                                                                                                                              
  curl = curl_easy_init();                                                                                                                   
                                                                                                                                             
  std::string header = *call->header;                                                                                                        
  std::string endpoint = *call->endpoint;        
                                                                                                                                                             
  if(curl) {                                                                                                                                                 
    std::string data = "{" + *call->method + ", " + "\"id\": " + std::to_string(get_id(call->id)) + "}";                                                     
                                                                                                                                                            
    struct curl_slist *list = NULL;                                                                                                                          
    list = curl_slist_append(list, header.c_str());                                                                                                          
                                                                                                                                                            
    curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());                                                                                                   
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);                                                                                                        
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());                                                                                                
                                                                                                                                                            
    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);                                                                                     
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);                                                                                                     
    res = curl_easy_perform(curl);                                                                                                                           
                                                                                                                                                             
    if (res != CURLE_OK)                                                                                                                                     
      fprintf(stderr, "curl_easy_perform() failed %s\n", curl_easy_strerror(res));                                                                           
                                                                                                                                                             
    curl_easy_cleanup(curl);                                                                                                                                 
  }                                                                                                                                                          
                                                                                                                                                            
  curl_global_cleanup();                                                                                                                                     
}
                                                                                                                                                            
int main(void) {                                                                                                                                             
  std::cout << "[x] ========================================================" << std::endl;                                                                  
  Endpoint geth("http://localhost", 8089);                                                                                                                   
  std::string node = geth.endpoint();                                                                                                                        
  std::string *np = &node;                                                                                                                                   
  std::cout << "[+] Connecting to endpoint: " <<  *np << std::endl;                                                                                                  
                                                                                                                                                            
  std::string json_header = "Content-Type: application/json";                                                                                                
  std::string *hp = &json_header;                                                                                                                            
  std::cout << "[+] Appending header: " <<  *hp << std::endl;                                                                                                    
                                                                                                                                                            
  int id = 0;                                                                                                                                                
  int *idp = &id;                                                                                                                                            
  std::cout << "[x] --------------------------------------------------------" << std::endl;                                                                  
                                                                                                                                                            
  // --                                                                                                                                                      
  Method chain_id("eth_chainId");                                                                                                                            
  std::string get_chainId = chain_id.str();                                                                                                                  
  Method block_number("eth_blockNumber");                                                                                                                    
  std::string get_blockNumber = block_number.str();                                                                                                          
  Method syncing("eth_syncing");                                                                                                                             
  std::string is_syncing = syncing.str();                                                                                                                    
  Method getBalance("eth_getBalance");                                                                                                                       
                                                                                                                                                             
  std::string *mp = &get_chainId;                                                                                                                            
                                                                                                                                                             
  // --    
  Call call_struct(np,hp,mp,idp);                                                                    
  Call *caller = &call_struct;                                                                       
  std::string buffer;                                                                                
  std::string *receiver = &buffer;                                                                   
                                                                                                     
  request(caller, receiver);                                                                         
                                                                                                     
  mp = &get_blockNumber;                                                                             
  caller->method = mp;                                                                               
  request(caller, receiver);                                                                         
                                                                                                     
  mp = &is_syncing;                                                                                  
  caller->method = mp;                                                                               
  request(caller, receiver);                                                                         
                                                                                                     
  getBalance.params = "\"0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2\",\"latest\"";                   
  std::string get_balance = getBalance.str();                                                        
  mp = &get_balance;                                                                                 
  caller->method = mp;                                                                               
  request(caller, receiver);                                                                         
                                                                                                     
  std::cout << buffer << std::endl;                                                                  
                                                                                                     
  return 0;                                                                                          
}                                                                                                    
/*
c++ test_rpc_call.cpp -lcurl -g -o test_rpc_call -O2
./test_rpc_call
[x] ========================================================
[+] Connecting to endpoint: http://localhost:8089
[+] Appending header: Content-Type: application/json
[x] --------------------------------------------------------
{"jsonrpc":"2.0","id":1,"result":"0x1"}
{"jsonrpc":"2.0","id":2,"result":"0xf3b81d"}
{"jsonrpc":"2.0","id":3,"result":false}
{"jsonrpc":"2.0","id":4,"result":"0x34a8327041e18bd2912cb"}
*/
