# Smart Temperature Control System using Arduino

<img src="https://github.com/BogdanSfrangeu/PS1_2021_Sfrangeu/blob/a54e2120a30546041737c93950f09fd6b29f7555/Project.png" width=100% height=100%>

Se va realiza un sistem de conducere pe bază de microcontroller care va efectua următoarele funcţii:
- Încălzirea sistemului până la o temperatura setată ​T​set​ într-un timp ​Tincalzire.
- Menţinerea acestei temperaturi pentru o perioada de timp ​T​mentinere.
- Răcirea sistemului treptat într-o perioadă de timp ​T​racire.

Interfaţa cu utilizatorul
- Sistemul va dispune de un LCD 16x2 pe care va fi afişat meniul, iar în timpul rulării se vor afişa temperatura setată, temperatura curentă şi timpul rămas din etapa actuală (​Tincalzire, ​Tmentinere, ​T​racire).
- Meniul sistemului va permite modificarea următorilor parametri: ​T​set, ​T​incalzire, ​Tmentinere, T​racire, ​K​P, ​K​I​, K​D.
- Parametrii vor fi salvaţi în memoria nevolatilă. Repornirea sistemului nu va afecta parametrii salvaţi. 
- Navigarea prin meniul echipamentului se va face prin patru butoane: “OK”, “Cancel”, “+”, “-”.
- Opţional, se poate crea o interfaţă pe PC pentru afişarea unui grafic al temperaturii. 
 
Reglarea temperaturii 
- Controlul temperaturii va fi asigurat de un regulator de tip PID.
- Senzorul de temperatură folosit va fi unul de tip LM35 sau echivalent.
- Elementul de execuţie al sistemului ce va asigura încălzirea senzorului va fi un bec incandescent c.c. de putere >= 5W ce va putea ajunge la o temperatura de minim 50°C.
- Se va folosi un releu sau un tranzistor pentru a comanda pornirea/oprirea alimentării acestui bec de către microcontroller. 

Perturbaţii
- Se va implementa un generator de numere random (PRNG sau TRNG).
- Sistemul va dispune de un generator de perturbații care va afecta circuitul de masurare a temperaturii cu valori aleatorii folosind modulul PRNG/TRNG implementat anterior.
- Apăsarea butonului “OK” în timpul rulării unui program de reglare a temperaturii va porni modulul de perturbaţii, iar apăsarea din nou a aceluiaşi buton o va opri. 


Tinkercad Project: https://www.tinkercad.com/things/6Wtlsk4OMX5-ps1n/editel?sharecode=Kp-2EgGeIBQzlztJ4XjauFU8ir9OvK4pSigLUvDXiq0

Project Video: https://drive.google.com/file/d/1AJzTQYlTL1-HBfP4wPxry3pfQdGMPHpf/view?usp=sharing
