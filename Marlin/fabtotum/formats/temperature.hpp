  void print_heater_state(const float &c, const float &t,
    #if ENABLED(SHOW_TEMP_ADC_VALUES)
      const float r,
    #endif
    const int8_t e=-2,
    const bool use_idx=0,
    const bool short_format=0
  ) {
    SERIAL_PROTOCOLCHAR(' ');
    SERIAL_PROTOCOLCHAR(
      #if HAS_TEMP_BED && HAS_TEMP_HOTEND
        e == -1 ? 'B' : 'T'
      #elif HAS_TEMP_HOTEND
        'T'
      #else
        'B'
      #endif
    );
    if(use_idx) {
      if (e >= 0) SERIAL_PROTOCOLCHAR('0' + e);
    }
    SERIAL_PROTOCOLPGM(": ");
    SERIAL_PROTOCOL(c);
    SERIAL_PROTOCOLPAIR("/" , t);
    #if ENABLED(SHOW_TEMP_ADC_VALUES)
      SERIAL_PROTOCOLPAIR(" (", r / OVERSAMPLENR);
      SERIAL_PROTOCOLCHAR(')');
    #endif
  }

  void print_heaterstates(const bool short_format) {
    #if HAS_TEMP_HOTEND
      print_heater_state(thermalManager.degHotend(target_extruder), thermalManager.degTargetHotend(target_extruder),
        #if ENABLED(SHOW_TEMP_ADC_VALUES)
          thermalManager.rawHotendTemp(target_extruder),
        #endif
        0,
        short_format
      );
    #endif
    #if HAS_TEMP_BED
      print_heater_state(thermalManager.degBed(), thermalManager.degTargetBed(),
        #if ENABLED(SHOW_TEMP_ADC_VALUES)
          thermalManager.rawBedTemp(),
        #endif
        -1, // BED
        short_format
      );
    #endif
    if(!short_format)
    {
      #if HOTENDS > 0
        HOTEND_LOOP() print_heater_state(thermalManager.degHotend(e), thermalManager.degTargetHotend(e),
          #if ENABLED(SHOW_TEMP_ADC_VALUES)
            thermalManager.rawHotendTemp(e),
          #endif
          e,
          1 /* append index */
        );
      #endif
      SERIAL_PROTOCOLPGM(" @: ");
      SERIAL_PROTOCOL(thermalManager.getHeaterPower(target_extruder));
      #if HAS_TEMP_BED
        SERIAL_PROTOCOLPGM(" B@: ");
        SERIAL_PROTOCOL(thermalManager.getHeaterPower(-1));
      #endif
      #if HOTENDS > 1
        HOTEND_LOOP() {
          SERIAL_PROTOCOLPAIR(" @", e);
          SERIAL_PROTOCOLPGM(": ");
          SERIAL_PROTOCOL(thermalManager.getHeaterPower(e));
        }
      #endif
    }
  }
