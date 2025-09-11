#ifndef TelnetBase_h
# define TelnetBase_h

# include <Arduino.h>
# if defined(TELNET_USE_ETHERNET)
#  include <EthernetNG.h>
#  include <EthernetClient.h>
#  include <EthernetServer.h>
#  define TNetwork Ethernet
#  define TClient EthernetClient
#  define TServer EthernetServer
# elif defined(TELNET_USE_WIFI)
#  include <WiFi.h>
#  define TNetwork WiFi
#  define TClient WiFiClient
#  define TServer WiFiServer
# elif defined(ARDUINO_ARCH_ESP32)
#  include <WiFi.h>
#  define TNetwork WiFi
#  define TClient WiFiClient
#  define TServer WiFiServer
# elif defined(ARDUINO_ARCH_ESP8266)
#  include <ESP8266WiFi.h>
#  define TNetwork WiFi
#  define TClient WiFiClient
#  define TServer WiFiServer
# endif

# ifdef TNetwork
#  include "DebugMacros.h"

#  define ASCII_BELL              7
#  define ASCII_BACKSPACE         8
#  define ASCII_LF                10
#  define ASCII_CR                13
#  define KEEP_ALIVE_INTERVAL_MS  1000
#  define MAX_ERRORS_ON_WRITE     3

using TCPClient = TClient;
using TCPServer = TServer;

#  if defined(TELNET_USE_ETHERNET)
#   define TelnetBaseClass    ETHTelnetBase
#  else
#   define TelnetBaseClass    TelnetBase
#  endif


class TelnetBaseClass {
  public:
    typedef void (*CallbackFunction)(String str);

    TelnetBaseClass();

    bool begin(uint16_t port = 23, bool checkConnection = true);
    void stop(void);
    void loop(void);

    bool isConnected(void);
    void disconnectClient(bool triggerEvent = true);

    void setKeepAliveInterval(int ms);
    int getKeepAliveInterval(void);

    virtual void flush(void);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t* data, size_t size);

    String getIP(void) const;
    String getLastAttemptIP(void) const;

    TCPClient& getClient(void);

    void onConnect(CallbackFunction f);
    void onConnectionAttempt(CallbackFunction f);
    void onReconnect(CallbackFunction f);
    void onDisconnect(CallbackFunction f);
    void onInputReceived(CallbackFunction f);

  protected:
    TCPServer server = TCPServer(23);  // must be initalized here
    TCPClient client;
    bool connected = false;  // needed because I cannot do "client = NULL"
    String ip = "";
    String attemptIp;
    String input = "";

    uint16_t server_port = 23;
    int keep_alive_interval = KEEP_ALIVE_INTERVAL_MS;
    long last_status_check;
    unsigned int failedWrites = 0;

    CallbackFunction on_connect = NULL;
    CallbackFunction on_reconnect = NULL;
    CallbackFunction on_disconnect = NULL;
    CallbackFunction on_connection_attempt = NULL;
    CallbackFunction on_input = NULL;

    virtual void onFailedWrite(void);
    virtual void onSuccessfullyWrite(void);

    void emptyClientStream(void);
    bool _isIPSet(IPAddress ip);
    virtual void handleInput(void) = 0;

  private:
    void connectClient(TCPClient c, bool triggerEvent = true);
    void processClientConnection(void);
    void performKeepAliveCheck(void);
    void handleClientInput(void);
    void notifyConnectionAttempt(const String &attemptedIp);
    void handleReconnection(TCPClient &newClient, const String &attemptedIp); 
    void handleExistingConnection(TCPClient &newClient);
    bool doKeepAliveCheckNow(void);
};


# endif /*TNetwork*/

#endif  /*TelnetBase.h_*/
