#include <Gamebuino-Meta.h>

//merci à l'exemple "physics" des librairies de la Meta pour les bouts de code :p

//définition de la "trame" des tuiles de 4px * 4px et de la vitesse de chutte et du scrolling
byte tilesize = 4;
float scrollspeed = 1;
float gravity = 0.3;
float friction = 0.9; //friction, dépends des surfaces
float movespeed = 0.3; //contrôle de la vitesse de déplacement


//define de type Box pour les obstacles et autres trucs "durs"
typedef struct {
  byte w;//largeur
  byte x;//pos X
  byte h;//hauteur
  byte y; //pos Y
  byte type; //etat, type, etc
} Box;

//define de type MovingBox pour les trucs qui "bougent"
typedef struct {
  byte w; //largeur
  float x; //pos X
  float xv; //vitesse X
  byte h; //hauteur
  float y; //pos Y
  float yv; //vitesse Y
  byte type; //etat, type, etc
} MovingBox;

void setup() {
  // put your setup code here, to run once:
  gb.begin();
  randomSeed(analogRead(0));

  initBorders();
  initPlateforms();
  initPlayer();

    
  
}


void loop() {
  // put your main code here, to run repeatedly:
  while(!gb.update());
  gb.display.clear();

  updatePlayer();
  updatePlateforms();
  drawPlateforms();
  drawPlayer();
  drawBorders();
  drawInterface();
  


}
