#if defined(TELNET_USE_ETHERNET)
# include "ETHTelnet.h"
#else
# include "Telnet.h"
#endif

#ifdef TNetwork

# ifndef TELNET_VPRINTF_BUFSIZE
#  define TELNET_VPRINTF_BUFSIZE 64
# endif


void
Telnet::handleInput(void)
{
  char c = _client.read();

  // collect string
  if (_lineMode) {
    if (c != _newlineCharacter) {
      if (c >= 32 && c < 127) {
        _input += c;
      }
      // EOL -> send input
    } else {
      _on_input(_input);
      _input = "";
    }
    // send individual characters
  } else {
    if (_input.length()) {
      _on_input(_input + String(c));
      _input = "";
    } else {
      _on_input(String(c));
    }
  }
}


void
Telnet::println(void)
{
  if (_client && isConnected()) {
    if (! _client.println())
      onFailedWrite();
    else
      onSuccessfullyWrite();
  }
}


size_t
Telnet::printf(const char *format, ...)
{
  if (!_client || !isConnected())
    return 0;
  
  va_list arg;
  va_start(arg, format);
  size_t len = vprintf(format, arg);
  va_end(arg);

  return len;
}


size_t
Telnet::vprintf(const char *format, va_list arg)
{
  char loc_buf[TELNET_VPRINTF_BUFSIZE];
  int len = vsnprintf(loc_buf, sizeof(loc_buf), format, arg);

  if (len < 0)
    return 0;

  if (len >= (int)sizeof(loc_buf)) {
    char *temp = (char *)malloc(len + 1);
    if (temp == nullptr) {
      return 0;
    }
    vsnprintf(temp, len + 1, format, arg);
    len = write((uint8_t *)temp, len);
    free(temp);
  } else
    len = write((uint8_t *)loc_buf, len);

  return len;
}


bool
Telnet::isLineModeSet(void)
{
  return _lineMode;
}


void
Telnet::setLineMode(bool value)
{
  _lineMode = value;
}


char
Telnet::getNewlineCharacter(void)
{
  return _newlineCharacter;
}


void
Telnet::setNewlineCharacter(char value)
{
  _newlineCharacter = value;
}


#endif  /*TNetwork*/
