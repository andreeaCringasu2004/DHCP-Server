Branch-ul meu de lucru

Primul commit
Initial am realizat o simpla conexiune client-server UDP. 
Al doilea commit
Pe baza a ceea ce a mai realizat colega, am mai extins proiectul:
- am standardizat mesajele, atribuindu-i fiecaruia cate o valoare.
(Am introdus identificatori numerici pentru mesajele DHCP (DISCOVER, OFFER, REQUEST etc.) prin definiții #define.)
- structura mesajului DHCP a fost impartita in 2 componente: 
DHCP_Header (partea fixa apachetului DHCP, comuna tuturor mesajelor)
DHCP_Message (extinde headerul cu informatii specifice protocolului DHCP: tipul mesajului (msg_type), IP-ul oferit (offered_ip), durata lease-ului, routerul si DNS-ul)
=> le-am creat si folosit
- am adaptat serverul pentru a recunoaste tipul mesajului si a raspunde corect (Discover -> Offer)
! Mentionez ca partea de DHCP Discover colega a realizat-o, eu doar am facut micile adaptari de la standardizarea mesajelor, dar in rest am lucrat modul in carea ea a realizat DHCP Discover
- Am gestionat trimiterea mesajului OFFER către client cu IP-ul atribuit, router și DNS
- am modificat porturile configurabile pe non-privilegiate ca eu le aveam ocupate pe ele care colega le folosisie initial


Cerinte:
Serverul trebuie sa identifice tipul de mesaj dhcp a primtit si cum sa il interpreteze.
Structurarea mesajelor DHCP ( DHCP_Header si DHCP_Message)
Algoritlul de alocare/"generare" al ip-urilor:
- pastram un pool de ip-uri disponibile (struct IP_Entry[])
- cand primeste server-ul DHCP Discover ( parcurgem range-ul de valor, si daca allocated == 0 sau lease-ul este expirat, putem atribui acel ip, setam ip-ul pe care il alocam ca allocated = 1 si ii setam si un lease_time)
Trimiterea mesajului de DHCP OFFER catre client.