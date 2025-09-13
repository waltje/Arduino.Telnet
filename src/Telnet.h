#ifndef Telnet_h
# define Telnet_h

# if defined(TELNET_USE_ETHERNET)
#  include "ETHTelnetBase.h"
#  define Telnet ETHTelnet
# else
#  include "TelnetBase.h"
# endif

# ifdef TNetwork


class Telnet : public TelnetBaseClass {
  public:
    using TelnetBaseClass::TelnetBaseClass;
  
    template<typename T>
    void print(const T& data) {
      if (_client && isConnected()) {
        if (! _client.print(data)) {
          onFailedWrite();
        } else {
          onSuccessfullyWrite();
        }
      }
    }

    template<typename T>
    void println(const T& data) {
      if (_client && isConnected()) {
        if (! _client.println(data)) {
          onFailedWrite();
        } else {
          onSuccessfullyWrite();
        }
      }
    }

    template<typename T>
    void print(const T& data, int base) {
      if (_client && isConnected()) {
        if (! _client.print(data, base)) {
          onFailedWrite();
        } else {
          onSuccessfullyWrite();
        }
      }
    }

    template<typename T>
    void println(const T& data, int base) {
      if (_client && isConnected()) {
        if (! _client.println(data, base)) {
          onFailedWrite();
        } else {
          onSuccessfullyWrite();
        }
      }
    }

    void println(void);
    size_t printf(const char *format, ...);
    size_t vprintf(const char *format, va_list);

    bool isLineModeSet(void);
    void setLineMode(bool value = true);

    char getNewlineCharacter(void);
    void setNewlineCharacter(char value = '\n');

  protected:
    void handleInput(void);

    bool _lineMode = true;
    char _newlineCharacter = '\n';
};


// << operator
template <class T>
inline Telnet &operator<<(Telnet &obj, T arg) {
  obj.print(arg);
  return obj;
}


# endif /*TNetwork*/

#endif  /*Telnet_h*/
