#include <Gamebuino-Meta.h>

//META-FALLDOWN pour Gamebuino META - Par geed - 2018
//merci à l'exemple "physics" de Rodot pour les bouts de code :p

//Paramètres du jeu - attention, ne pas trop bidouiller sinon ça plante !!
byte tilesize = 4;
float scrollspeed = 1; //1 c'bien ouais ouais !! ne pas trop toucher, sinon ça entre en conflit avec la "physique"
float gravity = 0.7; //base 0.7
float friction = 0.85; //friction, dépends des surfaces 0.85 de base
float movespeed = 0.3; //contrôle de la vitesse de déplacement 0.3 de base
float jump = 2.5; //saut 1 de base
float rebound = 0.5; // 0.5 de base, c'est le rebond en cas de choc

//variables diverses
byte mode; // mode de jeu, change l'aléatoire pour plus de variété dans les parties 
boolean pause = false; //tout ça c'est pour mes menus
boolean presstart = false;
boolean playerdead = false;
boolean newgame = true;
boolean bonusscore = false; // active le bonus de score qd le joueur est en haut
boolean malusscore = false; //l'inverse, quand le joueur est dans l'eau ou utilise le boost
int framecount = 0; // ça me sert à compter les frame dans plusieurs trucs

//variables du joueur
int score = 0;
int lastscore = 0;
int lives = 3;
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

void setup() {
  // put your setup code here, to run once:
  gb.begin();
}


void loop() {
  // put your main code here, to run repeatedly:
  while(!gb.update());
  gb.display.clear();

  if(newgame){ //inserer ici le menu de démarrage du jeu 
  mainMenu();
    if (presstart) {
      initBorders();
      initPlateforms();
      initPlayer();
      setMode();
      newgame = false;
      presstart = false;
    }
  }
  
  if(playerdead){ //inserer ici le menu "nouvelle" vie
    lives --;
    if (lives == 0) {
      newgame = true;
    }
    initBorders();
    initPlateforms();
    initPlayer();
  }

  if (!newgame) { //le jeu tourne normalement
    updatePlateforms(); //mise à jour des positions des plateformes (dans obstacles)
    updatePlayer(); //mise à jour du joueur (dans player)
    drawPlateforms(); //dessin des plateforme à l'écran
    drawPlayer(); //dessin du joueur à l'écran
    drawBorders(); //dessin des bordures à l'écran
    drawInterface(); //dessin des éléments d'interfaces à l'écran (dans interface)
  }


}
