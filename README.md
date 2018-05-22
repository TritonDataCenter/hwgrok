# hwgrok
This utility distills the topo snapshot down into a higher-level representation of the hardware configuration and emits that as a single JSON object, which in turn could be consumed by higher-level management software.

Sample output:

```
[root@sky1 /var/tmp/rejohnst]# ./hwgrok | json
{
  "chassis": {
    "manufacturer": "unknown",
    "model": "unknown",
    "sensors": [
      {
        "name": "System Temp",
        "type": "temperature",
        "state": 192,
        "state-description": "0xc0",
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
    "firmware-revision": "1.40",
    "ipv4-address": "172.20.1.28",
    "ipv4-subnet": "255.255.254.0",
    "ipv4-gateway": "172.20.0.1",
    "ipv4-config-type": "dhcp"
  },
  "motherboard": {
    "manufacturer": "Supermicro",
    "model": "unknown",
    "firmware-revision": "1.01",
    "sensors": [
      {
        "name": "PCH Temp",
        "type": "temperature",
        "state": 192,
        "state-description": "0xc0",
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
        "state": 192,
        "state-description": "0xc0",
        "reading": 56,
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
        "state": 192,
        "state-description": "0xc0",
        "reading": 62,
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
        "state": 192,
        "state-description": "0xc0",
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
        "state": 192,
        "state-description": "0xc0",
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
        "state": 192,
        "state-description": "0xc0",
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
        "name": "VRMCpu2IO Temp",
        "type": "temperature",
        "state": 192,
        "state-description": "0xc0",
        "reading": 48,
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
        "state": 192,
        "state-description": "0xc0",
        "reading": 51,
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
        "state": 192,
        "state-description": "0xc0",
        "reading": 40,
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
        "state": 192,
        "state-description": "0xc0",
        "reading": 51,
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
        "state": 192,
        "state-description": "0xc0",
        "reading": 44,
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
        "state": 192,
        "state-description": "0xc0",
        "reading": 12,
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
        "state": 192,
        "state-description": "0xc0",
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
        "state": 192,
        "state-description": "0xc0",
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
        "state": 65284,
        "state-description": "PRESENCE"
      },
      {
        "name": "Vcpu1",
        "type": "voltage",
        "state": 192,
        "state-description": "0xc0",
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
        "name": "Vcpu2",
        "type": "voltage",
        "state": 192,
        "state-description": "0xc0",
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
        "state": 192,
        "state-description": "0xc0",
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
        "state": 192,
        "state-description": "0xc0",
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
        "state": 192,
        "state-description": "0xc0",
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
        "state": 192,
        "state-description": "0xc0",
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
        "state": 192,
        "state-description": "0xc0",
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
      "label": "(null)",
      "manufacturer": "unknown",
      "model": "unknown",
      "number-of-cores": 14,
      "number-of-threads-per-core": 2,
      "sensors": [],
      "leds": []
    },
    {
      "label": "(null)",
      "manufacturer": "unknown",
      "model": "unknown",
      "number-of-cores": 14,
      "number-of-threads-per-core": 2,
      "sensors": [],
      "leds": []
    }
  ],
  "memory": [
    {
      "label": "P1-DIMMA1",
      "dimm": {
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "362BC882",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P1-DIMMA1 Temp",
            "type": "temperature",
            "state": 192,
            "state-description": "0xc0",
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
      "label": "P1-DIMMA2",
      "dimm": {
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "36246BC4",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P1-DIMMA2 Temp",
            "type": "temperature",
            "state": 192,
            "state-description": "0xc0",
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
      "label": "P1-DIMMB1",
      "dimm": {
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "36246309",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P1-DIMMB1 Temp",
            "type": "temperature",
            "state": 192,
            "state-description": "0xc0",
            "reading": 39,
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
      "dimm": {
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "36246BC0",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P1-DIMMC1 Temp",
            "type": "temperature",
            "state": 192,
            "state-description": "0xc0",
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
      "label": "P1-DIMMD1",
      "dimm": {
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "362BBB9F",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P1-DIMMD1 Temp",
            "type": "temperature",
            "state": 192,
            "state-description": "0xc0",
            "reading": 45,
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
      "dimm": {
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "362BC781",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P1-DIMMD2 Temp",
            "type": "temperature",
            "state": 192,
            "state-description": "0xc0",
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
      "label": "P1-DIMME1",
      "dimm": {
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "362468CC",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P1-DIMME1 Temp",
            "type": "temperature",
            "state": 192,
            "state-description": "0xc0",
            "reading": 50,
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
      "dimm": {
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "362BDCBA",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P1-DIMMF1 Temp",
            "type": "temperature",
            "state": 192,
            "state-description": "0xc0",
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
      "label": "P2-DIMMA1",
      "dimm": {
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "35808F99",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P2-DIMMA1 Temp",
            "type": "temperature",
            "state": 192,
            "state-description": "0xc0",
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
      "label": "P2-DIMMA2",
      "dimm": {
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "34EE8887",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P2-DIMMA2 Temp",
            "type": "temperature",
            "state": 192,
            "state-description": "0xc0",
            "reading": 50,
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
      "dimm": {
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "35808FF1",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P2-DIMMB1 Temp",
            "type": "temperature",
            "state": 192,
            "state-description": "0xc0",
            "reading": 50,
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
      "dimm": {
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "34EE7E63",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P2-DIMMC1 Temp",
            "type": "temperature",
            "state": 192,
            "state-description": "0xc0",
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
      "label": "P2-DIMMD1",
      "dimm": {
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "34EE88C1",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P2-DIMMD1 Temp",
            "type": "temperature",
            "state": 192,
            "state-description": "0xc0",
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
      "label": "P2-DIMMD2",
      "dimm": {
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "34EE8821",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P2-DIMMD2 Temp",
            "type": "temperature",
            "state": 192,
            "state-description": "0xc0",
            "reading": 43,
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
      "dimm": {
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "34EE8882",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P2-DIMME1 Temp",
            "type": "temperature",
            "state": 192,
            "state-description": "0xc0",
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
      "label": "P2-DIMMF1",
      "dimm": {
        "manufacturer": "Samsung",
        "part-number": "M393A4K40BB2-CTD",
        "serial-number": "362BBD60",
        "type": "DDR4",
        "size-in-bytes": "34359738368",
        "sensors": [
          {
            "name": "P2-DIMMF1 Temp",
            "type": "temperature",
            "state": 192,
            "state-description": "0xc0",
            "reading": 39,
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
      "pci-vendor-name": "ASPEED Technology, Inc.",
      "pci-device-name": "AST1150 PCI-to-PCI Bridge",
      "pci-subsystem-name": "unknown",
      "device-driver-name": "pcieb",
      "device-driver-instance": 1,
      "device-path": "/pci@0,0/pci8086,a195@1c,5/pci1a03,1150@0"
    },
    {
      "label": "CPU1 SLOT 1 PCI-E 3.0 X8",
      "pci-vendor-name": "ASPEED Technology, Inc.",
      "pci-device-name": "ASPEED Graphics Family",
      "pci-subsystem-name": "unknown",
      "device-driver-name": "vgatext",
      "device-driver-instance": 0,
      "device-path": "/pci@0,0/pci8086,a195@1c,5/pci1a03,1150@0/display@0"
    },
    {
      "label": "CPU1 SLOT 1 PCI-E 3.0 X8",
      "pci-vendor-name": "Intel Corporation",
      "pci-device-name": "unknown",
      "pci-subsystem-name": "unknown",
      "device-driver-name": "pcieb",
      "device-driver-instance": 4,
      "device-path": "/pci@14,0/pci8086,2030@0/pci8086,37c0@0"
    },
    {
      "label": "CPU1 SLOT 1 PCI-E 3.0 X8",
      "pci-vendor-name": "Intel Corporation",
      "pci-device-name": "unknown",
      "pci-subsystem-name": "unknown",
      "device-driver-name": "pcieb",
      "device-driver-instance": 5,
      "device-path": "/pci@14,0/pci8086,2030@0/pci8086,37c0@0/pci8086,37c5@3"
    },
    {
      "label": "CPU1 SLOT 1 PCI-E 3.0 X8",
      "pci-vendor-name": "Intel Corporation",
      "pci-device-name": "Ethernet Connection X722 for 10GBASE-T",
      "pci-subsystem-name": "unknown",
      "device-driver-name": "i40e",
      "device-driver-instance": 3,
      "device-path": "/pci@14,0/pci8086,2030@0/pci8086,37c0@0/pci8086,37c5@3/pci15d9,37d2@0,1"
    },
    {
      "label": "CPU2 SLOT 2 PCI-E 3.0 X16",
      "pci-vendor-name": "Intel Corporation",
      "pci-device-name": "82574L Gigabit Network Connection",
      "pci-subsystem-name": "Gigabit CT Desktop Adapter",
      "device-driver-name": "e1000g",
      "device-driver-instance": 0,
      "device-path": "/pci@14,0/pci8086,2032@2/pci8086,a01f@0"
    },
    {
      "label": "CPU2 SLOT 4 PCI-E 3.0 X16",
      "pci-vendor-name": "Intel Corporation",
      "pci-device-name": "Ethernet Controller XXV710 for 25GbE SFP28",
      "pci-subsystem-name": "Ethernet Network Adapter XXV710",
      "device-driver-name": "i40e",
      "device-driver-instance": 1,
      "device-path": "/pci@33,0/pci8086,2032@2/pci8086,0@0,1"
    },
    {
      "label": "CPU1 SLOT 7 PCI-E 3.0 X8",
      "pci-vendor-name": "LSI Logic / Symbios Logic",
      "pci-device-name": "SAS3008 PCI-Express Fusion-MPT SAS-3",
      "pci-subsystem-name": "unknown",
      "device-driver-name": "mpt_sas",
      "device-driver-instance": 1,
      "device-path": "/pci@55,0/pci8086,2030@0/pci15d9,808@0"
    },
    {
      "label": "CPU1 SLOT 3 PCI-E 3.0 X8",
      "pci-vendor-name": "LSI Logic / Symbios Logic",
      "pci-device-name": "SAS3008 PCI-Express Fusion-MPT SAS-3",
      "pci-subsystem-name": "unknown",
      "device-driver-name": "mpt_sas",
      "device-driver-instance": 0,
      "device-path": "/pci@7c,0/pci8086,2030@0/pci15d9,808@0"
    },
    {
      "label": "CPU1 SLOT 6 PCI-E 3.0 X8",
      "pci-vendor-name": "Chelsio Communications Inc",
      "pci-device-name": "T62100-LP-CR Unified Wire Storage Controller",
      "pci-subsystem-name": "unknown",
      "device-driver-name": "t4nex",
      "device-driver-instance": 0,
      "device-path": "/pci@a4,0/pci8086,2030@0/pci1425,0@0,6"
    },
    {
      "label": "CPU2 SLOT 5 PCI-E 3.0 X16",
      "pci-vendor-name": "LSI Logic / Symbios Logic",
      "pci-device-name": "SAS3008 PCI-Express Fusion-MPT SAS-3",
      "pci-subsystem-name": "unknown",
      "device-driver-name": "mpt_sas",
      "device-driver-instance": 2,
      "device-path": "/pci@cc,0/pci8086,2030@0/pci15d9,808@0"
    }
  ],
  "drive-bays": [
    {
      "label": "Front Disk 0",
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
        "manufacturer": "HGST",
        "model": "HUSMH8010BSS204",
        "serial-number": "0HWZDBUA",
        "firmware-revision": "C360",
        "size-in-bytes": 1245995008,
        "speed-in-rpm": 0,
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 30,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 1",
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
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08V1ERBH",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 39,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 2",
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
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08GAGYSZ",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
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
      "label": "Front Disk 3",
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
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08V134VH",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
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
      "label": "Front Disk 4",
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
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08V1EUPH",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
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
      "label": "Front Disk 5",
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
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08V19J2H",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 42,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 6",
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
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08V19H2H",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 42,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 7",
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
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08G81PYZ",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 43,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 8",
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
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08GAMP6Z",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 43,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 9",
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
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08VHWX3A",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 43,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 10",
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
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08V19HHH",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 43,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 11",
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
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08VHWEGA",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 43,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 12",
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
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08VHXBMA",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 43,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 13",
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
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08VHXDGA",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 42,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 14",
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
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08V1349H",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
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
      "label": "Front Disk 15",
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
        "manufacturer": "HGST",
        "model": "HUC101818CS4200",
        "serial-number": "08V19G8H",
        "firmware-revision": "ADB0",
        "size-in-bytes": 1706716160,
        "speed-in-rpm": 10520,
        "sensors": [
          {
            "name": "temp",
            "type": "TEMP",
            "reading": 40,
            "units": "DEGREES_C"
          }
        ],
        "leds": []
      }
    },
    {
      "label": "Front Disk 16",
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
      "sensors": [],
      "leds": [],
      "disk": {
        "manufacturer": "INTEL",
        "model": "SSDSC2CW120A3",
        "serial-number": "CVCV20240AEL120BGN",
        "firmware-revision": "400i",
        "size-in-bytes": 4070006784,
        "speed-in-rpm": 0,
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
      "sensors": [],
      "leds": []
    }
  ],
  "power-supplies": [
    {
      "label": "PSU 0",
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
      "sensors": [
        {
          "name": "FAN1",
          "type": "speed",
          "state": 192,
          "state-description": "0xc0",
          "reading": 3000,
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
      "sensors": [
        {
          "name": "FAN2",
          "type": "speed",
          "state": 192,
          "state-description": "0xc0",
          "reading": 3100,
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
      "sensors": [],
      "leds": []
    },
    {
      "label": "FAN 3",
      "sensors": [],
      "leds": []
    },
    {
      "label": "FAN 4",
      "sensors": [],
      "leds": []
    },
    {
      "label": "FAN 5",
      "sensors": [],
      "leds": []
    },
    {
      "label": "FAN 6",
      "sensors": [
        {
          "name": "FANA",
          "type": "speed",
          "state": 192,
          "state-description": "0xc0",
          "reading": 2600,
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
      "sensors": [],
      "leds": []
    }
  ]
}
```
