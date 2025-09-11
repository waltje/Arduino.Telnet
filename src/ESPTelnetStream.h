/////////////////////////////////////////////////////////////////

#pragma once
#ifndef ESPTelnetStream_h
#define ESPTelnetStream_h

/////////////////////////////////////////////////////////////////

#if defined(TELNET_USE_ETHERNET)
# include "ETHTelnetBase.h"
# define TelnetStream ETHTelnetStream
#else
# include "ESPTelnetBase.h"
# define TelnetStream ESPTelnetStream
#endif

#ifdef TNetwork
/////////////////////////////////////////////////////////////////

class TelnetStream : public TelnetBaseClass, public Stream {
 public:
  using TelnetBaseClass::TelnetBaseClass;

  int available();
  int read();
  int peek();
  void flush() override;

  size_t write(uint8_t) override;
  size_t write(const uint8_t* data, size_t size) override;

 protected:
  void handleInput();
};

/////////////////////////////////////////////////////////////////

// << operator
//  template<class T> inline TelnetStream &operator <<(TelnetStream &obj, T arg) { obj.print(arg); return obj; }

/////////////////////////////////////////////////////////////////

#endif  /*TNetwork*/

#endif
/////////////////////////////////////////////////////////////////
