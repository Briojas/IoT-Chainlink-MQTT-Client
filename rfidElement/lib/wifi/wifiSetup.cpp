#include <Arduino.h>
#include <wifiSetup.h>

void wifi_init(const char wifiName[], const char wifiPW[], bool softAccessPoint, bool hiddenAccessPoint){
    IPAddress myIP;
    int hideAP;
    if (softAccessPoint){ 
            //create an access point
        WiFi.enableAP(true);
        if(hiddenAccessPoint){
            hideAP = 1;
        }else{
            hideAP = 0;
        }
        Serial.println(WiFi.softAP(wifiName, wifiPW, 1, hideAP, 4)); //ssid,pw,channel,hidden,connections
        myIP = WiFi.softAPIP();
        Serial.print("Created new access point called: ");
        Serial.println(wifiName);
    } else {
            //connect to existing WiFi network
        WiFi.mode(WIFI_STA); 
        WiFi.begin(wifiName, wifiPW);
        Serial.println("waiting for wifi to be connected...");
        while (WiFi.status() != WL_CONNECTED){delay(500);} //wait for connection
        Serial.println("WiFi connected");
        myIP = WiFi.localIP();
    }
        //display IP address
    Serial.print("IP address: ");
    Serial.println(myIP);
}