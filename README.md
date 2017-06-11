# teensy-kvm-switcher
A KVM console switcher based on the PJRC Teensy 3.2

A simple application for the PJRC teensy 3.2, (although other versionsshould work fine).  Provides 6 buttons to switch a 4 PORT KVM.  
The KVM has a USB console port, which is not fully compatible with my RGB Keyboard.  Although the KVM supports many key combinations, 
I only use 6, a direct switch to one of the four ports, a 'next port', and a 'previous port'.

Also four LEDs are driven to indicate which port is currently switched.  Although there exists no direct query from the KVM, hence no 
LED's are turned on until the first selection is made.

In order to ensure the Teensy has USB KEYBOARD support ensure you compile withthe 'USE_HID' option (ex: -DUSE_HID).  I could not get 
the 'USE_KEYBOARDONLY' to work, but USE_HID works fine enough.

The buttons have PULL-UPS on the digital pin and I switch GROUND.  The LED pinon the otherhand provide power when turned on.
