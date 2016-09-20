HID generic example

Example description
The example shows how to us USBD ROM stack to creates a generic HID device.
The example supports 1 byte report and loops back the data received in
SET_REPORT message.
 
Special connection requirements
Connect the USB cable between micro connector on board and to a host.
The HID reports sent/received by this example are supported by HID_client.exe
program distributed as part of Keil installation "(C:\Keil\ARM\Utilities\HID_Client)".

Build procedures:
Visit the <a href="http://www.lpcware.com/content/project/lpcopen-platform-nxp-lpc-microcontrollers/lpcopen-v200-quickstart-guides">LPCOpen quickstart guides</a>
to get started building LPCOpen projects.