/////////////////////////////////////////////////////////////////

#include "ESPTelnetBase.h"

/////////////////////////////////////////////////////////////////

#ifdef TNetwork

TelnetBaseClass::TelnetBaseClass() {
  connected = false;
}

/////////////////////////////////////////////////////////////////

bool TelnetBaseClass::begin(uint16_t port /* = 23 */, bool checkConnection /* = true */) {
  ip = "";
  if (checkConnection) {
#ifndef TELNET_USE_ETHERNET
    // connected to WiFi or is ESP in AP mode?
    if (WiFi.status() != WL_CONNECTED && !_isIPSet(WiFi.softAPIP())) return false;
#endif
  }
  server_port = port;
  server = TCPServer(port);
  server.begin();
  server.setNoDelay(true);
  return true;
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::loop() {
  processClientConnection();
  performKeepAliveCheck();
  handleClientInput();
  // see https://github.com/LennartHennigs/ESPTelnet/issues/73
  // I removed the yield() call, as it causes context switches
  // yield(); 
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::processClientConnection() {
  if (!server.hasClient()) return;
  TCPClient newClient = server.accept();
  if (!connected) {
    connectClient(newClient);
  } else {
    handleExistingConnection(newClient);
  }
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::handleExistingConnection(TCPClient &newClient) {
  String attemptedIp = newClient.remoteIP().toString();

  if (!isConnected()) {
    disconnectClient();
    return;
  }

  if (attemptedIp == ip) {
    handleReconnection(newClient, attemptedIp);
  } else {
    notifyConnectionAttempt(attemptedIp);
  }
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::handleReconnection(TCPClient &newClient, const String &attemptedIp) {
  disconnectClient(false);
  connectClient(newClient, false);
  if (on_reconnect) on_reconnect(attemptedIp);
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::notifyConnectionAttempt(const String &attemptedIp) {
  if (on_connection_attempt) on_connection_attempt(attemptedIp);
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::performKeepAliveCheck() {
  if (doKeepAliveCheckNow() && connected && !isConnected()) {
    disconnectClient();
  }
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::handleClientInput() {
  if (on_input && client && client.available()) {
    handleInput();
  }
}


/////////////////////////////////////////////////////////////////

bool TelnetBaseClass::doKeepAliveCheckNow() {
  unsigned long currentTime = millis();
  unsigned long timeElapsed = currentTime - last_status_check;

  if (timeElapsed >= static_cast<unsigned long>(keep_alive_interval)) {
    last_status_check = currentTime;
    return true;
  }
  return false;
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::setKeepAliveInterval(int ms) {
  keep_alive_interval = ms;
}

/////////////////////////////////////////////////////////////////

int TelnetBaseClass::getKeepAliveInterval() {
  return keep_alive_interval;
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::flush() {
  if (!client || !isConnected()) {
    return;
  }

// only the ESP8266 has a "bool flush()" method
// https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/WiFiClient.cpp#L306

#ifdef ARDUINO_ARCH_ESP8266
  if (!client.flush(this->getKeepAliveInterval())) {
    onFailedWrite();
  } else {
    onSuccessfullyWrite();
  }
#else
  client.flush();
#endif
}

/////////////////////////////////////////////////////////////////

size_t TelnetBaseClass::write(uint8_t data) {
  if (!client || !isConnected()) {
    return 0;
  }

  size_t written = client.write(data);
  if (!written) {
    onFailedWrite();
  } else {
    onSuccessfullyWrite();
  }

  return written;
}

/////////////////////////////////////////////////////////////////

size_t TelnetBaseClass::write(const uint8_t* data, size_t size) {
  if (!client || !isConnected()) {
    return 0;
  }

  size_t written = client.write(data, size);
  if (written != size) {
    onFailedWrite();
  } else {
    onSuccessfullyWrite();
  }

  return written;
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::connectClient(TClient c, bool triggerEvent) {
  client = c;
  ip = client.remoteIP().toString();
  client.setNoDelay(true);
  client.setTimeout(this->getKeepAliveInterval());
  if (triggerEvent && on_connect != NULL) on_connect(ip);
  emptyClientStream();
  connected = true;
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::disconnectClient(bool triggerEvent) {
  emptyClientStream();
  client.stop();
  if (triggerEvent && on_disconnect != NULL) on_disconnect(ip);
  ip = "";
  connected = false;
}

/////////////////////////////////////////////////////////////////
// helper method, as ESP32's IPAddress has no isSet() method

bool TelnetBaseClass::_isIPSet(IPAddress ip) {
#if defined(ARDUINO_ARCH_ESP8266)
  return ip.isSet();
#else
  // this works for ESP32, hopefully for others as well
  return ip.toString() != "0.0.0.0";
#endif
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::stop() {
  server.stop();
}

/////////////////////////////////////////////////////////////////

bool TelnetBaseClass::isConnected() {
  bool res = true;
#if defined(ARDUINO_ARCH_ESP8266)
  res = client.status() == ESTABLISHED;
#elif defined(ARDUINO_ARCH_ESP32)
  res = client.connected();
#else
  res = client.connected();
#endif
  return res;
}

/////////////////////////////////////////////////////////////////

String TelnetBaseClass::getIP() const {
  return ip;
}

/////////////////////////////////////////////////////////////////

String TelnetBaseClass::getLastAttemptIP() const {
  return attemptIp;
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::onFailedWrite() {
  failedWrites++;

  if (failedWrites >= MAX_ERRORS_ON_WRITE) {
    failedWrites = 0;
    disconnectClient();
  }
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::onSuccessfullyWrite() {
  if (failedWrites > 0) {
    failedWrites = 0;
  }
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::emptyClientStream() {
  flush();
  delay(50);
  while (client.available()) {
    client.read();
  }
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::onConnect(CallbackFunction f) {
  on_connect = f;
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::onConnectionAttempt(CallbackFunction f) {
  on_connection_attempt = f;
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::onReconnect(CallbackFunction f) {
  on_reconnect = f;
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::onDisconnect(CallbackFunction f) {
  on_disconnect = f;
}

/////////////////////////////////////////////////////////////////

void TelnetBaseClass::onInputReceived(CallbackFunction f) {
  on_input = f;
}

/////////////////////////////////////////////////////////////////

  TCPClient& TelnetBaseClass::getClient() {
    return client;
  }

#endif  /*TNetwork*/
