# hwgrok
This utility distills the topo snapshot down into a higher-level representation of the hardware configuration and emits that as a single JSON object, which in turn could be consumed by higher-level management software.

Sample output:

```
# ./hwgrok | json
{
  "chassis": {
    "hc-fmri": "hc:///chassis=0",
    "manufacturer": "unknown",
    "model": "unknown",
    "sensors": [
      {
        "name": "System Temp",
        "type": "temperature",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 35,
        "units": "DEGREES_C",
        "threshold-lower-non-critical": 10,
        "threshold-lower-critical": 5,
        "threshold-lower-non-recoverable": 5,
        "threshold-upper-non-critical": 80,
        "threshold-upper-critical": 85,
        "threshold-upper-non-recoverable": 90
      },
      {
        "name": "Chassis Intru",
        "type": "PHYSICAL",
        "state": 1,
        "state-description": "GENERAL"
      }
    ],
    "leds": [
      {
        "type": "LOCATE",
        "mode": "off"
      }
    ]
  },
  "service-processor": {
    "hc-fmri": "hc:///motherboard=0/sp=0",
    "firmware-revision": "1.40",
    "mac-address": "ac:1f:6b:61:53:29",
    "ipv4-address": "172.20.1.28",
    "ipv4-subnet": "255.255.254.0",
    "ipv4-gateway": "172.20.0.1",
    "ipv4-config-type": "dhcp"
  },
  "motherboard": {
    "hc-fmri": "hc:///motherboard=0",
    "manufacturer": "Supermicro",
    "model": "X11DPH-T",
    "firmware-revision": "1.01",
    "sensors": [
      {
        "name": "PCH Temp",
        "type": "temperature",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 59,
        "units": "DEGREES_C",
        "threshold-lower-non-critical": 10,
        "threshold-lower-critical": 5,
        "threshold-lower-non-recoverable": 5,
        "threshold-upper-non-critical": 85,
        "threshold-upper-critical": 90,
        "threshold-upper-non-recoverable": 105
      },
      {
        "name": "Peripheral Temp",
        "type": "temperature",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 52,
        "units": "DEGREES_C",
        "threshold-lower-non-critical": 10,
        "threshold-lower-critical": 5,
        "threshold-lower-non-recoverable": 5,
        "threshold-upper-non-critical": 80,
        "threshold-upper-critical": 85,
        "threshold-upper-non-recoverable": 90
      },
      {
        "name": "MB_10G Temp",
        "type": "temperature",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 57,
        "units": "DEGREES_C",
        "threshold-lower-non-critical": 10,
        "threshold-lower-critical": 5,
        "threshold-lower-non-recoverable": 5,
        "threshold-upper-non-critical": 95,
        "threshold-upper-critical": 100,
        "threshold-upper-non-recoverable": 105
      },
      {
        "name": "VRMCpu1SA Temp",
        "type": "temperature",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 45,
        "units": "DEGREES_C",
        "threshold-lower-non-critical": 10,
        "threshold-lower-critical": 5,
        "threshold-lower-non-recoverable": 5,
        "threshold-upper-non-critical": 95,
        "threshold-upper-critical": 100,
        "threshold-upper-non-recoverable": 105
      },
      {
        "name": "VRMCpu1IO Temp",
        "type": "temperature",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 52,
        "units": "DEGREES_C",
        "threshold-lower-non-critical": 10,
        "threshold-lower-critical": 5,
        "threshold-lower-non-recoverable": 5,
        "threshold-upper-non-critical": 95,
        "threshold-upper-critical": 100,
        "threshold-upper-non-recoverable": 105
      },
      {
        "name": "VRMCpu1CCH Temp",
        "type": "temperature",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 42,
        "units": "DEGREES_C",
        "threshold-lower-non-critical": 10,
        "threshold-lower-critical": 5,
        "threshold-lower-non-recoverable": 5,
        "threshold-upper-non-critical": 95,
        "threshold-upper-critical": 100,
        "threshold-upper-non-recoverable": 105
      },
      {
        "name": "VRMCpu2IO Temp",
        "type": "temperature",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 46,
        "units": "DEGREES_C",
        "threshold-lower-non-critical": 10,
        "threshold-lower-critical": 5,
        "threshold-lower-non-recoverable": 5,
        "threshold-upper-non-critical": 95,
        "threshold-upper-critical": 100,
        "threshold-upper-non-recoverable": 105
      },
      {
        "name": "VRMP1ABC Temp",
        "type": "temperature",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 49,
        "units": "DEGREES_C",
        "threshold-lower-non-critical": 10,
        "threshold-lower-critical": 5,
        "threshold-lower-non-recoverable": 5,
        "threshold-upper-non-critical": 95,
        "threshold-upper-critical": 100,
        "threshold-upper-non-recoverable": 105
      },
      {
        "name": "VRMP1DEF Temp",
        "type": "temperature",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 38,
        "units": "DEGREES_C",
        "threshold-lower-non-critical": 10,
        "threshold-lower-critical": 5,
        "threshold-lower-non-recoverable": 5,
        "threshold-upper-non-critical": 95,
        "threshold-upper-critical": 100,
        "threshold-upper-non-recoverable": 105
      },
      {
        "name": "VRMP2ABC Temp",
        "type": "temperature",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 49,
        "units": "DEGREES_C",
        "threshold-lower-non-critical": 10,
        "threshold-lower-critical": 5,
        "threshold-lower-non-recoverable": 5,
        "threshold-upper-non-critical": 95,
        "threshold-upper-critical": 100,
        "threshold-upper-non-recoverable": 105
      },
      {
        "name": "VRMP2DEF Temp",
        "type": "temperature",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 43,
        "units": "DEGREES_C",
        "threshold-lower-non-critical": 10,
        "threshold-lower-critical": 5,
        "threshold-lower-non-recoverable": 5,
        "threshold-upper-non-critical": 95,
        "threshold-upper-critical": 100,
        "threshold-upper-non-recoverable": 105
      },
      {
        "name": "12V",
        "type": "voltage",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 11.941,
        "units": "VOLTS",
        "threshold-lower-non-critical": 10.761,
        "threshold-lower-critical": 10.289,
        "threshold-lower-non-recoverable": 10.171,
        "threshold-upper-non-critical": 12.944,
        "threshold-upper-critical": 13.298,
        "threshold-upper-non-recoverable": 13.416
      },
      {
        "name": "5VCC",
        "type": "voltage",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 5.18,
        "units": "VOLTS",
        "threshold-lower-non-critical": 4.49,
        "threshold-lower-critical": 4.28,
        "threshold-lower-non-recoverable": 4.25,
        "threshold-upper-non-critical": 5.39,
        "threshold-upper-critical": 5.54,
        "threshold-upper-non-recoverable": 5.6
      },
      {
        "name": "3.3VCC",
        "type": "voltage",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 3.418,
        "units": "VOLTS",
        "threshold-lower-non-critical": 2.959,
        "threshold-lower-critical": 2.823,
        "threshold-lower-non-recoverable": 2.789,
        "threshold-upper-non-critical": 3.554,
        "threshold-upper-critical": 3.656,
        "threshold-upper-non-recoverable": 3.69
      },
      {
        "name": "VBAT",
        "type": "BATTERY",
        "state": 32516,
        "state-description": "PRESENCE"
      },
      {
        "name": "Vcpu1",
        "type": "voltage",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 1.872,
        "units": "VOLTS",
        "threshold-lower-non-critical": 1.404,
        "threshold-lower-critical": 1.26,
        "threshold-lower-non-recoverable": 1.242,
        "threshold-upper-non-critical": 1.899,
        "threshold-upper-critical": 2.079,
        "threshold-upper-non-recoverable": 2.106
      },
      {
        "name": "Vcpu2",
        "type": "voltage",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 1.881,
        "units": "VOLTS",
        "threshold-lower-non-critical": 1.404,
        "threshold-lower-critical": 1.26,
        "threshold-lower-non-recoverable": 1.242,
        "threshold-upper-non-critical": 1.899,
        "threshold-upper-critical": 2.079,
        "threshold-upper-non-recoverable": 2.106
      },
      {
        "name": "5VSB",
        "type": "voltage",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 5.279,
        "units": "VOLTS",
        "threshold-lower-non-critical": 4.473,
        "threshold-lower-critical": 4.38,
        "threshold-lower-non-recoverable": 4.225,
        "threshold-upper-non-critical": 5.372,
        "threshold-upper-critical": 5.527,
        "threshold-upper-non-recoverable": 5.589
      },
      {
        "name": "3.3VSB",
        "type": "voltage",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 3.486,
        "units": "VOLTS",
        "threshold-lower-non-critical": 2.959,
        "threshold-lower-critical": 2.891,
        "threshold-lower-non-recoverable": 2.789,
        "threshold-upper-non-critical": 3.554,
        "threshold-upper-critical": 3.656,
        "threshold-upper-non-recoverable": 3.69
      },
      {
        "name": "1.8V PCH",
        "type": "voltage",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 1.854,
        "units": "VOLTS",
        "threshold-lower-non-critical": 1.62,
        "threshold-lower-critical": 1.584,
        "threshold-lower-non-recoverable": 1.53,
        "threshold-upper-non-critical": 1.944,
        "threshold-upper-critical": 1.998,
        "threshold-upper-non-recoverable": 2.016
      },
      {
        "name": "PVNN PCH",
        "type": "voltage",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 1.049,
        "units": "VOLTS",
        "threshold-lower-non-critical": 0.895,
        "threshold-lower-critical": 0.846,
        "threshold-lower-non-recoverable": 0.825,
        "threshold-upper-non-critical": 1.147,
        "threshold-upper-critical": 1.175,
        "threshold-upper-non-recoverable": 1.196
      },
      {
        "name": "1.05V PCH",
        "type": "voltage",
        "state": 0,
        "state-description": "NO_STATES_ASSERTED",
        "reading": 1.071,
        "units": "VOLTS",
        "threshold-lower-non-critical": 0.945,
        "threshold-lower-critical": 0.896,
        "threshold-lower-non-recoverable": 0.875,
        "threshold-upper-non-critical": 1.197,
        "threshold-upper-critical": 1.225,
        "threshold-upper-non-recoverable": 1.246
      }
    ],
    "leds": []
  },
  "processors": [
    {
      "label": "unknown",
      "hc-fmri": "hc:///motherboard=0/chip=0",
      "manufacturer": "Intel",
      "model": "unknown",
      "processor-brand": "Intel(r) Xeon(r) Gold 6132 CPU @ 2.60GHz",
      "processor-family": "6",
      "processor-model": "85",
      "processor-stepping": "4",
      "number-of-cores": 14,
      "number-of-threads-per-core": 2,
      "sensors": [],
      "leds": []
    },
    {
      "label": "unknown",
      "hc-fmri": "hc:///motherboard=0/chip=1",
      "manufacturer": "Intel",
      "model": "unknown",
      "processor-brand": "Intel(r) Xeon(r) Gold 6132 CPU @ 2.60GHz",
      "processor-family": "6",
      "processor-model": "85",
      "processor-stepping": "4",
      "number-of-cores": 14,
      "number-of-threads-per-core": 2,
      "sensors": [],
      "leds": []
    }
  ],
  "memory": [
    {
      "label": "P1-DIMMA1",
      "hc-fmri": "hc:///motherboard=0/slot=0",
      "dimm": {
        "hc-fmri": "hc:///motherboard=0/slot=0/dimm=0",
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "362BC882",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P1-DIMMA1 Temp",
            "type": "temperature",
            "state": 0,
            "state-description": "NO_STATES_ASSERTED",
            "reading": 36,
            "units": "DEGREES_C",
            "threshold-lower-non-critical": 10,
            "threshold-lower-critical": 5,
            "threshold-lower-non-recoverable": 5,
            "threshold-upper-non-critical": 80,
            "threshold-upper-critical": 85,
            "threshold-upper-non-recoverable": 90
          }
        ],
        "leds": []
      }
    },
    {
      "label": "P1-DIMMA2",
      "hc-fmri": "hc:///motherboard=0/slot=1",
      "dimm": {
        "hc-fmri": "hc:///motherboard=0/slot=1/dimm=0",
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "36246BC4",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P1-DIMMA2 Temp",
            "type": "temperature",
            "state": 0,
            "state-description": "NO_STATES_ASSERTED",
            "reading": 37,
            "units": "DEGREES_C",
            "threshold-lower-non-critical": 10,
            "threshold-lower-critical": 5,
            "threshold-lower-non-recoverable": 5,
            "threshold-upper-non-critical": 80,
            "threshold-upper-critical": 85,
            "threshold-upper-non-recoverable": 90
          }
        ],
        "leds": []
      }
    },
    {
      "label": "P1-DIMMB1",
      "hc-fmri": "hc:///motherboard=0/slot=2",
      "dimm": {
        "hc-fmri": "hc:///motherboard=0/slot=2/dimm=0",
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "36246309",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P1-DIMMB1 Temp",
            "type": "temperature",
            "state": 0,
            "state-description": "NO_STATES_ASSERTED",
            "reading": 37,
            "units": "DEGREES_C",
            "threshold-lower-non-critical": 10,
            "threshold-lower-critical": 5,
            "threshold-lower-non-recoverable": 5,
            "threshold-upper-non-critical": 80,
            "threshold-upper-critical": 85,
            "threshold-upper-non-recoverable": 90
          }
        ],
        "leds": []
      }
    },
    {
      "label": "P1-DIMMC1",
      "hc-fmri": "hc:///motherboard=0/slot=3",
      "dimm": {
        "hc-fmri": "hc:///motherboard=0/slot=3/dimm=0",
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "36246BC0",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P1-DIMMC1 Temp",
            "type": "temperature",
            "state": 0,
            "state-description": "NO_STATES_ASSERTED",
            "reading": 38,
            "units": "DEGREES_C",
            "threshold-lower-non-critical": 10,
            "threshold-lower-critical": 5,
            "threshold-lower-non-recoverable": 5,
            "threshold-upper-non-critical": 80,
            "threshold-upper-critical": 85,
            "threshold-upper-non-recoverable": 90
          }
        ],
        "leds": []
      }
    },
    {
      "label": "P1-DIMMD1",
      "hc-fmri": "hc:///motherboard=0/slot=4",
      "dimm": {
        "hc-fmri": "hc:///motherboard=0/slot=4/dimm=0",
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "362BBB9F",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P1-DIMMD1 Temp",
            "type": "temperature",
            "state": 0,
            "state-description": "NO_STATES_ASSERTED",
            "reading": 44,
            "units": "DEGREES_C",
            "threshold-lower-non-critical": 10,
            "threshold-lower-critical": 5,
            "threshold-lower-non-recoverable": 5,
            "threshold-upper-non-critical": 80,
            "threshold-upper-critical": 85,
            "threshold-upper-non-recoverable": 90
          }
        ],
        "leds": []
      }
    },
    {
      "label": "P1-DIMMD2",
      "hc-fmri": "hc:///motherboard=0/slot=5",
      "dimm": {
        "hc-fmri": "hc:///motherboard=0/slot=5/dimm=0",
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "362BC781",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P1-DIMMD2 Temp",
            "type": "temperature",
            "state": 0,
            "state-description": "NO_STATES_ASSERTED",
            "reading": 41,
            "units": "DEGREES_C",
            "threshold-lower-non-critical": 10,
            "threshold-lower-critical": 5,
            "threshold-lower-non-recoverable": 5,
            "threshold-upper-non-critical": 80,
            "threshold-upper-critical": 85,
            "threshold-upper-non-recoverable": 90
          }
        ],
        "leds": []
      }
    },
    {
      "label": "P1-DIMME1",
      "hc-fmri": "hc:///motherboard=0/slot=6",
      "dimm": {
        "hc-fmri": "hc:///motherboard=0/slot=6/dimm=0",
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "362468CC",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P1-DIMME1 Temp",
            "type": "temperature",
            "state": 0,
            "state-description": "NO_STATES_ASSERTED",
            "reading": 48,
            "units": "DEGREES_C",
            "threshold-lower-non-critical": 10,
            "threshold-lower-critical": 5,
            "threshold-lower-non-recoverable": 5,
            "threshold-upper-non-critical": 80,
            "threshold-upper-critical": 85,
            "threshold-upper-non-recoverable": 90
          }
        ],
        "leds": []
      }
    },
    {
      "label": "P1-DIMMF1",
      "hc-fmri": "hc:///motherboard=0/slot=7",
      "dimm": {
        "hc-fmri": "hc:///motherboard=0/slot=7/dimm=0",
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "362BDCBA",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P1-DIMMF1 Temp",
            "type": "temperature",
            "state": 0,
            "state-description": "NO_STATES_ASSERTED",
            "reading": 48,
            "units": "DEGREES_C",
            "threshold-lower-non-critical": 10,
            "threshold-lower-critical": 5,
            "threshold-lower-non-recoverable": 5,
            "threshold-upper-non-critical": 80,
            "threshold-upper-critical": 85,
            "threshold-upper-non-recoverable": 90
          }
        ],
        "leds": []
      }
    },
    {
      "label": "P2-DIMMA1",
      "hc-fmri": "hc:///motherboard=0/slot=8",
      "dimm": {
        "hc-fmri": "hc:///motherboard=0/slot=8/dimm=0",
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "35808F99",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P2-DIMMA1 Temp",
            "type": "temperature",
            "state": 0,
            "state-description": "NO_STATES_ASSERTED",
            "reading": 48,
            "units": "DEGREES_C",
            "threshold-lower-non-critical": 10,
            "threshold-lower-critical": 5,
            "threshold-lower-non-recoverable": 5,
            "threshold-upper-non-critical": 80,
            "threshold-upper-critical": 85,
            "threshold-upper-non-recoverable": 90
          }
        ],
        "leds": []
      }
    },
    {
      "label": "P2-DIMMA2",
      "hc-fmri": "hc:///motherboard=0/slot=9",
      "dimm": {
        "hc-fmri": "hc:///motherboard=0/slot=9/dimm=0",
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "34EE8887",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P2-DIMMA2 Temp",
            "type": "temperature",
            "state": 0,
            "state-description": "NO_STATES_ASSERTED",
            "reading": 48,
            "units": "DEGREES_C",
            "threshold-lower-non-critical": 10,
            "threshold-lower-critical": 5,
            "threshold-lower-non-recoverable": 5,
            "threshold-upper-non-critical": 80,
            "threshold-upper-critical": 85,
            "threshold-upper-non-recoverable": 90
          }
        ],
        "leds": []
      }
    },
    {
      "label": "P2-DIMMB1",
      "hc-fmri": "hc:///motherboard=0/slot=10",
      "dimm": {
        "hc-fmri": "hc:///motherboard=0/slot=10/dimm=0",
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "35808FF1",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P2-DIMMB1 Temp",
            "type": "temperature",
            "state": 0,
            "state-description": "NO_STATES_ASSERTED",
            "reading": 49,
            "units": "DEGREES_C",
            "threshold-lower-non-critical": 10,
            "threshold-lower-critical": 5,
            "threshold-lower-non-recoverable": 5,
            "threshold-upper-non-critical": 80,
            "threshold-upper-critical": 85,
            "threshold-upper-non-recoverable": 90
          }
        ],
        "leds": []
      }
    },
    {
      "label": "P2-DIMMC1",
      "hc-fmri": "hc:///motherboard=0/slot=11",
      "dimm": {
        "hc-fmri": "hc:///motherboard=0/slot=11/dimm=0",
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "34EE7E63",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P2-DIMMC1 Temp",
            "type": "temperature",
            "state": 0,
            "state-description": "NO_STATES_ASSERTED",
            "reading": 47,
            "units": "DEGREES_C",
            "threshold-lower-non-critical": 10,
            "threshold-lower-critical": 5,
            "threshold-lower-non-recoverable": 5,
            "threshold-upper-non-critical": 80,
            "threshold-upper-critical": 85,
            "threshold-upper-non-recoverable": 90
          }
        ],
        "leds": []
      }
    },
    {
      "label": "P2-DIMMD1",
      "hc-fmri": "hc:///motherboard=0/slot=12",
      "dimm": {
        "hc-fmri": "hc:///motherboard=0/slot=12/dimm=0",
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "34EE88C1",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P2-DIMMD1 Temp",
            "type": "temperature",
            "state": 0,
            "state-description": "NO_STATES_ASSERTED",
            "reading": 41,
            "units": "DEGREES_C",
            "threshold-lower-non-critical": 10,
            "threshold-lower-critical": 5,
            "threshold-lower-non-recoverable": 5,
            "threshold-upper-non-critical": 80,
            "threshold-upper-critical": 85,
            "threshold-upper-non-recoverable": 90
          }
        ],
        "leds": []
      }
    },
    {
      "label": "P2-DIMMD2",
      "hc-fmri": "hc:///motherboard=0/slot=13",
      "dimm": {
        "hc-fmri": "hc:///motherboard=0/slot=13/dimm=0",
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "34EE8821",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P2-DIMMD2 Temp",
            "type": "temperature",
            "state": 0,
            "state-description": "NO_STATES_ASSERTED",
            "reading": 42,
            "units": "DEGREES_C",
            "threshold-lower-non-critical": 10,
            "threshold-lower-critical": 5,
            "threshold-lower-non-recoverable": 5,
            "threshold-upper-non-critical": 80,
            "threshold-upper-critical": 85,
            "threshold-upper-non-recoverable": 90
          }
        ],
        "leds": []
      }
    },
    {
      "label": "P2-DIMME1",
      "hc-fmri": "hc:///motherboard=0/slot=14",
      "dimm": {
        "hc-fmri": "hc:///motherboard=0/slot=14/dimm=0",
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "34EE8882",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P2-DIMME1 Temp",
            "type": "temperature",
            "state": 0,
            "state-description": "NO_STATES_ASSERTED",
            "reading": 40,
            "units": "DEGREES_C",
            "threshold-lower-non-critical": 10,
            "threshold-lower-critical": 5,
            "threshold-lower-non-recoverable": 5,
            "threshold-upper-non-critical": 80,
            "threshold-upper-critical": 85,
            "threshold-upper-non-recoverable": 90
          }
        ],
        "leds": []
      }
    },
    {
      "label": "P2-DIMMF1",
      "hc-fmri": "hc:///motherboard=0/slot=15",
      "dimm": {
        "hc-fmri": "hc:///motherboard=0/slot=15/dimm=0",
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "362BBD60",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P2-DIMMF1 Temp",
            "type": "temperature",
            "state": 0,
            "state-description": "NO_STATES_ASSERTED",
            "reading": 38,
            "units": "DEGREES_C",
            "threshold-lower-non-critical": 10,
            "threshold-lower-critical": 5,
            "threshold-lower-non-recoverable": 5,
            "threshold-upper-non-critical": 80,
            "threshold-upper-critical": 85,
            "threshold-upper-non-recoverable": 90
          }
        ],
        "leds": []
      }
    }
  ],
  "pci-devices": [
    {
      "label": "CPU1 SLOT 1 PCI-E 3.0 X8",
      "hc-fmri": "hc:///motherboard=0/hostbridge=1/pciexrc=1/pciexbus=2/pciexdev=0",
      "pci-vendor-name": "ASPEED Technology, Inc.",
      "pci-device-name": "AST1150 PCI-to-PCI Bridge",
      "pci-subsystem-name": "unknown",
      "device-driver-name": "pcieb",
      "device-driver-instance": 1,
      "device-path": "/pci@0,0/pci8086,a195@1c,5/pci1a03,1150@0"
    },
    {
      "label": "CPU1 SLOT 1 PCI-E 3.0 X8",
      "hc-fmri": "hc:///motherboard=0/hostbridge=1/pciexrc=1/pciexbus=2/pciexdev=0/pciexfn=0/pcibus=3/pcidev=0",
      "pci-vendor-name": "ASPEED Technology, Inc.",
      "pci-device-name": "ASPEED Graphics Family",
      "pci-subsystem-name": "unknown",
      "device-driver-name": "vgatext",
      "device-driver-instance": 0,
      "device-path": "/pci@0,0/pci8086,a195@1c,5/pci1a03,1150@0/display@0"
    },
    {
      "label": "CPU1 SLOT 1 PCI-E 3.0 X8",
      "hc-fmri": "hc:///motherboard=0/hostbridge=2/pciexrc=2/pciexbus=24/pciexdev=0",
      "pci-vendor-name": "Intel Corporation",
      "pci-device-name": "unknown",
      "pci-subsystem-name": "unknown",
      "device-driver-name": "pcieb",
      "device-driver-instance": 3,
      "device-path": "/pci@14,0/pci8086,2030@0/pci8086,37c0@0"
    },
    {
      "label": "CPU1 SLOT 1 PCI-E 3.0 X8",
      "hc-fmri": "hc:///motherboard=0/hostbridge=2/pciexrc=2/pciexbus=24/pciexdev=0/pciexfn=0/pciexbus=25/pciexdev=3",
      "pci-vendor-name": "Intel Corporation",
      "pci-device-name": "unknown",
      "pci-subsystem-name": "unknown",
      "device-driver-name": "pcieb",
      "device-driver-instance": 4,
      "device-path": "/pci@14,0/pci8086,2030@0/pci8086,37c0@0/pci8086,37c5@3"
    },
    {
      "label": "CPU1 SLOT 1 PCI-E 3.0 X8",
      "hc-fmri": "hc:///motherboard=0/hostbridge=2/pciexrc=2/pciexbus=24/pciexdev=0/pciexfn=0/pciexbus=25/pciexdev=3/pciexfn=0/pciexbus=26/pciexdev=0",
      "pci-vendor-name": "Intel Corporation",
      "pci-device-name": "Ethernet Connection X722 for 10GBASE-T",
      "pci-subsystem-name": "unknown",
      "device-driver-name": "i40e",
      "device-driver-instance": 1,
      "device-path": "/pci@14,0/pci8086,2030@0/pci8086,37c0@0/pci8086,37c5@3/pci15d9,37d2@0,1"
    },
    {
      "label": "CPU2 SLOT 4 PCI-E 3.0 X16",
      "hc-fmri": "hc:///motherboard=0/hostbridge=3/pciexrc=3/pciexbus=59/pciexdev=0",
      "pci-vendor-name": "Intel Corporation",
      "pci-device-name": "82599ES 10-Gigabit SFI/SFP+ Network Connection",
      "pci-subsystem-name": "Ethernet Server Adapter X520-2",
      "device-driver-name": "ixgbe",
      "device-driver-instance": 1,
      "device-path": "/pci@34,0/pci8086,2032@2/pci8086,c@0,1"
    },
    {
      "label": "CPU1 SLOT 7 PCI-E 3.0 X8",
      "hc-fmri": "hc:///motherboard=0/hostbridge=4/pciexrc=4/pciexbus=94/pciexdev=0",
      "pci-vendor-name": "Broadcom / LSI",
      "pci-device-name": "SAS3008 PCI-Express Fusion-MPT SAS-3",
      "pci-subsystem-name": "AOC-S3008L-L8e",
      "device-driver-name": "mpt_sas",
      "device-driver-instance": 0,
      "device-path": "/pci@56,0/pci8086,2030@0/pci15d9,808@0"
    },
    {
      "label": "CPU1 SLOT 3 PCI-E 3.0 X8",
      "hc-fmri": "hc:///motherboard=0/hostbridge=5/pciexrc=5/pciexbus=134/pciexdev=0",
      "pci-vendor-name": "Broadcom / LSI",
      "pci-device-name": "SAS3008 PCI-Express Fusion-MPT SAS-3",
      "pci-subsystem-name": "AOC-S3008L-L8e",
      "device-driver-name": "mpt_sas",
      "device-driver-instance": 2,
      "device-path": "/pci@7d,0/pci8086,2030@0/pci15d9,808@0"
    },
    {
      "label": "CPU2 SLOT 5 PCI-E 3.0 X16",
      "hc-fmri": "hc:///motherboard=0/hostbridge=6/pciexrc=6/pciexbus=216/pciexdev=0",
      "pci-vendor-name": "Broadcom / LSI",
      "pci-device-name": "SAS3008 PCI-Express Fusion-MPT SAS-3",
      "pci-subsystem-name": "AOC-S3008L-L8e",
      "device-driver-name": "mpt_sas",
      "device-driver-instance": 1,
      "device-path": "/pci@ce,0/pci8086,2030@0/pci15d9,808@0"
    }
  ],
  "drive-bays": [
    {
      "label": "Front Disk 0",
      "hc-fmri": "hc:///chassis=0/bay=0",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        },
        {
          "type": "OK2RM",
          "mode": "off"
        }
      ],
      "disk": {
        "hc-fmri": "hc:///chassis=0/bay=0/disk=0",
        "manufacturer": "HGST",
        "model": "HUSMH8010BSS204",
        "serial-number": "0HWZDBUA",
        "firmware-revision": "C360",
        "size-in-bytes": 1245995008,
        "device-path": "/pci@7d,0/pci8086,2030@0/pci15d9,808@0/iport@1/disk@w5000cca0496fa475,0",
        "logical-disk": "c3t5000CCA0496FA475d0",
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 29,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 1",
      "hc-fmri": "hc:///chassis=0/bay=1",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        },
        {
          "type": "OK2RM",
          "mode": "off"
        }
      ],
      "disk": {
        "hc-fmri": "hc:///chassis=0/bay=1/disk=0",
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08V1ERBH",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "device-path": "/pci@7d,0/pci8086,2030@0/pci15d9,808@0/iport@2/disk@w5000cca031029f85,0",
        "logical-disk": "c4t5000CCA031029F85d0",
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 41,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 2",
      "hc-fmri": "hc:///chassis=0/bay=2",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        },
        {
          "type": "OK2RM",
          "mode": "off"
        }
      ],
      "disk": {
        "hc-fmri": "hc:///chassis=0/bay=2/disk=0",
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08GAGYSZ",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "device-path": "/pci@7d,0/pci8086,2030@0/pci15d9,808@0/iport@4/disk@w5000cca02c130f79,0",
        "logical-disk": "c5t5000CCA02C130F79d0",
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 44,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 3",
      "hc-fmri": "hc:///chassis=0/bay=3",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        },
        {
          "type": "OK2RM",
          "mode": "off"
        }
      ],
      "disk": {
        "hc-fmri": "hc:///chassis=0/bay=3/disk=0",
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08V134VH",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "device-path": "/pci@7d,0/pci8086,2030@0/pci15d9,808@0/iport@8/disk@w5000cca0310200e1,0",
        "logical-disk": "c6t5000CCA0310200E1d0",
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 44,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 4",
      "hc-fmri": "hc:///chassis=0/bay=4",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        },
        {
          "type": "OK2RM",
          "mode": "off"
        }
      ],
      "disk": {
        "hc-fmri": "hc:///chassis=0/bay=4/disk=0",
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08V1EUPH",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "device-path": "/pci@7d,0/pci8086,2030@0/pci15d9,808@0/iport@10/disk@w5000cca03102a121,0",
        "logical-disk": "c7t5000CCA03102A121d0",
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 44,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 5",
      "hc-fmri": "hc:///chassis=0/bay=5",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        },
        {
          "type": "OK2RM",
          "mode": "off"
        }
      ],
      "disk": {
        "hc-fmri": "hc:///chassis=0/bay=5/disk=0",
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08V19J2H",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "device-path": "/pci@7d,0/pci8086,2030@0/pci15d9,808@0/iport@20/disk@w5000cca031026069,0",
        "logical-disk": "c8t5000CCA031026069d0",
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 45,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 6",
      "hc-fmri": "hc:///chassis=0/bay=6",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        },
        {
          "type": "OK2RM",
          "mode": "off"
        }
      ],
      "disk": {
        "hc-fmri": "hc:///chassis=0/bay=6/disk=0",
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08V19H2H",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "device-path": "/pci@7d,0/pci8086,2030@0/pci15d9,808@0/iport@40/disk@w5000cca031025fed,0",
        "logical-disk": "c9t5000CCA031025FEDd0",
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 45,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 7",
      "hc-fmri": "hc:///chassis=0/bay=7",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        },
        {
          "type": "OK2RM",
          "mode": "off"
        }
      ],
      "disk": {
        "hc-fmri": "hc:///chassis=0/bay=7/disk=0",
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08G81PYZ",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "device-path": "/pci@7d,0/pci8086,2030@0/pci15d9,808@0/iport@80/disk@w5000cca02c0ea585,0",
        "logical-disk": "c10t5000CCA02C0EA585d0",
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 46,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 8",
      "hc-fmri": "hc:///chassis=0/bay=8",
      "sensors": [],
      "leds": []
    },
    {
      "label": "Front Disk 9",
      "hc-fmri": "hc:///chassis=0/bay=9",
      "sensors": [],
      "leds": []
    },
    {
      "label": "Front Disk 10",
      "hc-fmri": "hc:///chassis=0/bay=10",
      "sensors": [],
      "leds": []
    },
    {
      "label": "Front Disk 11",
      "hc-fmri": "hc:///chassis=0/bay=11",
      "sensors": [],
      "leds": []
    },
    {
      "label": "Front Disk 12",
      "hc-fmri": "hc:///chassis=0/bay=12",
      "sensors": [],
      "leds": []
    },
    {
      "label": "Front Disk 13",
      "hc-fmri": "hc:///chassis=0/bay=13",
      "sensors": [],
      "leds": []
    },
    {
      "label": "Front Disk 14",
      "hc-fmri": "hc:///chassis=0/bay=14",
      "sensors": [],
      "leds": []
    },
    {
      "label": "Front Disk 15",
      "hc-fmri": "hc:///chassis=0/bay=15",
      "sensors": [],
      "leds": []
    },
    {
      "label": "Front Disk 16",
      "hc-fmri": "hc:///chassis=0/bay=16",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        },
        {
          "type": "OK2RM",
          "mode": "off"
        }
      ]
    },
    {
      "label": "Front Disk 17",
      "hc-fmri": "hc:///chassis=0/bay=17",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        },
        {
          "type": "OK2RM",
          "mode": "off"
        }
      ]
    },
    {
      "label": "Front Disk 18",
      "hc-fmri": "hc:///chassis=0/bay=18",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        },
        {
          "type": "OK2RM",
          "mode": "off"
        }
      ]
    },
    {
      "label": "Front Disk 19",
      "hc-fmri": "hc:///chassis=0/bay=19",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        },
        {
          "type": "OK2RM",
          "mode": "off"
        }
      ]
    },
    {
      "label": "Front Disk 20",
      "hc-fmri": "hc:///chassis=0/bay=20",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        },
        {
          "type": "OK2RM",
          "mode": "off"
        }
      ]
    },
    {
      "label": "Front Disk 21",
      "hc-fmri": "hc:///chassis=0/bay=21",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        },
        {
          "type": "OK2RM",
          "mode": "off"
        }
      ]
    },
    {
      "label": "Front Disk 22",
      "hc-fmri": "hc:///chassis=0/bay=22",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        },
        {
          "type": "OK2RM",
          "mode": "off"
        }
      ]
    },
    {
      "label": "Front Disk 23",
      "hc-fmri": "hc:///chassis=0/bay=23",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        },
        {
          "type": "OK2RM",
          "mode": "off"
        }
      ]
    },
    {
      "label": "Rear Disk 0",
      "hc-fmri": "hc:///chassis=0/bay=24",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        }
      ],
      "disk": {
        "hc-fmri": "hc:///chassis=0/bay=24/disk=0",
        "manufacturer": "Samsung",
        "model": "SSD 883 DCT 1.92TB",
        "serial-number": "S483NW0K600158N",
        "firmware-revision": "HXT7004Q",
        "size-in-bytes": 533028864,
        "device-path": "/pci@0,0/pci15d9,981@11,5/disk@4,0",
        "logical-disk": "c1t4d0",
        "sensors": [
          {
            "name": "temp",
            "type": "(null)",
            "reading": 0,
            "units": "(null)"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Rear Disk 1",
      "hc-fmri": "hc:///chassis=0/bay=25",
      "sensors": [],
      "leds": [
        {
          "type": "SERVICE",
          "mode": "off"
        },
        {
          "type": "LOCATE",
          "mode": "off"
        }
      ],
      "disk": {
        "hc-fmri": "hc:///chassis=0/port=3/usb-device=0/disk=0",
        "manufacturer": "Verbatim",
        "model": "STORE N GO",
        "serial-number": "E2FF6CAD256CB6DEF3E7",
        "firmware-revision": "8.07",
        "size-in-bytes": 2749366272,
        "device-path": "/pci@0,0/pci15d9,981@14/storage@3/disk@0,0",
        "logical-disk": "c2t0d0",
        "sensors": [],
        "leds": []
      }
    }
  ],
  "power-supplies": [
    {
      "label": "PSU 0",
      "hc-fmri": "hc:///chassis=0/psu=0",
      "manufacturer": "unknown",
      "model": "unknown",
      "firmware-revision": "unknown",
      "sensors": [
        {
          "name": "PS1 Status",
          "type": "POWER_SUPPLY",
          "state": 1,
          "state-description": "PRESENT"
        }
      ],
      "leds": []
    },
    {
      "label": "PSU 1",
      "hc-fmri": "hc:///chassis=0/psu=1",
      "manufacturer": "unknown",
      "model": "unknown",
      "firmware-revision": "unknown",
      "sensors": [
        {
          "name": "PS2 Status",
          "type": "POWER_SUPPLY",
          "state": 1,
          "state-description": "PRESENT"
        }
      ],
      "leds": []
    }
  ],
  "fans": [
    {
      "label": "FAN 0",
      "hc-fmri": "hc:///chassis=0/fan=0",
      "sensors": [
        {
          "name": "FAN1",
          "type": "speed",
          "state": 0,
          "state-description": "NO_STATES_ASSERTED",
          "reading": 3200,
          "units": "RPM",
          "threshold-lower-non-critical": 700,
          "threshold-lower-critical": 500,
          "threshold-lower-non-recoverable": 300,
          "threshold-upper-non-critical": 25300,
          "threshold-upper-critical": 25400,
          "threshold-upper-non-recoverable": 25500
        }
      ],
      "leds": []
    },
    {
      "label": "FAN 1",
      "hc-fmri": "hc:///chassis=0/fan=1",
      "sensors": [
        {
          "name": "FAN2",
          "type": "speed",
          "state": 0,
          "state-description": "NO_STATES_ASSERTED",
          "reading": 3300,
          "units": "RPM",
          "threshold-lower-non-critical": 700,
          "threshold-lower-critical": 500,
          "threshold-lower-non-recoverable": 300,
          "threshold-upper-non-critical": 25300,
          "threshold-upper-critical": 25400,
          "threshold-upper-non-recoverable": 25500
        }
      ],
      "leds": []
    },
    {
      "label": "FAN 2",
      "hc-fmri": "hc:///chassis=0/fan=2",
      "sensors": [],
      "leds": []
    },
    {
      "label": "FAN 3",
      "hc-fmri": "hc:///chassis=0/fan=3",
      "sensors": [],
      "leds": []
    },
    {
      "label": "FAN 4",
      "hc-fmri": "hc:///chassis=0/fan=4",
      "sensors": [],
      "leds": []
    },
    {
      "label": "FAN 5",
      "hc-fmri": "hc:///chassis=0/fan=5",
      "sensors": [],
      "leds": []
    },
    {
      "label": "FAN 6",
      "hc-fmri": "hc:///chassis=0/fan=6",
      "sensors": [
        {
          "name": "FANA",
          "type": "speed",
          "state": 0,
          "state-description": "NO_STATES_ASSERTED",
          "reading": 2200,
          "units": "RPM",
          "threshold-lower-non-critical": 700,
          "threshold-lower-critical": 500,
          "threshold-lower-non-recoverable": 300,
          "threshold-upper-non-critical": 25300,
          "threshold-upper-critical": 25400,
          "threshold-upper-non-recoverable": 25500
        }
      ],
      "leds": []
    },
    {
      "label": "FAN 7",
      "hc-fmri": "hc:///chassis=0/fan=7",
      "sensors": [],
      "leds": []
    }
  ],
  "usb-devices": [
    {
      "hc-fmri": "hc:///motherboard=0/port=1/usb-device=0",
      "usb-vendor-name": "unknown",
      "usb-product-name": "unknown",
      "serial-number": "unknown",
      "usb-version": "2.0",
      "usb-speed": "high-speed",
      "device-driver-name": "hubd",
      "device-driver-instance": 0,
      "device-path": "/pci@0,0/pci15d9,981@14/hub@6",
      "is-internal": "true"
    },
    {
      "hc-fmri": "hc:///motherboard=0/port=1/usb-device=0/port=0/usb-device=0",
      "usb-vendor-name": "unknown",
      "usb-product-name": "unknown",
      "serial-number": "unknown",
      "usb-version": "1.1",
      "usb-speed": "low-speed",
      "device-driver-name": "usb_mid",
      "device-driver-instance": 0,
      "device-path": "/pci@0,0/pci15d9,981@14/hub@6/device@1",
      "is-internal": "true"
    },
    {
      "hc-fmri": "hc:///chassis=0/port=3/usb-device=0",
      "usb-vendor-name": "Verbatim",
      "usb-product-name": "STORE-N-GO",
      "serial-number": "E2FF6CAD256CB6DEF3E7",
      "usb-version": "2.0",
      "usb-speed": "high-speed",
      "device-driver-name": "scsa2usb",
      "device-driver-instance": 0,
      "device-path": "/pci@0,0/pci15d9,981@14/storage@3",
      "is-internal": "false"
    }
  ]
}

```
