#include <Arduino.h>
#include <HTTPClient.h>

#include "Beras_POSTHandler.h"

Beras_POSTHandler::Beras_POSTHandler(HTTPClient *httpClient, const char *hostingUrl, const char *phpAuthenticationKey) : http{httpClient}, url{hostingUrl}, authKey{phpAuthenticationKey}
{
    resetRequestData();
}

void Beras_POSTHandler::resetRequestData()
{
    requestData = "auth_key=" + authKey;
}

void Beras_POSTHandler::addRequestData(const char *arrayKeyName, const char *arrayValue)
{
    requestData += '&';
    requestData += arrayKeyName;
    requestData += '=';
    requestData += arrayValue;
}

String Beras_POSTHandler::getStringPayload(const char *phpFilename, int *httpResponseCode)
{
    http->begin(url + phpFilename);
    http->addHeader(F("Content-Type"), F("application/x-www-form-urlencoded"));

    *httpResponseCode = http->POST(requestData);
    const String payload = http->getString();

    http->end();

    resetRequestData();

    return payload;
}