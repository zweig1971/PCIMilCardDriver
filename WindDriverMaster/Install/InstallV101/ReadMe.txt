Installation Windriver für die PCI-Mil- Karte 

Sollte wiedererwarten die Installation mit dem grafisch 
unterstützen Tool fehlschlagen, so kann auch per hand 
der Treiber installiert werden. 

Für Windows NT / 2000 :


Schritt 1 : windriver.sys in das Systemverzeichnis 
            \WINNT\system32\drivers kopieren.

Schritt 2 : PCIMilDriver.dll in das Systemverzeichnis 
            \WINNT\system32 kopieren.

ruft man wdreg.exe auf, so erhält man eine Hilfe für die 
möglichen Aufruf- Optionen, mit wdreg install wird der 
Treiber installiert und mit wdreg remove wird der Treiber 
deinstalliert.

Für Windows 98 :

Schritt 1 : windriver.vxd in das Systemverzeichnis 
            \WINDOWS\system\vmm32 kopieren.

Schritt 2 : PCIMilDriver.dll in das Systemverzeichnis 
            \WINDOWS\system kopieren.

mit wdreg -vxd install wird der Treiber installiert und 
mit wdreg -vxd remove wird der Treiber deinstalliert.

viel glück
