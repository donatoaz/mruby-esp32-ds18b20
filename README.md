mruby-esp32-ds18b20
============

1-wire temperature sensor library for mruby and ESP32 -- for one single sensor
on the 1-wire bus.

## Installation
Add the line below to your `build_config.rb`:

```ruby
  conf.gem :github => 'donatoaz/mruby-esp32-ds18b20'
```

## Example
```ruby

pin_number = 14
sensor = ESP32::DS18B20::Sensor.new(pin_number)

# Delay a few seconds before polling the sensor.
ESP32::System.delay(3000)

loop {
  puts sensor.get_temp()
  ESP32::System.delay(10 * 1000)
}
```

## Schematic

Remember to add 4k7 ohm resistor between data pin and vcc.

![ds18b20 pinout](http://www.14core.com/wp-content/uploads/2015/11/Temperature-Sensor-Dallas-Pinout-Diagram.jpg)

## License

This work is mainly based on [YAMAMOTO Masaya](https://github.com/pandax381)'s 
work on [mruby-esp32](https://github.com/mruby-esp32/mruby-esp32).

The C library for accessing the
[ds18b20](https://github.com/feelfreelinux/ds18b20) is based on
[Filip](https://github.com/feelfreelinux)'s work.

Copyright (c) 2018 Donato Azevedo.

MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
