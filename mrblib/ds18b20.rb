module ESP32
  module DS18B20
    class Sensor
      attr_reader :pin
      def initialize(pin)
        @pin = pin
      end
  
      def init
        DS18B20.ds18b20_init pin
      end

      def get_temp
        DS18B20.ds18b20_get_temp
      end
    end
  end
end
