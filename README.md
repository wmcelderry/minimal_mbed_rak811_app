# Minimal Project for RAK811
This provides an absolute minimal test for RAK811.


# To compile:
```
git clone git@github.com:wmcelderry/minimal_mbed_rak811_app.git
cd minimal_mbed_rak811_app
git submodule init
git submodule update
docker run --rm --mount=type=bind,source="$(pwd)",destination=/var/mbed -w /var/mbed ghcr.io/armmbed/mbed-os-env mbed-cli compile -m "GENERIC_STM32L151CBA" -c
```

# Noticed warning when compiling:
```
[Warning] @,: Compiler version mismatch: Have 10.3.1; expected version >= 9.0.0 and < 10.0.0
```
- likely no affect.



# Symptoms:
When the 'Startup' message is displayed, subsequent output from the periodic 'report' is output at double baud (230400-8N1 instead of 115200-8N1)

When STARTUP_PRINT = 0  there is no Startup message and the periodic 'report' messages output at 115200-8N1.
