#ifndef TelnetStream_h
# define TelnetStream_h

#if defined(TELNET_USE_ETHERNET)
# include "ETHTelnetBase.h"
# define TelnetStream ETHTelnetStream
#else
# include "TelnetBase.h"
#endif

# ifdef TNetwork


class TelnetStream : public TelnetBaseClass, public Stream {
  public:
    using TelnetBaseClass::TelnetBaseClass;

    int available(void);
    int read(void);
    int peek(void);
    void flush(void) override;

    size_t write(uint8_t) override;
    size_t write(const uint8_t* data, size_t size) override;

  protected:
    void handleInput(void);
};


// << operator
//  template<class T> inline TelnetStream &operator <<(TelnetStream &obj, T arg) { obj.print(arg); return obj; }


#endif  /*TNetwork*/

#endif  /*TelnetStream_h*/
