#include "EscapeCodes.h"


String EscapeCodes::cls(void)
{
  return prefix() + "1J";
}


String EscapeCodes::home(void)
{
  return prefix() + "H";
}


String EscapeCodes::cursorXY(int x, int y)
{
  return prefix() + String(y) + ";" + String(x) + "H";
}


String EscapeCodes::cursorUp(int x)
{
  return prefixAndNumberAndValue(x, 'A');
}


String EscapeCodes::cursorDown(int x)
{
  return prefixAndNumberAndValue(x, 'B');
}


String EscapeCodes::cursorRight(int x)
{
  return prefixAndNumberAndValue(x, 'C');
}


String EscapeCodes::cursorLeft(int x)
{
  return prefixAndNumberAndValue(x, 'D');
}


String EscapeCodes::setBG(int color)
{
  return setAttribute(color + 40);
}


String EscapeCodes::setFG(int color)
{
  return setAttribute(color + 30);
}


String EscapeCodes::clearLine(void)
{
  return prefix() + "2K";
}


String EscapeCodes::clearEoLine(void)
{
  return prefix() + "K";
}


String EscapeCodes::bold(String str) 
{
  return setAttribute(1) + str + setAttribute(22);
}


String EscapeCodes::blink(String str)
{
  return setAttribute(5) + str + setAttribute(25);
}


String EscapeCodes::italic(String str)
{
  return setAttribute(3) + str + setAttribute(23);
}


String EscapeCodes::underline(String str)
{
  return setAttribute(4) + str + setAttribute(24);
}


String EscapeCodes::inverse(String str)
{
  return setAttribute(7) + str + setAttribute(27);
}


String EscapeCodes::showCursor(bool blink)
{
  return prefix() + "?25" + (blink ? 'h' : 'l');
}


String EscapeCodes::reset(void)
{
  return prefix() + String('m');
}


String EscapeCodes::prefix(void)
{
  return String("\033[");
}


String EscapeCodes::prefixAndNumberAndValue(int x, char v)
{
  return prefix() + String(x) + String(v);
}


String EscapeCodes::setAttribute(int a)
{
  return prefixAndNumberAndValue(a, 'm');
}
