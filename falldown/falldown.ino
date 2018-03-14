#include <Gamebuino-Meta.h>

//merci à l'exemple "physics" pour les bouts de code :p

//définition de la "trame" des tuiles de 4px * 4px et de la vitesse de chutte et du scrolling, etc.
byte tilesize = 4;
float scrollspeed = 0; //1 c'bien ouais ouais !!
float gravity = 0.3; //base 0.3
float friction = 0.9; //friction, dépends des surfaces 0.9 de base
float movespeed = 0.4; //contrôle de la vitesse de déplacement 0.3 de base
float jump = 1; //saut 1 de base
int score = 0; //le score !!


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


  //TEST TEST TEST 
  scrollspeed = 0;
  if(gb.buttons.repeat(BUTTON_A, 1)){
    scrollspeed = 1;
  } 
  if(gb.buttons.repeat(BUTTON_B, 1)){
    scrollspeed = 3;
  }
  if(gb.buttons.repeat(BUTTON_B, 1) && gb.buttons.repeat(BUTTON_A, 1)){
    scrollspeed = -1;
  }
  // TEST TEST TEST

  score ++;
 
  updatePlateforms();
  updatePlayer();
  
   
  drawPlateforms();
  drawPlayer();
  drawBorders();
  drawInterface();
  


}
