# Systemy operacyjne 2 - projekt
 ## Symulator ruchu drogowego



Prosta gra napisana w języku C++ przy pomocy biblioteki SFML z napisanymi własnoręcznie wątkami i sekcjami krytycznymi. 

## Opis rozgrywki
Gra polega na unikaniu kolizji między nadjeżdżającymi samochodami na skrzyżowaniu. Niektóre z nich dodatkowo zmieniają tor jazdy sugerując to wcześniej za pomocą kierunkowskazu. Każda kolizja zmniejsza życie, aż gra się kończy i musimy zaczynać od nowa.


## Wątki
- addCar_thread - dodawanie samochodów na plansze
- collision_thread - sprawdzanie kolizji między samochodami
- mvThread - odpowiedzialny za poruszanie się samochodów
## Sekcje krytyczne(mutex'y)
- mtx_initGame - pozwawla zablokować dodawanie samochodów na plansze podczas resetowania rozgrywki
- mutex_objects - blokuje dostęp do statycznego vectora objektów klasy Car przechowująca wszystkie aktualne samochody na planszy. Pozwala to na bezproblemowe dodawania jak i usuwanie obiektów z tego vectora. 
- mutex_stop - w połączeniu z condition variable o nazwie cv_stop pozwala na zatrzymanie i wznowienie ruchu samochodu
