simple Bomberman game, created with chatgpt


first Prompt:

Erzeuge mir das Spiel Bomberman. 
Umgebung: Linux, Spielengine SDL3
Werkzeuge: C++ (max Version 14), g++, cmake, .gitignore
Benutze soweit wie möglich Trennung der Dateien, Klassen und Vererbung.
Images: Erzeuge detailreiche Images Figuren durch Zeichnen.
erzeuge ein Menu, Scores (save high score in score.txt), Game Over Menu
erzeuge config.ini, wo wichtige Parameters geladen/gespeichert werden.
Bildschirm: 1200x1000
Benutze kein main.cpp, sondern die von SDL3 verwendete SDL_MAIN_USE_CALLBACKS.

Spielprinzip & Ziel

    Spielfeld: Ein orthogonales Raster (meist 13×11 Felder), in dem sich 
    unzerstörbare Mauern (fest) und zerstörbare Blöcke (weiche Blocksteine) abwechseln.

    Ziel: Alle Gegner (KI‑Gegner oder Mitspieler im Mehrspielermodus) durch 
    Bombenexplosionen ausschalten und dabei selbst nicht getroffen werden. 
    In Levels mit Boss‑ oder Endgegnern besiegst du diese, indem du sie mit Bomben triffst.

Spielablauf

    Levelstart
        Du startest in der Mitte oder Eck des Spielfelds, umgeben von 
        festen Mauern und weichen Blöcken.

        Unter den weichen Blöcken verbergen sich Power‑ups (mehr Bomben, 
        größere Explosionsreichweite, erhöhte Geschwindigkeit, Fernzünder, 
        Schutzschilde etc.).
        
        Bewegung: mit der Tasten Links,Oben,Rechts,Unten
        p fuer Pause
        q fuer quit

    Bomben legen

        Drücke die Bomben‑Taste(Spacebar), um eine Bombe an deiner aktuellen 
        Position abzulegen.

        Du kannst nur eine (oder bei Upgrades mehrere) aktive Bombe(n) 
        gleichzeitig platzieren.

    Explosion & Kettenreaktion

        Nach einem festen Countdown (ca. 2 Sekunden) explodiert die Bombe und 
        erzeugt kreuzförmige Feuerlanzen in alle vier Himmelsrichtungen.

        Feuerlanzen durchbrechen weiche Blöcke (zerstören sie), prallen an 
        festen Mauern ab und können weitere Bomben entzünden (Kettenreaktion).

    Power‑ups sammeln

        Sobald ein weicher Block zerstört wird, erscheint oft ein Power‑up, 
        das du einfach überläufst, um es zu aktivieren.

        Typische Power‑ups:

            +1 Bombe (mehr gleichzeitig platzierbare Bomben)

            +Reichweite (längere Feuerlanzen)

            Schnelligkeit (schnelleres Laufen)

            Fernzünder (Zündung manuell per Knopfdruck)

            Schuhe (Rutschfähigkeit, höherer Geschwindigkeitsschub)

    Gegner & Mehrspieler

        KI‑Gegner: Verschiedene Monster oder computergesteuerte Bombermänner, 
        die dich jagen und selbst Bomben legen.

        Mehrspieler: Bis zu vier Spieler im lokalen Splitscreen oder 
        Online‑Match legen sich gegenseitig Bomben aus.
        
        erzeuge zur Zeit nur Einspieler Modus mit KI-Gegner (einfach)

    Levelabschluss

        Sobald alle Gegner ausgeschaltet sind, öffnet sich ein Ausgangstor 
        (oder ein Portal), durch das du ins nächste Level gelangst.

