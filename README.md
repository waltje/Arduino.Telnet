# Telnet

Library allowing you to set up a Telnet server on your Arduino.

* Author: Fred N. van Kempen [https://github.com/waltje](https://github.con/waltje)
* Copyright 2025 Fred N. van Kempen.
* Author: Lennart Hennigs [https://www.lennarthennigs.de](https://www.lennarthennigs.de)
* Copyright 2018-2025 Lennart Hennigs.
* Released under the MIT license.

## Description

Use this library to set up a Telnet server to communicate status messages or debug / error log output. This is especially useful when you don't have a serial connection to the Arduino.

Although originally developed for the ESP8266 and ESP32 architectures (with their built-in WiFi support, hence the original name of ESP_Telnet), it is now generalized for all architectures, and will support Ethernet (if the ETH.. files are included), WiFi if explicitly requested, and will default to WiFi if compiled for the ESP8266 and ESP32 platforms.

To see the latest changes to the library please take a look at the [Changelog](https://github.com/waltje/Arduino.Telnet/blob/master/CHANGELOG.md).

## How To Use

### Initialization

* For Ethernet support, include either `ETHTelnet.h` or `ETHTelnetStream.h` and the library will then be compiled for Ethernet mode.  Currently, EthernetNG is required, but it will soon also allow for Ethernet3. For WiFi mode, you can simply include the `Telnet.h` or `TelnetStream.h` files, and WiFi mode will be set up.

### Starting / Stopping

* Use the `begin()` and `stop()` to start or stop the Telnet server.
* It needs an active Ethernet connection or a WiFi connection (in Client mode or in softAP mode.)
* You can set a custom port (other than 23) via the `begin()` function.
* If you don't want the client to check for a connection (i.e. you use an Ethernet adapter) you can bypass the WIFI connection check with the second parameter `begin(23, false)`.
* The Telnet server only allows you to connect a single client to it.
* You can use `getIP()` to get the connected client's IP address.
* You can manually disconnect the client via `disconnectClient()`.
* The server detects whether a client has disconnected. It checks periodically (default: every 1000ms).
* You can define the interval to check via `setKeepAliveInterval(int ms)`.

### Callback Handlers

* The library uses callback handlers to notify you of different Telnet events:
  * `void onConnect(CallbackFunction f);`
  * `void onConnectionAttempt(CallbackFunction f);`
  * `void onReconnect(CallbackFunction f);`
  * `void onDisconnect(CallbackFunction f);`
  * `void onInputReceived(CallbackFunction f);`

* All callback functions need a String parameter, it contains...
  * ...the input text for `onInputReceived()`
  * ...the IP from the connected client is send, otherwise.

### Output and Input

* Via `print()`, `printf()`, `vprintf()` and `println()` you can output text on the Telnet server.
* To receive and parse input from the Telnet client you can add a handler via `onInputReceived()`.
* By default, the library waits for a newline character from the client, and sends data to the callback handler one line at a time. This behavior can be deactivated by calling `setlineMode(false)`.
* A default newline character `'\n'` is used to determine the end of a line. This can be overridden by by calling `setNewlineCharacter('\r')` where `'\r'` can be swapped with any character.

### Using stream functions

* Alternatively, you can use the `Stream` implementation of Telnet.
* This does not provide `print()` or `println()` functions, see [TelnetStreamExample](https://github.com/waltje/Arduino.Telnet/blob/master/examples/TelnetStreamExample/TelnetStreamExample.ino) for more details.
* You'll also find the class definition below.

### Using ANSI Escape Sequences

* Please see [EscapeCodes.h](https://github.com/waltje/Arduino.Telnet/blob/master/src/EscapeCodes.h) for a list of constants and functions and take a look at the [AnsiExample](https://github.com/waltje/Arduino.Telnet/blob/master/examples/AnsiExample/AnsiExample.ino).
* The functions of this class return Strings with ANSI escape sequences. Send these to the Telnet client:

  ``` c++
  telnet.print(ansi.cls());
  telnet.print(ansi.home());
  telnet.print(ansi.setFG(ANSI_BRIGHT_WHITE));
  telnet.println("\nWelcome " + telnet.getIP());
  telnet.println("(Use ^] + q  to disconnect.)");
  telnet.print(ansi.reset());
  ```

### The Loop

* For the class to work, you need to call the `loop()` member function in your sketch's `loop()` function.

## Examples

* [TelnetServerExample](https://github.com/waltje/Arduino.Telnet/blob/master/examples/TelnetServerExample/TelnetServerExample.ino) – basic example
* [TelnetServerExampleWithWiFiManager](https://github.com/waltjes/Arduino.Telnet/blob/master/examples/TelnetServerWithWiFiManager/TelnetServerWithWiFiManager.ino) – basic example using [WiFiManager](https://github.com/tzapu/WiFiManager) (use it for ESP8266s)
* [TelnetServerWithAutoconnect](https://github.com/waltje/Arduino.Telnet/blob/master/examples/TelnetServerWithAutoconnect/TelnetServerWithAutoconnect.ino) – basic example using [Autoconnect](https://github.com/Hieromon/AutoConnect) (use it for ESP266 or ESP32)
* [TelnetStreamExample](https://github.com/waltje/Arduino.Telnet/blob/master/examples/TelnetStreamExample/TelnetStreamExample.ino) - stream example
* [AnsiExample](https://github.com/waltje/Arduino.Telnet/blob/master/examples/AnsiExample/AnsiExample.ino) - sending ANSI escape codes

## Notes

* To see the latest changes to the library please take a look at the [Changelog](https://github.com/waltje/Arduino.Telnet/blob/master/CHANGELOG.md).

* If you find this library helpful, please consider giving it a star at [GitHub](https://github.com/waltje/Arduino.Telnet). Thanks!

## Class Definition

These are the constructors and the member functions the library provides:

### Telnet Definition

``` c++
    Telnet();

    bool begin(uint16_t port = 23, bool checkConnection = true);
    void loop(void);
    void stop(void);

    void print(const String &str);
    void println(const String &str);
    void print(const char c);
    void println(const char c);
    void print(unsigned char b, int base);
    void println(unsigned char b, int base);
    void print(int n, int base);
    void println(int n, int base);
    void print(unsigned int n, int base);
    void println(unsigned int n, int base);
    void print(const Printable& x);
    void println(const Printable& x);
    void println(void);
    size_t printf(const char *format, ...);
    size_t vprintf(const char *format, va_list);

    String getIP(void) const;
    String getLastAttemptIP(void) const;

    bool isConnected(void);
    void setKeepAliveInterval(int ms);
    int getKeepAliveInterval(void);

    bool isLineModeSet(void);
    void setLineMode(bool value = true);

    char getNewlineCharacter(void);
    void setNewlineCharacter(char value = '\n');

    void onConnect(CallbackFunction f);
    void onConnectionAttempt(CallbackFunction f);
    void onReconnect(CallbackFunction f);
    void onDisconnect(CallbackFunction f);
    void onInputReceived(CallbackFunction f);

    void disconnectClient(bool triggerEvent = true);
```

### TelnetStream Definition

``` c++
    TelnetStream();

    bool begin(uint16_t port = 23, bool checkConnection = true);
    void loop(void);
    void stop(void);

    int available(void);
    int read(void);
    int peek(void);
    void flush(void);
    size_t write(uint8_t c);
    size_t write(const uint8_t *data, size_t size);

    String getIP(void) const;
    String getLastAttemptIP(void) const;

    bool isConnected(void);
    void setKeepAliveInterval(int ms);
    int getKeepAliveInterval(void);

    void onConnect(CallbackFunction f);
    void onConnectionAttempt(CallbackFunction f);
    void onReconnect(CallbackFunction f);
    void onDisconnect(CallbackFunction f);
    void onInputReceived(CallbackFunction f);

    void disconnectClient(bool triggerEvent = true);
```

## Installation

Open the Arduino IDE choose "Sketch > Include Library" and search for "Telnet".
You could also download the [ZIP archive](https://github.com/waltje/Arduino.Telnet/zipball/master), and choose "Sketch > Include Library > Add .ZIP Library..." and select the downloaded file.

The "Telnet" library is also available on the PlatformIO registry and can be included by adding the following line to the leb_deps option of the platformio.ini file:

``` json
   lennarthennigs/ESP Telnet@^2.2.2
```

## License

MIT License

Copyright 2025 Fred N. van Kempen

Copyright 2018-2025 Lennart Hennigs

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
