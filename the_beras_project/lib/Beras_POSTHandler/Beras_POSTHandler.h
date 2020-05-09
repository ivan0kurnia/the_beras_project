#ifndef BERAS_POST_HANDLER_h
#define BERAS_POST_HANDLER_h

#include <Arduino.h>
#include <HTTPClient.h>

class Beras_POSTHandler
{
public:
    Beras_POSTHandler(HTTPClient *httpClient, const char *hostingUrl, const char *phpAuthenticationKey);

    void resetRequestData();
    void addRequestData(const char *arrayKeyName, const char *arrayValue);

    String getStringPayload(const char *phpFilename, int *httpResponseCode);

private:
    HTTPClient *http;
    const String url;
    const String authKey;

    String requestData;
};

#endif