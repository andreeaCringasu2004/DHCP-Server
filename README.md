# DHCP-Server
Proiect PSO - Implementare server DHCP

Cuprins:
  1. Comunicare UDP
     - Makefile pentru intregul proiect
     - socketi client + server
     - comunicare broadcast client
  2. Client + Server
     - client: DHCP Discover + DHCP Request
     - server: DHCP Offer + DHCP Ack + incarcarea configuratiei serverului(Fisier configuratie)
  3. Tratarea tipologiilor de mesaje
     - Header DHCP
     - Algoritm generare IP
  4. Stocarea IP-uri denerate + atribuire
     - baza de date IP + monitorizarea leaseurilor

Cerinte:
Lab3 (Comunicare UDP, DHCP Discover, DHCP Offer, incarcare configuratie, generare IP)
  1. Creati un fisier Makefile care sa permita compilarea automata care sa compileze si headerele create.
  2. Creati o comunicare UDP Client-Server (Server sa permita conexiunea cu mai multi clienti)
  3. Incarcati in server, conform fisierului de configurare, detaliile despre acesta.
  4. Permiteti clientului sa poata trimita mesaje broadcast pentru descoperirea serverului.
  5. Configurati serverul astfel incat sa identifice mesajul de tip DHCP Discover (mai intai fol o var hardcodata , iar mai tarziu conform tipologiei de mesaj)
  6. Structurati mesajele DHCP in functie de formatul headerului acestui protocol.
  7. Implementati algoritmul de "generare" al IP-urilor
  8. Alegerea unui IP de catre server pentru clentul care a trimis DHCP Discover.
  9. Creati mesajul de tip DHCP Offer, realizati implementarea, gestionarea si trimiterea acestuia (broadcast) catre client.
