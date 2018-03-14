#include <Gamebuino-Meta.h>

//merci à l'exemple "physics" pour les bouts de code :p

//définition de la "trame" des tuiles de 4px * 4px et de la vitesse de chutte et du scrolling, etc.
byte tilesize = 4;
float scrollspeed = 1; //1 c'bien ouais ouais !!
float gravity = 0.5; //base 0.35
float friction = 0.85; //friction, dépends des surfaces 0.9 de base
float movespeed = 0.3; //contrôle de la vitesse de déplacement 0.4 de base
float jump = 1; //saut 1 de base




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

//define de type MovingBoxP pour le joueur
typedef struct {
  byte w; //largeur
  float x; //pos X
  float xv; //vitesse X
  byte h; //hauteur
  float y; //pos Y
  float yv; //vitesse Y
} MovingBoxP;

void setup() {
  // put your setup code here, to run once:
  gb.begin();
  
  randomSeed(analogRead(0));
  //ça c'est une astuce pour les random pour être sûr de partir sur une "graine" différente à chaque lancement
  

  initBorders();
  initPlateforms();
  initPlayer();

    
  
}


void loop() {
  // put your main code here, to run repeatedly:
  while(!gb.update());
  gb.display.clear();


  if(gb.buttons.repeat(BUTTON_MENU, 1)){
  initBorders();
  initPlateforms();
  initPlayer();
  }

 
  updatePlateforms();
  updatePlayer();
  
   
  drawPlateforms();
  drawPlayer();
  drawBorders();
  drawInterface();
  


}
