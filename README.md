A front-end to YDP Collins Dictionary written in Qt4

Originally created by Marcin Nawrocki
http://qt-apps.org/content/show.php/?content=148095

----------------------------------------------------

Ydpd jest wieloplatformową aplikacją umożliwiającą korzystanie z baz słowników Collins oraz Langenscheidt utworzonych przez firmę Young Digital Planet.

Możliwość programu:
* obsługa dwóch słowników oraz dwóch kierunków tłumaczeń
 - Collins - En->Pl/Pl->En
 - Langenscheidt - De->Pl/Pl->De
* wybieranie/wyszukiwanie haseł w słownikach
* zapamiętywanie tłumaczonych haseł i nawigacja pomiędzy nimi
* otwieranie na starcie konkretnego słownika lub ostatnio używanego
* śledzenie schowka - automatyczne tłumaczenie wyrażenia kopiowanego do schowka
* utrzymywanie okna aplikacji zawsze na wierzchu
* śledzenie własnego zaznaczenia (tylko w X11)
* odgrywanie próbek dźwiękowych
* skróty klawiaturowe
 - Esc - powrót do pola edycji wyszukiwanego słowa (jeśli nie jest ono aktywne)
 - Esc - skasowanie tekstu w polu edycji poszukiwanego hasła (jeśli pole jest aktywne)
 - F1 - ustawienie angielsko/niemiecko-polskiego kierunku tłumaczenia
 - F2 - ustawienie polsko-angielskiego/niemieckiego kierunku tłumaczenia
 - Ctrl-D - przełączenie komplementarnych baz słowników dwujęzycznych
 - Ctrl-P - odegranie próbki dźwiękowej zaznaczonego hasła
 - Ctrl-C - kopiowanie zaznaczonego tekstu do schowka
 - Ctrl-O - wywołanie okna opcji
 - Ctrl-</Ctrl-> - poprzedni/kolejny wpis w historii

Ydpd jest tylko interfejsem stąd do prawidłowego jego działania potrzebne są pliki słowników. Należy użyć plików z instalacji windows.
* pliki danych słowników
  - dla Collins'a: dict100.dat  dict100.idx  dict101.dat  dict101.idx
  - dla Langenscheidt'a: dict200.dat  dict200.idx  dict201.dat  dict201.idx
* pliki próbek dźwiękowych haseł
  - dla Collins'a: katalogi S000 S001 S002 itd. (znajdują się na płycie CD)
  - dla Langenscheidt'a: katalogi s000 s001 s002 itd. (znajdują się na płycie CD)

Znane problemy:
* znaki fonetyczne w windows

Do zrobienia
* własny kod obsługi słowników - linki pomiędzy słowami (jak w komercyjnej wersji)

Budowanie projektu testowałem na Ubuntu 11.10 i Kubuntu 11.10
W celu doinstalowania niezbędnych pakietów w konsoli należy wykonać następujące polecenia:
Ubuntu:
sudo apt-get install qt4-dev-tools build-essential libphonon-dev libqt4-dev libqt4-core phonon-backend-gstreamer
Kubuntu:
sudo apt-get install qt4-dev-tools build-essential libphonon-dev

W katalogu Ydpd należy wykonać polecenia:
qmake -config release
make

Budowanie projektu jako plasmoid testowałem na Kubuntu 11.10
W celu doinstalowania niezbędnych pakietów w konsoli należy wykonać następujące polecenia:
sudo apt-get install kdesdk cmake build-essential kdebase-workspace-dev libphonon-dev libydpdict2 libydpdict2-dev

W katalogu Ydpd należy wykonać polecenia:
cmake -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix`
make
sudo make install
kquitapp plasma-desktop && kstart plasma-desktop
