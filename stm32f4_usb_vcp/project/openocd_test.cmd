rem set oocd_bin=.\OpenOCD\bin\openocd-0.6.0-dev-120229143915.exe
set oocd_bin=.\OpenOCD\bin\openocd.exe
rem start %oocd_bin% -d 3 -f interface/stlink-v2.cfg -f target/stm32f4x_stlink.cfg -c init
start  %oocd_bin% -d 0 -f board\stm32f4discovery.cfg -c "init" -c "poll on" -c "target_request debugmsgs enable"
rem start telnet localhost 4444
start %ProgramFiles%\putty\putty.exe telnet://localhost:4444
rem pause