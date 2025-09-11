#if defined(TELNET_USE_ETHERNET)
# include "ETHTelnetStream.h"
#else
# include "TelnetStream.h"
#endif

#ifdef TNetwork


void
TelnetStream::handleInput(void)
{
  char c = client.read();
  on_input(String(c));
}


int
TelnetStream::available(void)
{
  if (client && isConnected())
    return client.available();

  return 0;
}


int
TelnetStream::read(void)
{
  if (client && isConnected())
    return client.read();

  return 0;
}


int
TelnetStream::peek(void)
{
  if (client && isConnected())
    return client.peek();

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
