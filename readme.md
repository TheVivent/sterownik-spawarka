# Sterownik

Projekt prostego sterownika pozwalającego na powtarzalne spawanie domowej roboty zgrzewarką do akumulatorów (zrobiona z transformatora mikrofali).

## config
w liniach 12-16 `main.cpp` znajduje się konfiguracja
- STEP - wielkość kroku przy kręceniu encoderem (także najniższa możliwa wartość wyświetlana na ekranie)
- MAX_VALUE - maksymalna wartość wyświetlana na ekranie
- DELAY_FACTOR - mnożnik wartości ustawionej na ekranie.

### zalecenie
```
MAX_VALUE nie powinna być większa niż 99, ponieważ ekran (arduino) nie daje rady
```

## schemat:
![](https://github.com/TheVivent/sterownik-spawarka/blob/master/schemat.png?raw=true)

# sposób działania
- przy pomocy encodera można ustawić czas podawania (lub nie-podawania) napięcia przez relay (w zależności jak podłączymy relay)
- "przycisk" przełącza relay na czas wyświetlony na ekranie i pomnożony przez DELAY_FACTOR (w milisekundach)
- po naciśnięciu pokrętła encodera wartość wyświetlana na ekranie zostaje zapisana w pamięci trwałej która odczytywana jest przy podłączeniu sterownika do prądu.