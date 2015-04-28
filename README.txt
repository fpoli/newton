===================================================
===  Installazione e utilizzo su Ubuntu/Debian  ===
===================================================

# Installazione delle liberie
make lib

# Compilazione
make install

# Esecuzione del programma
./newton-gui scripts/s-sol.conf

=================
===  Comandi  ===
=================

# Comandi generali

tasto esc: chiudi il programma
tasto p: ferma/riprendi la simulazione
tasto n: visualizza/nascondi i nomi dei pianeti
tasto r: riporta i pianeti nella situazione originaria
tasto t: visualizza/nascondi il tracciato dei pianeti

# Movimento nello spazio

tasto sinistro del mouse + movimento:
Ruota la telecamera attorno al pianeta su cui è ancorata.

tasto destro del mouse + movimento:
Con un movimento verticale si inclina la telecamera di lato.
Con un calibrato movimento diretto verso il basso e a sinistra è possibile
ruotare la telecamera attorno all'asse verticale (relativo alla visuale) che
passa per pianeta su cui è ancorata

rotella del mouse: muove la telecamera avanti/indietro

# Parametri della visualizzazione

tasto 1 e 2: cambia il pianeta su cui è ancorata la telecamera
tasto 3 e 4: cambia il pianeta rispetto a cui è relativo il tracciato del
pianeta a cui è ancorata la telecamera

# Informazioni aggiuntive o di debug

tasto i: visualizza le informazioni di debug (tempo trascorso, step per frame,
pianeta ancora, tracciato relativo a..)

tasto o: visualizza il numero effettivo di frame per secondo e il logo di Ogre
tasto m: visualizza in modo ciclico solo le facce (default), gli spigoli
(wireframe) o i vertici (nuvola di punti) dei volumi rappresentati
tasto c: stampa su stdout (nel terminale) le coordinate attuali della telecamera
rispetto al pianeta su cui è ancorata

=================================
===  Utilizzo di newton-calc  ===
=================================

Per accelerare la simulazone fisica è possibile rinunciare alla visualizzazione
grafica, utilizzando il programma newton-calc.
Analogamente a newton-gui, è richiesto come primo parametro il file di
configurazione, seguito dalla durata in secondi della simulazione e dal percorso
del file su cui deve essere salvata la configurazione finale.

# es: calcola la configurazione dei pianeti dopo un'ora (3600 secondi)
./newton-gui scripts/s-sol.conf 3600 scripts/s-sol_finale.conf

==============================
===  Strumenti aggiuntivi  ===
==============================

In tools/ ci sono alcuni programmi che sono stati utili allo sviluppo del
programma.

spherical2skybox permette di convertire un'immagine rappresentate la superficie
di una sfera nelle sei immagini di cui è composto il cubo del plugin skybox di
Ogre

get_conf.sh e get_planet_data.sh sono due script bash eseguibili sotto linux che
aiutano a costruire una configurazione scaricando le coordinate dei pianeti dal
servizio pubblico Horizons della NASA

