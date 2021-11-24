/*
Establishes client connection to MQTT 3.1.1 Broker
    - Blake Riojas (9/16/21)
*/        

//$$ Libraries $$//
#include <MQTT.h>
#include <WiFi.h> //needed for WiFiClient references

//$$ Structs $$//
struct mqtt_pubSubDef_t {
    String topic; //topic being subscribed to or published on
    String payload; //Storage of data to be published or of data subscribed to
    int qos = 0; //quality of service (qos) level (default 0)
    bool retained = false; //should published message be kept and provided to new subscribing clients
};

//$$ Classes & Functions $$//
class MQTT_Client_Handler {
    public:
            //mqtt client
        MQTTClient *mqttClient = nullptr; 
            //array and size of the array for all the messages that will be subscribed to
        mqtt_pubSubDef_t *subs; 
        int numSubs;

    //These must be changed before calling connect(). 
            //last will and testament defaults. 
        char lwt_topic[20] = "/lastWill"; //lwt topic. device's name will be appended to front.
        char lwt_payload[20] = "connection lost";
        bool lwt_retained = true;
        int lwt_qos = 2;
            //keep alive interval (seconds)
        int keepAlive = 10;
            //session retention on the broker side
        bool cleanSession = true;
            //timeout for all commands (milliseconds)
        int timeout = 1000;

    /*    
    Inputs: 
        MQTTClient &_mqttClient - 
        Client &_wifiClient - 
        mqtt_pubSubDef_t *_pubSubs - 
        int _numPubSubs - 
        void *callback
    Purpose:
        Class constructor
    Outputs:  
        None
    */
        MQTT_Client_Handler(MQTTClient &_mqttClient, Client &_wifiClient, const char brokerName[], mqtt_pubSubDef_t *_subs, int _numSubs, MQTTClientCallbackSimple callback, int port = 1883);

    /*
    Inputs: 
        const char deviceName[] - name of this device as it will appear in the broker
        const char brokerLogin[] - login username for the cloud mqtt service
        const char brokerPW[] - login password for the cloud mqtt service
        MQTTClient &mqttClient - mqtt client itself that connects to the broker
    Purpose:
        mqtt_connect will initiate a connection to the mqtt cloud broker for this specific mqtt client. 
    Outputs:
        None. mqtt_client.connected() will be used to determine connection status to the broker. 
    */
        void connect(const char deviceName[]);
        void connect(const char deviceName[], const char brokerLogin[]);
        void connect(const char deviceName[], const char brokerLogin[], const char brokerPW[]);

    /*
    Inputs: 
        const char deviceName[] - name of this device as it will appear in the broker
    Purpose:
        Sets last will and testament of device
    Outputs:  
        None
    */
        void set_options(const char deviceName[]);

    /*
    Inputs: 
        None
    Purpose:
        Periodically checks for new messages relevant to this device's sub list
    Outputs:  
        True - client is still connected to broker and fetching relvant subs
        False - client is currently disconnected and needs to reconnect
    */
        bool loop();

    /*
    Inputs: 
        mqtt_subDef_t message - a message with a payload that is ready to be published
    Purpose:
        Submits the message to the broker for publishing.
    Outputs:  
        None
    */
        void publish(mqtt_pubSubDef_t message);
    
    /*
    Inputs: 
        None, subs and their latest callback data are stored within the client
    Purpose:
        Subscribes to topics on the broker specified with the qos specified in the constructor
    Outputs:  
        None
    */
        void subscribe();
};