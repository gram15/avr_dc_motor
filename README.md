# avr_dc_motor
dc_motor_contro for avr8 (Atmel atmega328)

This project has been prepared for Qt Creator

For deploying the compiled program into avr, please set the followint parts in the Run Settings:

Deployment
- Method: deploy locally
- Override /usr/bin/make: none
- Make arguments: upload

Run
- Run configuration: Run /usr/bin/gtkterm
- Executable: /usr/bin/gtkterm
- Command line arguments:  --port /dev/ttyS0 --speed 115200 (note: use richt seral device for your configuration)
- Working directory: %{buildDir}

