# Proyecto: Voltimetro
## puerto.sh

Este script llamado `puerto.sh` utiliza la herramienta `socat` para crear y vincular dos dispositivos TTY (terminales virtuales) en Linux. Estos dispositivos actúan como puertos seriales virtuales, permitiendo la simulación de comunicación serial entre dos aplicaciones o programas que operan sobre dichos puertos.

### Descripción del código

```bash
#!/bin/sh
socat PTY,link=/tmp/ttyS0,raw,echo=0 PTY,link=/tmp/ttyS1,raw,echo=0


