#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// shows how the write callback function can be used to download data
// into a chunk of memory instead of storing it in a file.

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    // out of memory
    printf("[!] not enough memory (realloc returned NULL)\n");
    return 0;
  }
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
  return realsize;
}

int main(void) {
  CURL *curl_handle;
  CURLcode res;

  struct MemoryStruct chunk;

  chunk.memory = malloc(1); // grown as needed by realloc
  chunk.size = 0;             // no data currently

  curl_global_init(CURL_GLOBAL_ALL);
  curl_handle = curl_easy_init(); // initialize session

  curl_easy_setopt(curl_handle, CURLOPT_URL, "https://lo.hobgobl.in");       // specify url to get
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); // send data to this function
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) &chunk);         // pass our chunk struct back to callback function
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");     // provide user agent

  res = curl_easy_perform(curl_handle); // fetch

  // check for errors
  if (res != CURLE_OK) {
    fprintf(stderr, "[!] curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  } else {
    // now chunk .mem points to mem block of chunk.size bytes
    printf("[+] %lu bytes retrieved\n", (unsigned long)chunk.size);
  }
  // clean up
  curl_easy_cleanup(curl_handle);
  free(chunk.memory);
  curl_global_cleanup();

  return 0;

}
