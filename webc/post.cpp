#include <iostream>                                                                                  
#include <curl/curl.h>                                                                               
#include <strings.h>                                                                                 
#include <fstream>                                                                                   
                                                                                                     
void get_env(std::string *ptr, std::string &key) {                                                   
  std::string cache;                                                                                 
  std::ifstream file (".env");                                                                       
  if(file.is_open())                                                                                 
    file >> cache;                                                                                   
  std::size_t pos = cache.find(key+"=");                                                             
  *key = var.substr((pos+_var.size())+1);                                                            
}                                                                                                    
                                                                                                     
size_t callback(void *ptr, size_t size, size_t nmemb, std::string *data) {                           
  data->append((char *) ptr, size * nmemb);                                                          
  return size * nmemb;                                                                               
}                                                                                                    
                                                                                                     
int main(void) {                                                                                     
  CURL *handle;                                                                                      
  CURLcode result;                                                                                   
  handle = curl_easy_init();                                                                         
                                                                                                     
  if(handle) {                                                                                       
    std::string endpoint, reference = "ETHERSCAN";                                                   
    get_env(&endpoint, reference);                                                                   
                                                                                                     
    std::string url = "https://api.etherscan.io/api?module=gastracker&action=gasoracle&apikey="+key; 
                                                                                                     
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());                                              
    std::string response;                                                                            
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, callback);                                       
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);                                          
                                                                                                     
    result = curl_easy_perform(handle);                                                              
    if (result != CURLE_OK)                                                                          
      fprintf(stderr, "curl_easy_perform() failed %s\n", curl_easy_strerror(result));                
                                                                                                     
    curl_easy_cleanup(handle);                                                                       
    handle = NULL;                                                                                   
                                                                                                     
    std::cout << response << std::endl;                                                              
  }                                                                                                  
                                                                                                     
  curl_global_cleanup();                                                                             
  return 0;                                                                                          
}                                                                                                    
