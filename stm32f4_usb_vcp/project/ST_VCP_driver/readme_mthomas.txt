Filename of the ZIP-Archive was stm32_vcp.zip which has been downloaded
from st.com. Version in this directory is 1.3.1.0 from 25.04.2010.
The underlaying driver-binary is Microsoft's usbser.sys.

Run as as Administrator or as member of the Administrator Group to 
install driver. Files will also unpacked to 
%ProgramFiles%\STMircoelectronic\Virtual COM Port Driver
during installation. 

VID is 0483 and PID is 5740, they may
be changed in inf-file and firmware but the signature
will be invalid after this modification and the package
needs to be re-signed by Microsoft. 

Device should show up as "STMicroelectronics Virtual 
COM Port" in the device-manager under Connections (COM and LPT)
once a CDC-device is connected. 