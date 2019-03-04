# Duplikaty

* Podstawy programowania – Laboratoria – Projekt 3 - Rafał Uzarowicz – grupa 4 – 300282

* Treść zadania: Napisać program 
duplikaty <folder1> <folder2>
który jako parametry bierze nazwy katalogów bazowego i nowego
Program powinien produkować (czyli pisać na standardowe wyjście) skrypt który usuwa z katalogu+podkatalogów nowego wszystkie pliki występujące w katalogu+podkatalogach bazowym (porównujey zawartość a nie nazwę pliku). Czyli zostaje jedna kopia. Usunięte pliki zastępujemy twardymi linkami (o nazwach takich jakie były oryginalnie).
Program powinien rodzić sobie z dowolnie dużymi folderami (np. cały dysk)
katalog nowy może być podfolderem katalogu bazowego - w takim wypadku pliki katalogu nowego nie są brane do listy plików katalogu bazowego (czyli szukamy czy gdzieś jeszcze nie występują)

* Metoda:
1.	Przyjmujecie dwoch sciezek do folderow (bazowego i nowego).
2.	Utworzenie dwoch list plikow z podanych folderow.
3.	Wziecie jednego pliku z folderu nowe.
4.	Znalezienie duplikatu w liście plików folderu bazowego.
5.	Wypisanie na standardowe wyjście skryptu umożliwającego usunięcie duplikatu i
utworzenie na jego miejscu twardego linku.
6.	Powtórzenie kroków 4-5, aż do końca listy plików folderu bazowego.
7.	Powtórzenie kroków 3-6, aż do końca listy plików folderu nowego.

* Ważne informacje:
- Program przyjmuje dwie prawidłowe ścieżki w systemie Linux.
- Program wypisze odpowiednie komunikaty przy odpowiednich błędach.
- Program ma ograniczenie długości nazwy podfolderów/plików do 50 znaków.
- Program należy uruchamiać w konsoli używając kompilatora gcc.
- Program jest napisany w standardzie języka C99.
- Program wypisuje pełen zbiór komend potrzebny do usunięcia wszystkich duplikatów.
- Program nie przyjmuje sytuacji w której folder bazowy jest podfolderem folderu nowego.

* Funkcje:
- Własne: listFiles, addToList, findDupli, writeOut, freeList, checkDir, checkPath, compareFiles
- Z poszczególnych bibliotek: <stdio.h>: {fopen, puts, fclose, printf, fseek, fread}, <string.h>: {strlen, strcpy, strcat, <dirent.h>: {opendir, readdir, closedir} , <stdlib.h>: {malloc, exit, free}, <errno.h>: {Odpowiednie wartości błędów.}

* Drzewko programu:
-main.

* Testowanie:
- Podanie nieprawidłowej ilości argumentów - Program wypisał komunikat: "Podano nieprawidlowa ilosc sciezek.";
- Podanie nieprawidłowej ścieżki bazowej - Program wypisał komunikat: "Sciezka bazowa jest niepoprawna.";
- Podanie nieistniejącej ścieżki nowej - Program wypisał komunikat: "Sciezka folderu nowego nie istnieje."
- Podanie folderu bazowego jako nowego, a nowego jako bazowego - Program wypisał komunikat: "Folder bazowy nie moze byc podfolderem folderu nowego."

* Wnioski:
Program w prawidłowy sposób wypisuje na standardowe wyjście skrypt do usuwania odpowiednich plików oraz wypisuje odpowiednie komunikaty przy odpowiednich błędach. Program może obsłużyc z góry nieokreślone ilości podfolderów ze względu na użycie listy jednokierunkowej.
