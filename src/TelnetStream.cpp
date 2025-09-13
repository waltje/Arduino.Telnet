#if defined(TELNET_USE_ETHERNET)
# include "ETHTelnetStream.h"
#else
# include "TelnetStream.h"
#endif

#ifdef TNetwork


void
TelnetStream::handleInput(void)
{
  char c = _client.read();

  _on_input(String(c));
}


int
TelnetStream::available(void)
{
  if (_client && isConnected())
    return _client.available();

  return 0;
}


int
TelnetStream::read(void)
{
  if (_client && isConnected())
    return _client.read();

  return 0;
}


int
TelnetStream::peek(void)
{
  if (_client && isConnected())
    return _client.peek();

  return 0;
}


void
TelnetStream::flush(void)
{
  return TelnetBaseClass::flush();
}


size_t
TelnetStream::write(uint8_t data)
{
  return TelnetBaseClass::write(data);
}


size_t
TelnetStream::write(const uint8_t *data, size_t size)
{
  return TelnetBaseClass::write(data, size);
}


#endif  /*TNetwork*/
