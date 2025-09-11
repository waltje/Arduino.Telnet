/////////////////////////////////////////////////////////////////

#if defined(TELNET_USE_ETHERNET)
# include "ETHTelnetStream.h"
#else
# include "ESPTelnetStream.h"
#endif

#ifdef TNetwork
/////////////////////////////////////////////////////////////////

void TelnetStream::handleInput() {
  char c = client.read();
  on_input(String(c));
}

/////////////////////////////////////////////////////////////////

int TelnetStream::available() {
  if (client && isConnected()) {
    return client.available();
  } else {
    return 0;
  }
}

/////////////////////////////////////////////////////////////////

int TelnetStream::read() {
  if (client && isConnected()) {
    return client.read();
  } else {
    return 0;
  }
}

/////////////////////////////////////////////////////////////////

int TelnetStream::peek() {
  if (client && isConnected()) {
    return client.peek();
  } else {
    return 0;
  }
}

/////////////////////////////////////////////////////////////////

void TelnetStream::flush() {
  return TelnetBaseClass::flush();
}

/////////////////////////////////////////////////////////////////

size_t TelnetStream::write(uint8_t data) {
  return TelnetBaseClass::write(data);
}

/////////////////////////////////////////////////////////////////

size_t TelnetStream::write(const uint8_t* data, size_t size) {
  return TelnetBaseClass::write(data, size);
}

/////////////////////////////////////////////////////////////////

#endif  /*TNetwork*/
