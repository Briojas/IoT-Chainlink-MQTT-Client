/*
Creates, or connects to, an access Point
    - Blake Riojas
*/        

//$$ Libraries $$//
#include <WiFi.h>

//$$ Functions $$//
    /*Inputs: 
        accessPointName     - name of soft access point
        accessPointPW       - password, must be 8 chars
        accessPointSoft     - true: creates an access point with the name and password given
                            - false (default): connects to an existing access point with the name and password given
        accessPointHidden   - false: visible 
                            - true (default): hidden
    */
void wifi_init(const char wifiName[], const char wifiPW[], bool softAccessPoint = false, bool hiddenAccessPoint = true);
