# C-Operating-System

	Sistem de operare minimal.

		main:
	Initial, aloc memorie si initializez directorul "home". Pentru a citi fiecare comanda
prin redirectare ma folosesc de fgets care ia informatia de la stdin si o copiaza in sirul s.
words este sirul meu in care retin pe rand fiecare token pe care il extrag din s cu strtok.
Aloc memorie unui string in care copiez ce este in token-ul word.
	Folosind un switch dupa prima litera din cuvant, am separat mai elegant cazurile. Ideea 
generala este ca tratez fiecare comanda in functie de prima litera pentru a diminua comparatiile 
cu strcmp. Daca se introduce o comanda precum "stod", nu se va intra pe comparatia cu "stop" si 
se va elibera memoria prin free(cuv) si se va trece mai departe. In plus, am pus si cazul de 
default in care eliberez memoria pentru comenzi introduse gresit de la prima litera, cum ar fi
"youch" in loc de "touch". La comanda stop merg inapoi pe parent pana la home pentru a elibera
memoria. 

		touch:
	Daca directorul in care vreau sa creez fisierul are si alte fisiere deja, vreau sa verific
daca numele noului fisier coincide cu cele deja existente. Altfel, pur si simplu pointerul se duce 
pana la finalul listei de fisiere pentru a face introducerea. Aloc memorie si initializez fisierul,
copiez numele si refac legaturile din lista.

		mkdir:
	Daca directorul in care vreau sa creez directorul are si alte fisiere deja, vreau sa 
verific daca numele noului director coincide cu cele deja existente. Altfel, pur si simplu 
pointerul se duce pana la finalul listei de directoare pentru a face introducerea. Aloc memorie si
initializez directorul, copiez numele si refac legaturile din lista.

		ls:	
	Ma pozitionez atat pe head in lista de directoare, cat si pe head in lista de fisiere, cu
pointerii specifici fiecarei structuri. Folosind 2 while-uri, afisez intai fiecare director, apoi
fiecare fisier.

		rm:
	Folosesc 2 pointeri p si q pentru a retine atat adresa fisierului gasit, cat si adresa
fisierului precedent din lista. Ulterior, daca s-a gasit fisierul, am 2 cazuri: daca fisierul ce
trebuie sters este head in lista, reinitializez noul head si eliberez memoria, altfel ma folosesc
de pointerul q pentru a reface legaturile dintre fisiere si eliberez memoria.

		rmdir:
	Folosesc 2 pointeri p si q pentru a retine atat adresa directorului gasit, cat si adresa
directorului precedent din lista. Daca s-a gasit directorul, se verifica daca acesta are directoare
in interior. Daca da, se apeleaza functia stop, pentru a sterge recursiv tot ce se afla pe aceasta
cale. Altfel se sterg doar fisierele din interiorul acestuia, intrand pe ramura else. Ulterior, 
se verifica daca directorul ce trebuie sters este head sau nu si se refac legaturile din lista 
pentru fiecare caz in parte.

		cd:
	Pentru ".." functia pozitioneaza target in directorul parinte al acestuia. Altfel, daca
directorul in care ne aflam nu are alte directoare, se incheie functia. In caz contrar, se merge
pe lista de directoare pana se gaseste cel dorit.

		pwd:
	Functia afiseaza calea pana la directorul curent. Este o functie recursiva ce se foloseste
de concatenarea sirurilor intr-un sir declarat static, pentru a se retine valoarea acestuia dupa 
fiecare apel recursiv.

		stop:
	Este o functie recursiva care se pozitioneaza pe fiecare director incepand cu target, se
duce in adancime pana la ultimul director, apoi incepe de acolo stergerea inapoi. Altfel, s-ar fi
pierdut legaturile in ierarhia de fisiere si directoare. Pentru fiecare director, se sterg toate
fisierele din acesta, apoi se trece la urmatorul director din lista si se continua reapelarea
functiei. Scopul functiei stop este de a elibera memoria.

		tree:
	Dupa modelul functiei stop, tree este tot o functie recursiva. Se merge in adancime pentru
a se afisa tot ce contine fiecare director, cu tot cu nr de spatii corespunzator, care este retinut
in level. Se afiseaza intai directoarele, apoi fisierele din directorul curent, se incrementeaza
level la fiecare apel recursiv, insa la final se si decrementeaza level pentru ca in apelul recursiv
am folosit ++level.

		mv:
	Deoarece nu se specifica daca redenumirea se face pe fisier sau director, verific ambele
cazuri. Pentru director, retin adresa directorului ce trebuie redenumit si adresa celui precedent
pentru a reface corect legaturile in lista dupa stergere. Daca sunt contradictii la nume sau la 
existenta fisierelor se afiseaza mesajele specifice. Daca trebuie redenumit directorul, se aloca
memorie si se initializeaza directorul nou cu valorile din directorul vechi si se adauga la 
finalul listei. Se refac legaturile cu ajutorul pointerului precedent si se elibereaza memoria.
La fisier, pe cealalta parte, se apeleaza functiile rm si touch.
