// dotDevice 
// A header file for sending commands from your TinyPico to your virtual dotDevice on the server
// This is a thin layer around the basic websockets stuff that adds some rate limiting on the
// messages being sent so that students don't _accidentally_ bring the server to its knees.
//
//  Augmented the file so that it can send more efficient binary payloaded data.
//
// author: stf

#include <ArduinoWebsockets.h>
#include <WiFi.h>

using namespace websockets;
WebsocketsClient client;

class dotDevice{

        public:
                dotDevice(const char* ssid, const char* password, const char* ws) {
                        _ssid = ssid;
                        _password = password;
                        _ws = ws;

                        _time_last_send = millis();
                }

                void connect() {
                        // Connect to the server
                        WiFi.begin(_ssid, _password);

                        // Wait some time to connect to wifi
                        for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
                            delay(1000);
                        }

                        client.connect(_ws);

                }

                // Sends JSON data to the server
                void sendJSON(String s) {
                        while(_time_last_send + _rate_limit > millis()) { /* block */ }
                        client.send(s);
                        _time_last_send = millis();
                }

                // Send a binary payload of data to the server
                void sendBIN(char *t, size_t size){
                       while(_time_last_send + _rate_limit > millis()) { /* block */ }
                       client.sendBinary(t, size);
                       _time_last_send = millis(); 
                }


        private: 
                const char *_ssid;
                const char *_password;
                const char *_ws;
                const char *_gid;

                unsigned long _time_last_send;
                const unsigned long _rate_limit = 50; // can't send a command less than 50ms apart. 
        
};
