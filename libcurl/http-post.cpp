#include <iostream>                                                                              
#include <curl/curl.h>                                                                           
                                                                                                 
std::string ENDPOINT = "<enter_endpoint>";  
std::string call = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_blockNumber\",\"params\":[],\"id\":0}";
                                                                                                 
size_t write_callback(void *buffer, size_t size, size_t nmemb, std::string *data) {              
  data -> append ((char *) buffer, size * nmemb);                                                
  return size * nmemb;                                                                           
}                                                                                                
                                                                                                 
int main(void) {                                                                                 
  CURL *curl;                                                                                    
  CURLcode res;                                                                                  
                                                                                                 
  curl = curl_easy_init();                                                                       
                                                                                                 
  if(curl) {                                                                                     
    curl_easy_setopt(curl, CURLOPT_URL, ENDPOINT.c_str());                                       
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, call.c_str());                                    
                                                                                                 
    std::string response;                                                                        
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);                               
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);                                        
    res = curl_easy_perform(curl);                                                               
                                                                                                 
    if(res != CURLE_OK)                                                                          
      std::cout << stderr << "curl_easy_perform() failed: %s\n" << std::endl;                    
                                                                                                 
    curl_easy_cleanup(curl);                                                                     
    curl = NULL;                                                                                 
    std::cout << response << std::endl;                                                          
  }                                                                                              
                                                                                                 
  curl_global_cleanup();                                                                         
  return 0;                                                                                      
}                                                                                                                                                                                         
