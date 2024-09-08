# ArduinoProject

Ilman kosteuden säätölaite.
Kotonani on kuiva sisäilma, sekä “tyhmä” ilmankostuttaja. Järjestelmä tulee ratkaisemaan
tämän ongelman niin, että kun ilmankosteuden ala tai ylä arvon ohi mennään, servo sulkee
tai käynnistää kyseisen laitteen työntämällä laitteen käynnistysnappia.
Laitteistona Seediuno Nano mikrokontrolleri
AM2320-sensori, joka mittaaa lämpötilaa ja ilman kosteutta
Micro Servo 9g DF9GMS “nappina”
Ohjelmistona Arduino IDE


![image](https://github.com/user-attachments/assets/6a896c7b-f994-4a97-9829-473c7d36c9d2)

Kytkennät:
AM2320-sensori:
VCC -> Arduino 5V
GND -> Arduino GND
SDA -> Arduino A4
SCL -> Arduino A5

Microservo:
Punainen johto -> Arduino 5V
Ruskea johto -> Arduino GND
Oranssi johto -> Arduino D9
Kuvassa DHT22 sensori, joka vähän erilainen kun käytössä oleva, mutta oikeassa elämässä
kytkennät nämä.


Käytetyt kirjastot:
Wire.h (I2C-väylän hallintaan)
Adafruit_Sensor.h (yleinen sensorikirjasto)
Adafruit_AM2320.h (AM2320-sensorin hallintaan)
Servo.h (servo-ohjauksen hallintaan)

Toiminta
Järjestelmään on kovakoodattu 35% ja 45% raja-arvot. Aina kun raja-arvo 35% alittuu, servo
pyörähtää 90 astetta ja kääntyy takaisin (tällä saadaan painettu nappi päälle). Kun yläarvo
45% ylittyy, servo pyörähtää 90 astetta ja palaa takaisin. Näin saadaan helposti painettua
kosteuden mukaan joko nappi päälle tai pois. Raja-arvot valikoituivat nyt puhtaasti sen
mukaan, mihin pääseminen oli sensoriin hengittämällä helpointa.
Ohjelmisto itsessää on aika yksinkertaista koodia. Alustelut ohjeet ja säännöt, minkä
mukaan liikutellaan mitäkin. Kirjastot olivat tässä työssä “mvp” osiossa.


Järjestelmän testaus
Testaus tehtiin aika “pällitestinä” käyttäen Serial Monotoria, johon syötin raja-arvot. Koodi
printtaa minulle tämän hetken kosteutta, sekä tällä hetkellä asetettuja raja-arvoja. Hengitin
ilmaa sensoriin, kunnes ylä-arvo ylittyi, ja servo pyörähti. Kun en enää hengitelly sensoriin,
arvot tippuivat ja ala-arvon alitettua servo liikahti uudestaan. Näin testaamalla pääsin
simuloimaan tilanteen, jossa kosteudet muuttuu. Järjestelmässä on myös mahdollisuus
syöttää Serial Monotorin kautta uudet raja-arvot. Näin ollen sarjaporttia tulee käytettyä.

Tulokset
Järjestelmä toimii juuri niinkuin halusin. Jatkojalostus ideoina olisi led näytölle nykyisen
ilmankosteuden näyttäminen. Kuitenkin tällä hetkellä olleet tavoitteeni onnistuivat ja
järjestelmä toimii niinkuin kuuluukin.
Havaintona yllättävät vaikeudet, saada mikrokontrolleria toimimaan. Tuli yritettyä FTDI
ohjaimia, CH340 ohjaimia, ja laitehallinnan kautta “Valitse ohjaimet tietokoneen käytettävissä
olevien ohjelmien luettelosta” kautta. Tuota kautta löytyi jostain syystä käytännössä kaikki
muut arduinon mallit, paitsi Nano. Myöskään uudelle koneelle laitettuna ohjaimien asennus
ei vain onnistunut. Kuitenkin pitkän taistelun jälkeen CP210x Universal Windows Driver
lataus ja laitehallinnan kautta sen käyttäminen ratkaisi ongelman.



UPDATE

UPDATE: 27.05.2024, kerta sai vielä päivittää niin ajattelin päivittää :D
Näyttö lisätty käyttäen <LiquidCrystal_I2C.h> kirjastoa. Koodi ladattu githubista ja laitettu
kirjastoksi zip kautta arduino idessä
![image](https://github.com/user-attachments/assets/ada6974a-faa9-4f5e-8420-eb3882afce4f)


Kytkennät LCD NÄYTTÖ SDA a4 ja SCL a5, maadoitus maadoitukseen ja virta v5
