#include "TelnetBase.h"
#ifdef TNetwork


#if defined(ARDUINO_ARCH_AVR)
static String
getIPaddress(uint32_t ip)
{
  char temp[16];
  uint8_t b1, b2, b3, b4;

  /*
   * It seems that the String() function in most cores does not
   * convert the address into a dotted-decimal number, but just
   * to an unsigned decimal. 
   * 
   * In addition, on AVR at least, a direct print of the shifted 
   * bytes will mess up the numbers, so I now do it the weird way..
    */
  b1 = (ip & 0xff);
  ip >>= 8;
  b2 = (ip & 0xff);
  ip >>= 8;
  b3 = (ip & 0xff);
  ip >>= 8;
  b4 = (ip & 0xff);

  snprintf_P(temp, sizeof(temp),
             PSTR("%d.%d.%d.%d"), b1, b2, b3, b4);

  return String(temp);
}
#endif


TelnetBaseClass::TelnetBaseClass(void) :
  _connected(false)
{
}


bool
TelnetBaseClass::begin(uint16_t port, bool checkConnection)
{
  _ip = "";

  if (checkConnection) {
#ifndef TELNET_USE_ETHERNET
    // connected to WiFi or is ESP in AP mode?
    if (WiFi.status() != WL_CONNECTED && !_isIPSet(WiFi.softAPIP())) return false;
#endif
  }
  _server_port = port;
  _server = TCPServer(port);
  _server.begin();
  _server.setNoDelay(true);

  return true;
}


void
TelnetBaseClass::loop(void)
{
  processClientConnection();
  performKeepAliveCheck();
  handleClientInput();
  // see https://github.com/LennartHennigs/ESPTelnet/issues/73
  // I removed the yield() call, as it causes context switches
  // yield(); 
}


void
TelnetBaseClass::processClientConnection(void)
{
  TCPClient newClient;

  if (! _server.hasClient())
    return;

  newClient = _server.accept();
  if (! _connected)
    connectClient(newClient);
  else
    handleExistingConnection(newClient);
}


void
TelnetBaseClass::handleExistingConnection(TCPClient& newClient)
{
#if defined(ARDUINO_ARCH_AVR)
  String attemptedIp = getIPaddress((uint32_t)newClient.remoteIP());
#else
  String attemptedIp = newClient.remoteIP().toString();
#endif

  if (! isConnected()) {
    disconnectClient();
    return;
  }

  if (attemptedIp == _ip)
    handleReconnection(newClient, attemptedIp);
  else
    notifyConnectionAttempt(attemptedIp);
}


void
TelnetBaseClass::handleReconnection(TCPClient& newClient, const String& attemptedIp)
{
  disconnectClient(false);
  connectClient(newClient, false);

  if (_on_reconnect)
    _on_reconnect(attemptedIp);
}


void
TelnetBaseClass::notifyConnectionAttempt(const String& attemptedIp)
{
  if (_on_connection_attempt)
    _on_connection_attempt(attemptedIp);
}


void
TelnetBaseClass::performKeepAliveCheck(void)
{
  if (doKeepAliveCheckNow() && _connected && !isConnected()) {
    disconnectClient();
  }
}


void
TelnetBaseClass::handleClientInput(void)
{
  if (_on_input && _client && _client.available()) {
    handleInput();
  }
}


bool
TelnetBaseClass::doKeepAliveCheckNow(void)
{
  uint32_t currentTime = millis();
  uint32_t timeElapsed = currentTime - _last_status_check;

  if (timeElapsed >= static_cast<uint32_t>(_keep_alive_interval)) {
    _last_status_check = currentTime;
    return true;
  }

  return false;
}


void
TelnetBaseClass::setKeepAliveInterval(int ms)
{
  _keep_alive_interval = ms;
}


int
TelnetBaseClass::getKeepAliveInterval(void)
{
  return _keep_alive_interval;
}


