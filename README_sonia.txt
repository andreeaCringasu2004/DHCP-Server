--->Server

Inițializează un socket UDP și activează opțiunile:

1.SO_REUSEADDR și SO_BROADCAST pentru reutilizarea portului și recepția broadcasturilor;

2.Încarcă din ipconfig.txt adresele disponibile pentru alocare; ---> format ipconfig.txt: ubrețeaua utilizată, intervalul de adrese IP disponibile, gateway-ul, serverul DNS și timpii de alocare a adreselor

3.Primește pachete de tip broadcast trimise de clienți;

4.Identifică mesajele DHCP Discover și le salvează în memorie

--->Client

Creează un socket UDP;

Trimite mesaje de tip broadcast către portul serverului;

Utilizează structuri sockaddr_in pentru trimiterea pachetelor;

Afișează mesajele trimise și confirmarea transmiterii.


--->Makefile

Permite compilarea atât a surselor C, cât și a fișierelor header (.h) și a modulelor auxiliare (config.c, client.c, server.c);

Include reguli separate pentru:

1.make server – compilarea doar a serverului;

2.make client – compilarea doar a clientului;

3.make all – compilarea ambelor;

make clean – ștergerea executabilelor și fișierelor obiect.
