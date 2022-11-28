#ifndef MASU_H                                                                                                                            
#define MASU_H                                                                                                                            
                                                                                                                                          
#include <cstdio>                                                                                                                         
#include <stdexcept>                                                                                                                      
#include <time.h>                                                                                                                         
#include <cstring>                                                                                                                        
                                                                                                                                          
static const size_t MAX_LEN = 15;                                                                                                         
                                                                                                                                          
struct Taima {                                                                                                                            
  // constructor                                                                                                                          
  Taima (const char *_tag) {                                                                                                              
    if (strlen(_tag) > MAX_LEN) throw std::runtime_error { "[!] Max tag size is 15." };                                                   
    tag = new char [MAX_LEN+1];                                                                                                           
    tag = _tag;                                                                                                                           
    timestamp = clock();                                                                                                                  
  }                                                                                                                                       
  // destructor                                                                                                                           
  ~Taima (void) {                                                                                                                         
    if (timestamp) {                                                                                                                      
      timestamp = clock() - timestamp;                                                                                                    
      printf("[+] %s: stamped in @ %fs\n", tag, (double) timestamp / (double) CLOCKS_PER_SEC);                                            
      tag = nullptr;                                                                                                                      
      delete[] tag;                                                                                                                       
    }                                                                                                                                     
  }                                                                                                                                       
  // copy constructor                                                                                                                     
  Taima(const Taima& og) : timestamp{ og.timestamp }, tag{ new char [MAX_LEN+1] } {                                                       
    std::memcpy((char *)tag, og.tag, sizeof(og.tag));                                                                                     
  }                                                                                                                                       
  // copy assignment operator                                                                                                             
  Taima &operator = (Taima &og) {                                                                                                         
    if (this == &og) return *this;                                                                                                        
    timestamp = og.timestamp;                                                                                                             
    tag = og.tag;                                                                                                                         
    return *this;                                                                                                                         
  }                                                                                                                                       
  // move constructor                                                                                                                     
  Taima(Taima&& og) noexcept : timestamp{ og.timestamp }, tag{ og.tag } {                                                                 
    og.timestamp = 0;                                                                                                                     
    og.tag = nullptr;                                                                                                                     
  }                                                                                                                                       
  // move assignment operator                                                                                                             
  Taima &operator = (Taima&& og) noexcept {                                                                                               
    if (this == &og) return *this;                                                                                                        
    timestamp = og.timestamp;                                                                                                             
    tag = og.tag;                                                                                                                         
    og.timestamp = 0;                                                                                                                     
    og.tag = nullptr;                                                                                                                     
    return *this;                                                                                                                         
  }                                                                                                                                       
  private:                                                                                                                                
  clock_t timestamp;                                                                                                                      
  const char *tag;                                                                                                                        
};                                                                                                                                        
                                                                                                                                          
#endif /* MASU_H */                                                                                                                         
