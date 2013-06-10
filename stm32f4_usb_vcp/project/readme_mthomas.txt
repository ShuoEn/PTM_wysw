------------------------------------------------------------------------------
A USB virtual COM Port example for STM32F4 controllers (ARMv7, CM4) 

Martin Thomas, Kaiserslautern, Germany, 9/2012 (mthomas(at)rhrk.uni-kl.de)
------------------------------------------------------------------------------

Changelog:
12. Sept. 2012
- initial release based on an example from STMicroelectronics 

This packages comes with:
- Complete source-code for the user application (modified code from STM).
- The libraries from STM (CMSIS, Std.-Periph.-Lib., DSP Lib, USB-Library)
- OpenOCD (prerelease from Freddie Chopin's website, not linked
  with FT2XX-libraries, so no GPL violation)
- ST-Link V2 USB-Driver as this is the interface which is embedded
  on the STM32F4 discovery board
- libusb-Win32 filter-driver which "glues" between the ST-Link2-
  driver and OpenOCD. There is  no need to install a special
  driver for the ST-Link, just install the filter-driver "on top"
  of the standard driver from STM.
  Simply start the GUI and "bind" the ST-Link driver to the filter.
- ST-LINK_CLI: a tool to flash firmware from the command-line
  using the ST-LINK V2. Available from STMicro's website.
- A eclipse workspace (makefile project) including "launches"
  for OpenOCD and Debugging.
- A Keil µVision workspace with all settings to build the firmware
  using GNU tools, debug and flash using the STM32F4-discovery's
  on-board interface.
- ST Microelectronics Virtual COM Port driver for MS Windows
  (AKAIK no special driver is needed for LINUX- and BSD-Systems)
  
  
Test environment:
- STM32F4 discovery with STM32F407VG with updated ST-LINK firmware
- GNU Tools for ARM Embedded Processors (from the 
    launchpad site) (gcc 4.6.2)
- STM32F4xx Standard-Peripheral-Library V1.0.2
- Eclipse Indigo for C/C++ developers with gdb hardware debugging
  plug-in
- OpenOCD 0.6.0-prerelease (openocd-0.6.0-dev-120229143915.exe)
- libusb-win32 filter driver (1.2.6)
- ST-Link_CLI
- Programmers-Notepad (as in WinAVR 10/2010)
- MDKARM 4.5 evaluation-version
- STM32 USB Library V2.1.0

Getting started (all tests have been done on PC under MS Windows XP 32bit)
1. Download & install CodeBench lite if you do not need to use the 
     hardware floating-point unit. (there are no libraries with FPU-code
	 in Codebench lite, software-emulation is always used). 
   - or -
   Download & install the GNU Tools for ARM Embedded Processors
     to use the hardware floating-point unit with this toolchain
     there are 3 options:
     - software-emulation of FPU (aka "no FP"), soft-ABI
     - hardware FPU, soft-ABI
     - hardware FPU, hard-ABI
   The example can be built with both toolchains and more than one
   toolchain can be installed. If no toolchain-path is configured
   in the makefile, the code will be built with the first toolchain
   listed in the system's search-PATH.
2. Download & install the ST-Link V2 driver (included in this package)
3. Download & install the libusb-win32 filter driver (included in this package)
4. Start the LIBUSB's filter-driver GUI and "bind"/install a filter to
   the ST-Link (install filter, select ST-Link)
5. Compile & link the firmware, either by
   - using the makefile with make all (see below)
   - download, install and start MDKARM from ARM/Keil, open 
     the project and hit the build-button. Make sure
     the GNU-toolchain is selected in Manage->Components
     and the path to the CodeBench or ARM toolchain/launchpad 
     installation-directory is correct.
   - download, install and start Eclipse, open the workspace
     and hit the "hammer" (build all)
6. Transfer the firmware to the STM32F4-discovery board
   using the on-board ST-Link either by
   - using OpenOCD: set FLASH_TOOL = openocd in the 
     makefile an use make program
   - using ST-LINK_CLI: set FLASH_TOOL = ST_LINK-CLI
     in the makefile and use make program
   - Eclipse: use the make-target program OR start
     OpenOCD from the external tools menu and
     "project default" from the debug menu
     (this will transfer the firmware an start debugging)
    - ARM/Keil µVision: hit the "Flash"-Button
7. Debugging through the ST-Link-interface can be done in
   Eclipse (debugview<->gdb hardware debugging<->
   arm-none-eabi-gdb<->OpenOCD<->libusbwin32 filter
   <->ST-Link USB-driver<->ST Link hardware<->STM32F4)
   Everything is already set-up.
   Debugging in µVision is also possible. The size
   of this small example is far below the restriction
   of the evaluation version as long as the stdio-tests
   are disabled (printf...). Everything is already
   configured, just hit the "Debug"-button.

8. Install the Virtual COM Port driver and connect
   the STM32F4 discovery to the USB Host (PC) using
   cable with a micro USB Type B plug.


Known issues:
- OpenOCD and uVision sometimes can not connect to the 
  ST-Link. After un-plugging and re-plugging the USB
  cable connection is possible again.
- A workaround has been used for reliable reset
  with OpenOCD by issuing a SYSRESETREQ through the
  AIRCR register. It seems that the OpenOCD's
  cortex_m3 reset_config option is not yet available
  for the ST-LINK V2
- So far DCC does not work as expected, so simple
  "printf-debugging" can not be done through the SWD-
  connection. Maybe a configuration mistake or a 
  later OpenOCD-version will solve this issue. For 
  now USB-CDC or UART can be used to send text to 
  a PC.
- The "reset and run" option in µVisions flash-config
  causes the message that the target can not be restarted
  but the controller starts anyway.   
- The uVision MDKARM 4.5 Simulator's peripheral view only
  supports the ARM-core.   

   
Output of "make all program" (copy/paste from Eclipse console):

(missing)
