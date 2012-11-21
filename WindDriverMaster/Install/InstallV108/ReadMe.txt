Installation Windriver für die PCI-Mil- Karte 


Für Windows 98 / 2000 / XP

Für diese Systeme muss noch die PCI_Mil.inf- Datei dem HW- Manager
bekannt gegeben werden, um eine immer wieder beim Neu- Booten der
Systeme das erkennen einer vermeintlichen Neuen HW zu verhindern. Unter 
XP ist die inf Datei lebenswichtig, da es sonst zu Störungen kommen kann.

Dazu geht man auf

Start -> Systemsteuerung -> System -> Hardware -> Geräte Manager

Die Karte ,meist wird die Karte als System DMA Controller bezeichnet,
mit Doppelklick anwählen. Dann 'Treiber neu installieren' auswählen.

-> 'Software von eine Liste oder bestimmten Quelle installieren'
-> 'weiter'

Diskette einlegen.

-> 'Folgende Quellen ebenfalls durchsuchen'
-> 'A:\' 
-> 'weiter'

Das System installiert die inf- Datei.

-> 'Fertig stellen'

Wichtig ist, das ERST das Install.exe ausgeführt worden ist und die
Installation erfolgreich war !

Sollte wiedererwarten die Installation mit dem grafisch 
unterstützen Tool ( Install.exe ) fehlschlagen, so kann auch per hand 
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
