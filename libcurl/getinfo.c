#include <stdio.h>
#include <curl/curl.h>

int main(void) {
  CURL *curl;
  CURLcode res;
  
  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https:/lo.hobgobl.in");
    res = curl_easy_perform(curl);

    if (CURLE_OK == res) {
      char *ct;
      res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);

      if ((CURLE_OK == res) && ct)
        printf("[+] Received Content-Type: %s\n", ct);
    }
    curl_easy_cleanup(curl);
  }
  return 0;
}