void
TelnetBaseClass::flush(void)
{
  if (!_client || !isConnected())
    return;

// only the ESP8266 has a "bool flush()" method
// https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/WiFiClient.cpp#L306
#ifdef ARDUINO_ARCH_ESP8266
  if (!_client.flush(this->getKeepAliveInterval())) {
    onFailedWrite();
  } else {
    onSuccessfullyWrite();
  }
#else
  _client.flush();
#endif
}


size_t
TelnetBaseClass::write(const uint8_t *data, size_t size)
{
  if (! _client || !isConnected())
    return 0;

  size_t written = _client.write(data, size);
  if (written != size)
    onFailedWrite();
  else
    onSuccessfullyWrite();

  return written;
}


size_t
TelnetBaseClass::write(uint8_t data)
{
  if (!_client || !isConnected())
    return 0;

  size_t written = _client.write(data);
  if (! written)
    onFailedWrite();
  else
    onSuccessfullyWrite();

  return written;
}


void
TelnetBaseClass::connectClient(TClient c, bool triggerEvent)
{
#if defined(ARDUINO_ARCH_AVR)
  _ip = getIPaddress((uint32_t)c.remoteIP());
#else
  _ip = c.remoteIP().toString();
#endif

  _client = c;
  _client.setNoDelay(true);
  _client.setTimeout(this->getKeepAliveInterval());
  if (triggerEvent && _on_connect != NULL)
    _on_connect(_ip);
  emptyClientStream();

  _connected = true;
}


void
TelnetBaseClass::disconnectClient(bool triggerEvent)
{
  emptyClientStream();
  _client.stop();

  if (triggerEvent && _on_disconnect != NULL)
    _on_disconnect(_ip);

  _ip = "";

  _connected = false;
}


// helper method, as ESP32's IPAddress has no isSet() method
bool
TelnetBaseClass::_isIPSet(IPAddress ip)
{
#if defined(ARDUINO_ARCH_ESP8266)
  return ip.isSet();
#elif defined(ARDUINO_ARCH_AVR)
  return (uint32_t)ip != 0;
#else
  // this works for ESP32, hopefully for others as well
  return ip.toString() != "0.0.0.0";
#endif
}


void
TelnetBaseClass::stop(void)
{
  _server.stop();
}


bool
TelnetBaseClass::isConnected(void)
{
  bool res = true;

#if defined(ARDUINO_ARCH_ESP8266)
  res = _client.status() == ESTABLISHED;
#elif defined(ARDUINO_ARCH_ESP32)
  res = _client.connected();
#else
  res = _client.connected();
#endif

  return res;
}


String
TelnetBaseClass::getIP(void) const
{
  return _ip;
}


String
TelnetBaseClass::getLastAttemptIP(void) const
{
  return _attemptIp;
}


void
TelnetBaseClass::onFailedWrite(void)
{
  _failedWrites++;

  if (_failedWrites >= MAX_ERRORS_ON_WRITE) {
    _failedWrites = 0;
    disconnectClient();
  }
}


void
TelnetBaseClass::onSuccessfullyWrite(void)
{
  if (_failedWrites > 0)
    _failedWrites = 0;
}


void
TelnetBaseClass::emptyClientStream(void)
{
  flush();
  delay(50);

  while (_client.available())
    _client.read();
}


void
TelnetBaseClass::onConnect(CallbackFunction f)
{
  _on_connect = f;
}


void
TelnetBaseClass::onConnectionAttempt(CallbackFunction f)
{
  _on_connection_attempt = f;
}


void
TelnetBaseClass::onReconnect(CallbackFunction f)
{
  _on_reconnect = f;
}


void
TelnetBaseClass::onDisconnect(CallbackFunction f)
{
  _on_disconnect = f;
}


void
TelnetBaseClass::onInputReceived(CallbackFunction f)
{
  _on_input = f;
}


TCPClient&
TelnetBaseClass::getClient(void)
{
  return _client;
}


#endif  /*TNetwork*/
