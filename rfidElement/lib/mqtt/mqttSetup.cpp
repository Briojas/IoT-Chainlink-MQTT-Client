#include <mqttSetup.h>
#include <Arduino.h>

MQTT_Client_Handler::MQTT_Client_Handler(MQTTClient &_mqttClient, Client &_wifiClient, const char brokerName[], mqtt_pubSubDef_t *_subs, int _numSubs, MQTTClientCallbackSimple callback){
    this->mqttClient = &_mqttClient;
    this->subs = _subs;
    this->numSubs = _numSubs;
    this->mqttClient->begin(brokerName, _wifiClient);
    this->mqttClient->onMessage(callback);
}

void MQTT_Client_Handler::connect(const char deviceName[], const char brokerLogin[], const char brokerPW[]){
        //addressing advanced settings
    char lwt_topic[50];
    strcat(lwt_topic, "/");
    strcat(lwt_topic, deviceName);
    strcat(lwt_topic, this->lwt_topic);
    this->mqttClient->setWill(lwt_topic, this->lwt_payload, this->lwt_retained, this->lwt_qos);
    this->mqttClient->setOptions(this->keepAlive, this->cleanSession, this->timeout);
        //connect to broker
    Serial.println("connecting...");
    while(!this->mqttClient->connect(deviceName, brokerLogin, brokerPW)){
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\nconnected");
        //subscribe to relevant topics
    Serial.println("establishing subscriptions:");
    int i;
    for(i=0; i<this->numSubs; i++){
        this->mqttClient->subscribe(this->subs[i].topic, this->subs[i].qos);
        Serial.println(this->subs[i].topic);
    }
    Serial.println("subscribed successfully");
}

bool MQTT_Client_Handler::loop(){
    this->mqttClient->loop();
    if(!this->mqttClient->connected()){
        return false;
    }
    return true;
}

void MQTT_Client_Handler::publish(mqtt_pubSubDef_t message){
    this->mqttClient->publish(
        message.topic,
        message.payload,
        message.retained,
        message.qos
    );
}
