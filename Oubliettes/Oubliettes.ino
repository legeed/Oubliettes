#include <Gamebuino-Meta.h>

//META-FALLDOWN pour Gamebuino META - Par geed - 2018
//merci à l'exemple "physics" de Rodot pour les bouts de code ainsi qu'à la communauté :p
//ainsi qu'au créateur de https://opengameart.org/content/4x4-dark-platform-tileset qui a fait un supertaf
//les graphismes sont largement inspirés de cette toute petite image :)
//

String VERSION = "V1";

//Paramètres du jeu - attention, ne pas trop bidouiller sinon ça plante !!
byte tilesize = 4;
float scrollspeed; //1 c'bien ouais ouais !! ne pas trop toucher, sinon ça entre en conflit avec la "physique"
float gravity = 0.7; //base 0.7
float friction = 0.85; //friction, dépends des surfaces 0.85 de base
float movespeed = 0.3; //contrôle de la vitesse de déplacement 0.3 de base
float jump = 2.5; //saut 1 de base
float rebound = 0.5; // 0.5 de base, c'est le rebond en cas de choc

//variables diverses
byte mode; // mode de jeu, change l'aléatoire pour plus de variété dans les parties 
int pause = 75; //temps de pause pour nouvelle vie
byte gamestate = 1; //0 running, 1 mainmenu, 2 gameover menu, 3 nvle vie, 4 pause
boolean presstart = false; //a appuyé sur A ?
boolean highscore = false; //a fait un meilleur score !
boolean bonusscore = false; // active le bonus de score qd le joueur est en haut
boolean malusscore = false; //l'inverse, quand le joueur est dans l'eau ou utilise le boost
int framecount = 0; // ça me sert à compter les frame dans plusieurs trucs
int timerinit = 0; //pareil, je compte des trucs avec

//variables du joueur
float score = 0;
int lastscore = 0;
int bestscore = 0;
int lives;
int boosttime = 100; //en frames donc 4s


//contrôle de la difficultée via la taille des plateformes vu que le scrolling est relativement fixe
// L pour plateformes de gauche, R pour droite
// W c'est la longeur et X c'est la position
byte pLWmin; // 3
byte pLWmax; // 5
byte pLXmin; // 4
byte pLXmax; // 9
byte pRWmin; // 7
byte pRWmax; // 9
byte pRXmin; // 0
byte pRXmax; // 4


//là je définit des types, ça permet de dire, par exemple, toutes les "boites"  ont une valeur w x h et y
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


//////////////////////////////////////////////


void setup() {

  gb.begin();
  bestscore = gb.save.get(0); //je recup l'ancien best score de la sauvegarde
}


//////////////////////////////////////////////


void loop() {
   
  while(!gb.update());
  
  gb.display.clear();
  gb.lights.clear();

  switch (gamestate) {
    case 0: //jeu en cours
      presstart = false; //bon, pour le coup, presstart est lié à C, d'hbaitude il est lié à A
      updatePlateforms(); //mise à jour des positions des plateformes (dans obstacles)
      updatePlayer(); //mise à jour du joueur (dans player)
      drawBack(); //dessin de l'arrière plan
      drawPlateforms(); //dessin des plateforme à l'écran
      drawPlayer(); //dessin du joueur à l'écran
      drawInterface(); //dessin des éléments d'interfaces à l'écran (dans interface)
      //drawBenchmark(); //pour avoir des stats sur le CPU et la ram
      if (presstart) {
        gamestate = 4; //->pause
      }
      break;
    case 1: //menu principal
      presstart = false;
      lives = 3;
      setMode();
      drawMain(); //ecran principal
      if (presstart) {
        gamestate = 3;
      }
      break;
    case 2: //menu gameover
      presstart = false;
      if (highscore) {
        drawHscore(); //high score !
      } else {      
      drawGameover(); //ecran gameover
      }
      if (presstart) {
        lives = 3;
        setMode();
        gamestate = 3;
      }
      break;
    case 3: //nouvelle vie
      presstart = false;
      initBorders();
      initPlateforms();
      drawBack();
      drawInterface();
      drawReady(); //ecran "pret ?"
      if (presstart) {
        initPlayer(); //initialisation du joueur
        gamestate = 0;
      }   
      break;
    case 4: //pause
      presstart = false;
      drawBack(); 
      drawPlateforms();
      drawPlayer();
      drawInterface();
      drawPause(); //pause
      if (presstart) {
      gamestate = 0;
      }
      break;
  }
}
