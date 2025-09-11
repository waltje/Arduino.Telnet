/////////////////////////////////////////////////////////////////

#pragma once
#ifndef ESPTelnet_h
#define ESPTelnet_h

/////////////////////////////////////////////////////////////////

#if defined(TELNET_USE_ETHERNET)
# include "ETHTelnetBase.h"
# define Telnet ETHTelnet
#else
# include "ESPTelnetBase.h"
# define Telnet ESPTelnet
#endif

#ifdef TNetwork

/////////////////////////////////////////////////////////////////

class Telnet : public TelnetBaseClass {
 public:
  using TelnetBaseClass::TelnetBaseClass;

  
  template<typename T>
  void print(const T& data) {
    if (client && isConnected()) {
      if (!client.print(data)) {
        onFailedWrite();
      } else {
        onSuccessfullyWrite();
      }
    }
  }

  template<typename T>
  void println(const T& data) {
    if (client && isConnected()) {
      if (!client.println(data)) {
        onFailedWrite();
      } else {
        onSuccessfullyWrite();
      }
    }
  }

  template<typename T>
  void print(const T& data, int base) {
    if (client && isConnected()) {
      if (!client.print(data, base)) {
        onFailedWrite();
      } else {
        onSuccessfullyWrite();
      }
    }
  }

  template<typename T>
  void println(const T& data, int base) {
    if (client && isConnected()) {
      if (!client.println(data, base)) {
        onFailedWrite();
      } else {
        onSuccessfullyWrite();
      }
    }
  }

  void println();
  size_t printf(const char *format, ...);
  size_t vprintf(const char *format, va_list);

  bool isLineModeSet();
  void setLineMode(bool value = true);

  char getNewlineCharacter();
  void setNewlineCharacter(char value = '\n');

  protected:
  bool _lineMode = true;
  char _newlineCharacter = '\n';

  void handleInput();
};

/////////////////////////////////////////////////////////////////

// << operator
template <class T>
inline Telnet &operator<<(Telnet &obj, T arg) {
  obj.print(arg);
  return obj;
}

#endif  /*TNetwork*/

/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
