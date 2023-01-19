#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



/* Constantes globales */
    /* Taille de la grille */
#define Larg 7
#define Haut 7

#define Vide ' '
    /* Le nombre de jetons à alligner pour gagner */
#define Puissance 4




/* Variables globales */
    /* L'angle, le sens dans lequel est la grille */
int angle;
    /* La grille dans laquelle on joue */
char grille[Larg][Haut];




/* Initialisation de la grille */
void initGrille() {
    /* On rempli la grille de Vide */
    int i,j;
    for (i = 0; i < Larg; i++) {
        for (j = 0; j < Haut; j++) {
            grille[i][j] = Vide;
        }
    }
}




/* Appliquer la gravité */
    /* Sur une colonne */
void appliqueGravite1C(int nc) {
    int i, k;
    switch (angle) {
    case 0:
    /* Un module qu'on répète en adaptant le "haut" et le "bas" */
        /* On part de la ligne "en bas"-1 et on "remonte" */
        for (i = 1; i < Haut; i++) {
            /* Si la case est vide, on ne fait rien */
            if (grille[nc][Haut-1-i] != ' '){
                k = Haut-1-i;
                /* On "descend" les pions, soit jusqu'à ce qu'il rencontre un autre pion, soit jusqu'"en bas" de la grille */
                /* On répète tant que (on n'est pas "en bas") et (la case "en dessous" est vide) */
                while (k < Haut-1 && grille[nc][k+1] == ' ') {
                    grille[nc][k+1] = grille[nc][k];
                    grille[nc][k] = ' ';
                    k++;
                }
            }
        }
        break;
    case 1:
        for (i = 1; i < Larg; i++) {
            if (grille[Larg-1-i][Haut-1-nc] != ' '){
                k = Larg-1-i;
                while (k < Larg-1 && grille[k+1][Haut-1-nc] == ' ') {
                    grille[k+1][Haut-1-nc] = grille[k][Haut-1-nc];
                    grille[k][Haut-1-nc] = ' ';
                    k++;
                }
            }
        }
        break;
    case 2:
        for (i = 1; i < Haut; i++) {
            if (grille[Larg-1-nc][i] != ' '){
                k = i;
                while (k > 0 && grille[Larg-1-nc][k-1] == ' ') {
                    grille[Larg-1-nc][k-1] = grille[Larg-1-nc][k];
                    grille[Larg-1-nc][k] = ' ';
                    k--;
                }
            }
        }
        break;
    case 3:
        for (i = 1; i < Larg; i++) {
            if (grille[i][nc] != ' '){
                k = i;
                while (k > 0 && grille[k-1][nc] == ' ') {
                    grille[k-1][nc] = grille[k][nc];
                    grille[k][nc] = ' ';
                    k--;
                }
            }
        }
        break;
    default:
        break;
    }
}
    /* Sur la grille totale */
void appliqueGravite() {
    /* On applique la gravité sur chaque colonne */
    int i;
    for (i = 0; i < (angle % 2 ? Haut : Larg); i++) {
        appliqueGravite1C(i);
    }
}




/* Jouer */

/* Rotation du plateau de (90*n)° (0 < n < 4) dans le sens trigo */
void tournerGrille(int n){
    /* On change l'angle et on applique la gravité */
    angle = (angle + n) % 4;
    appliqueGravite();
}
/* Jouer un coup */
void jouer(int nc, char joueur){
    /* On pose le pion "en haut" et on applique la gravité sur la ligne */
    switch (angle) {
    case 0:
        grille[nc][0] = joueur;
        appliqueGravite1C(nc);
        break;
    case 1:
        grille[0][Haut-1-nc] = joueur;
        appliqueGravite1C(nc);
        break;
    case 2:
        grille[Larg-1-nc][Haut-1] = joueur;
        appliqueGravite1C(nc);
        break;
    case 3:
        grille[Larg-1][nc] = joueur;
        appliqueGravite1C(nc);
        break;
    default:
        break;
    }
}




/* Tests */

/* Test de jouabilité sur une colonne */
int jouable(int nc) {
    /* On test si la case "en haut" est vide */
    switch (angle) {
    case 0:
        if(grille[nc][0] == Vide){return 1;}
        break;
    case 1:
        if(grille[0][Haut-1-nc] == Vide){return 1;}
        break;
    case 2:
        if(grille[Larg-1-nc][Haut-1] == Vide){return 1;}
        break;
    case 3:
        if(grille[Larg-1][Haut-1-nc] == Vide){return 1;}
        break;
    default:
        break;
    }
    return 0;
}
/* Teste si la grille est pleine */
int grillePleine() {
    /* On test s'il reste une colonne jouable */
    int i;
    for (i = 0; i < (angle % 2 ? Haut : Larg); i++) {
        if (jouable(i)) {return 0;};
    }
    return 1;
}
/* Donne la ligne à laquelle est le dernier pion joué de la colonne en entrée */
int dernierJoue(int nc) {
    int i = 0;
    switch (angle) {
    case 0:
    /* Un module qu'on répète en adaptant le "haut" et le "bas" */
        /* On cherche le premier pion en "descendant" */
        /* Tant que on ne sort pas de la grille et que la case "en dessous" est vide */
        while (i < Haut && grille[nc][i] == Vide) {i++;}
        /* Soit on a trouvé un pion (à la place i), soit on est sorti de la grille (et on renvoie -1) */
        if (i != Haut) {return i;}
        break;
    case 1:
        while (i < Larg && grille[i][Haut-1-nc] == Vide) {i++;}
        if (i != Larg) {return i;}
        break;
    case 2:
        while (i < Haut && grille[Larg-1-nc][Haut-1-i] == Vide) {i++;}
        if (i != Haut) {return Haut-1-i;}
        break;
    case 3:
        while (i < Larg && grille[Larg-1-i][nc] == Vide) {i++;}
        if (i != Larg) {return Larg-1-i;}
        break;
    default:
        break;
    }
    return -1;
}
/* Détection puissance 4 */
    /* Unité élémentaire */
int puissance4PionDir(int c, int l, int direction) {
    int i;
    if (grille[c][l] == Vide) {return 0;}
    switch (direction) {
    case 0:
        i = 0;
        while (i < Puissance && l + i < Haut && grille[c][l+i] == grille[c][l]) {i++;}
        if (i == Puissance) {return 1;}
        break;
    case 1:
        i = 0;
        while (i < Puissance && l + i < Haut && c + i < Larg && grille[c+i][l+i] == grille[c][l]) {i++;}
        if (i == Puissance) {return 1;}
        break;
    case 2:
        i = 0;
        while (i < Puissance && c + i < Larg && grille[c+i][l] == grille[c][l]) {i++;}
        if (i == Puissance) {return 1;}
        break;
    case 3:
        i = 0;
        while (i < Puissance && l - i >= 0 && c + i < Larg && grille[c+i][l-i] == grille[c][l]) {i++;}
        if (i == Puissance) {return 1;}
        break;
    case 4:
        i = 0;
        while (i < Puissance && l - i >= 0 && grille[c][l-i] == grille[c][l]) {i++;}
        if (i == Puissance) {return 1;}
        break;
    case 5:
        i = 0;
        while (i < Puissance && l - i >= 0 && c - i >= 0 && grille[c-i][l-i] == grille[c][l]) {i++;}
        if (i == Puissance) {return 1;}
        break;
    case 6:
        i = 0;
        while (i < Puissance && c - i >= 0 && grille[c-i][l] == grille[c][l]) {i++;}
        if (i == Puissance) {return 1;}
        break;
    case 7:
        i = 0;
        while (i < Puissance && l + i <= Haut && c - i >= 0 && grille[c-i][l+i] == grille[c][l]) {i++;}
        if (i == Puissance) {return 1;}
        break;
    default:
        break;
    }
    return 0;
}
    /* Autour d'un pion (toute les combinaisons comprenant ce pion) */
int puissance4autour(int c, int l) {
    int i = 0;

    if (grille[c][l] == Vide) {return 0;}
    for (i = 0; i < 8; i++) {
        if (puissance4PionDir(c, l, i)) {return 1;}
    }
    if (puissance4PionDir(c, l - 1, 0)) {return 1;}
    if (puissance4PionDir(c - 1, l - 1, 1)) {return 1;}
    if (puissance4PionDir(c - 1, l, 2)) {return 1;}
    if (puissance4PionDir(c - 1, l + 1, 3)) {return 1;}
    if (puissance4PionDir(c, l + 1, 4)) {return 1;}
    if (puissance4PionDir(c + 1, l + 1, 5)) {return 1;}
    if (puissance4PionDir(c + 1, l, 6)) {return 1;}
    if (puissance4PionDir(c + 1, l - 1, 7)) {return 1;}
    return 0;
}
    /* S'il y en a un pour le joueur en entrée n'importe où dans la grille */
int puissance4global(char joueur) {
    int c, l;
    for (c = 0; c < Larg; c++) {
        for (l = 0; l < Haut - Puissance + 1; l++) {
            if (grille[c][l] == joueur && puissance4PionDir(c,l,0)) {return 1;}
        }
    }
    for (c = 0; c < Larg - Puissance + 1; c++) {
        for (l = 0; l < Haut; l++) {
            if (grille[c][l] == joueur && puissance4PionDir(c,l,2)) {return 1;}
        }
    }
    for (c = 0; c < Larg - Puissance + 1; c++) {
        for (l = 0; l < Haut - Puissance + 1; l++) {
            if (grille[c][l] == joueur && puissance4PionDir(c,l,1)) {return 1;}
        }
    }
    for (c = Puissance; c < Larg; c++) {
        for (l = 0; l < Haut - Puissance + 1; l++) {
            if (grille[c][l] == joueur && puissance4PionDir(c,l,7)) {return 1;}
        }
    }
    return 0;
}




/* Affichage de la grille */
void afficheGrille () {
    int i, j, colonneActuelle, ligneActuelle;
    /* On varie en fonction de l'angle si on regarde d'abord la hauteut ou la largeur */
    colonneActuelle = angle % 2 ? Haut : Larg;
    ligneActuelle = angle % 2 ? Larg : Haut;
    /* Pour chaque lignes */
    for (i = 0; i < ligneActuelle; i++) {
        printf("|");
        /* On trace le séparateur de la bonne longueur */
        for (j = 0; j < colonneActuelle - 1; j++) {printf("---+");}
        printf("---|\n|");
        /* On affiche la bonne case */
        for (j = 0; j < colonneActuelle; j++) {
            switch (angle) {
            case 0:
                printf(" %c |", grille[j][i]);
                break;
            case 1:
                printf(" %c |", grille[i][colonneActuelle-1-j]);
                break;
            case 2:
                printf(" %c |", grille[colonneActuelle-1-j][ligneActuelle-1-i]);
                break;
            case 3:
                printf(" %c |", grille[ligneActuelle-1-i][j]);
                break;
            default:
                break;
            }
        }
        printf("\n");
    }
    /* On trace la fin de la grille */
    for (j = 0; j < colonneActuelle; j++) {printf("¯¯¯¯");}
    printf("¯\n");

    /* On affiche les numéros des colones */
    for (i = 1; i < colonneActuelle + 1; i++) {
        printf(" %2d ", i);
    }
    printf("\n");
}




/* Fonctions pour séparer le main code */

    /* Choix entre Joueur contre Joueur, Joueur contre IA ou IA contre IA */
int choixJoueurIA() {
    int choix;
    printf("Je vais te demander de me dire si tu veux : \n1 - Jouer seul contre le bot\n2 - En 1 contre 1 avec un ami\n3 - Seulement regarder une partie de bots\nRéponse : ");
    scanf("%d", &choix);
    while (choix < 1 || choix > 3) {
        printf("Tu as entré un mauvais numéro, réessaye : ");
        scanf("%d", &choix);
    }
    return choix;
}
    /* Choix du symbole pour le J1 (ou seul joueur) */
char choixSymboleJ1() {
    char symb;
    printf("J1 tu prends quel symbole ? (O ou X)\n");

    /* Je ne comprends pas pourquoi lors de l'exécution le code ignore cette ligne */
    scanf("%c", &symb);

    printf("Oplaaaa trop lent (Non en vrai il y a un bug)\n");
    while (symb != 'x' && symb != 'X' && symb != '2' && symb != 'o' && symb != 'O' && symb != '0' && symb != '1') {
        printf("J'ai pas compis, J1 tu prends quel symbole ? ");
        scanf("%c", &symb);
    }
    if (symb == 'x' || symb == 'X' || symb == '2') {return 'X';} else {return 'O';}
}
    /* Choix du joueur qui commence la partie */
int choixQuiCommence() {
    int choix;
    printf("Qui va commencer à jouer :\n1 - J1\n2 - J2\n3 - Aléatoire\nRéponse : ");
    scanf("%d", &choix);
    while (choix < 1 || choix > 3) {
        printf("J'ai pas compis, qui commence ? ");
        scanf("%d", &choix);
    }
    switch (choix) {
    case 1:
        return 0;
        break;
    case 2:
        return 1;
        break;
    default:
        return (rand() % 2);
        break;
    }
}
    /* Choix de tourner la grille ou de jouer un pion */
int tournerOuJouer() {
    int choix;
    printf("Veux-tu\n1 - Tourner la grille\n2 - Jouer un pion\nRéponse : ");
    scanf("%d", &choix);
    while (choix != 1 && choix != 2) {
        printf("J'ai pas compis, tourner ou jouer ? ");
        scanf("%d", &choix);
    }
    return (choix-1);
}
    /* Choix de l'angle duquel on veut tourner la grille */
int tournerAngle() {
    int choix;
    printf("Comment veux-tu tourner la grille :\n1 - À droite\n2 - D'un demi-tour\n3 - À gauche\nRéponse : ");
    scanf("%d", &choix);
    while (choix < 1 || choix > 3) {
        printf("J'ai pas compis, comment veux-tu tourner la grille ? ");
        scanf("%d", &choix);
    }
    return choix;
}
    /* Choix de la colonne à laquelle on veut mettre le pion */
int jouerColonne() {
    int choix;
    printf("A quelle place veux-tu mettre ton pion ? ");
    scanf("%d", &choix);
    while (choix < 1 || choix > (angle % 2 ? Haut : Larg)) {
        printf("J'ai pas compis, ou veux-tu mettre ton pion ? ");
        scanf("%d", &choix);
    }
    return (choix-1);
}
    /* Un tour d'un joueur */
int tourJoueur(char joueur) {
    int nc;
    /* On commence par afficher la grille */
    afficheGrille();
    /* On précise quel est le symbole que joue le joueur */
    printf("Tu joues les pions : %c\n", joueur);
    /* On change en fonction du choix : jouer ou tourner */
    if (tournerOuJouer()){
        /* Le joueur choisis où il veut jouer */
        do {nc = jouerColonne();}
        while(!jouable(nc));
        /* On joue */
        jouer(nc,joueur);
        /* On teste s'il y a un puissance 4 */
        switch (angle) {
        case 0:
            if (puissance4autour(nc,dernierJoue(nc))){return 1;}
            break;
        case 1:
            if (puissance4autour(dernierJoue(nc),Haut-1-nc)){return 1;}
            break;
        case 2:
            if (puissance4autour(Larg-1-nc,dernierJoue(nc))){return 1;}
            break;
        case 3:
            if (puissance4autour(dernierJoue(nc),nc)){return 1;}
            break;
        default:
            break;
        }
    } else {
        /* On tourne la grille en fonction de ce que nous dis le joueur */
        tournerGrille(tournerAngle());
        /* On teste s'il y a un puissance 4 d'abord chez le joueur, ensuite chez l'adversaire */
        if (puissance4global(joueur)){return 1;}
        if (puissance4global(joueur == 'O' ? 'X' : 'O')){return -1;}
    }
    return 0;
}
    /* Un tour d'un bot random */
int tourBot(char joueur) {
    /* On choisis un coup aléatoire */
    int nc = rand() % (3 + (angle % 2 ? Haut : Larg));
    switch (nc) {
    /* D'abord les rotations */
    case 0:
        /* On tourne */
        tournerGrille(1);
        /* Puis on détecte */
        if (puissance4global(joueur)){return 1;}
        if (puissance4global(joueur == 'O' ? 'X' : 'O')){return -1;}
        break;
    case 1:
        tournerGrille(2);
        if (puissance4global(joueur)){return 1;}
        if (puissance4global(joueur == 'O' ? 'X' : 'O')){return -1;}
        break;
    case 2:
        tournerGrille(3);
        if (puissance4global(joueur)){return 1;}
        if (puissance4global(joueur == 'O' ? 'X' : 'O')){return -1;}
        break;
    default:
    /* Ensuite le placement des jetons */
        nc = nc - 3;
        /* On joue */
        jouer(nc, joueur);
        /* Puis on détecte */
        switch (angle) {
        case 0:
            if (puissance4autour(nc,dernierJoue(nc))){return 1;}
            break;
        case 1:
            if (puissance4autour(dernierJoue(nc),Haut-1-nc)){return 1;}
            break;
        case 2:
            if (puissance4autour(Larg-1-nc,dernierJoue(nc))){return 1;}
            break;
        case 3:
            if (puissance4autour(dernierJoue(nc),nc)){return 1;}
            break;
        default:
            break;
        }
        break;
    }
    return 0;
}
    /* La fin avec le gagnant */
void fin(char gagnant) {
    afficheGrille();
    printf("GG '%c', tu as gagné, tu as battu '%c' !!\n", (gagnant == 'O' ? 'O' : 'X'), (gagnant == 'X' ? 'O' : 'X'));
}
    /* Choix de si tu veux rejouer */
int rejouer() {
    int choix;
    printf("Veux tu rejouer ?\n1 - Oui\n2 - Non\nRéponse : ");
    scanf("%d", &choix);
    while (choix < 1 || choix > 3) {
        printf("Je n'ai pas compris, veux-tu rejouer ? ");
        scanf("%d", &choix);
    }
    return 2-choix;
}

/* Différents modes de jeux */
    /* Mode joueur contre joueur */
void JcJ(char J1, char J2) {
    int etat;
    while (!grillePleine()) {
        /* On regarde si le J1 gagne, perds ou aucun */
        etat = tourJoueur(J1);
        /* S'il perd, on affiche la fin et on arrète */
        if (etat == -1) {fin(J2); break;} 
        /* S'il gagne, on affiche la fin et on arrète */
        else if (etat == 1) {fin(J1); break;}
        /* On passe au J2 */
        etat = tourJoueur(J2);
        if (etat == -1) {fin(J1); break;}
        else if (etat == 1) {fin(J2); break;}
    }
}
    /* Mode joueur contre IA */
void JcIA(char J1, char J2) {
    /* Pareil que JcJ */
    int etat;
    while (!grillePleine()) {
        etat = tourJoueur(J1);
        if (etat == -1) {fin(J2); break;} 
        else if (etat == 1) {fin(J1); break;}
        etat = tourBot(J2);
        if (etat == -1) {fin(J1); break;}
        else if (etat == 1) {fin(J2); break;}
    }
}
    /* Mode IA contre joueur */
void IAcJ(char J1, char J2) {
    /* Pareil que JcJ */
    int etat;
    while (!grillePleine()) {
        etat = tourBot(J1);
        if (etat == -1) {fin(J2); break;} 
        else if (etat == 1) {fin(J1); break;}
        etat = tourJoueur(J2);
        if (etat == -1) {fin(J1); break;}
        else if (etat == 1) {fin(J2); break;}
    }
}
    /* Mode IA contre IA */
void IAcIA(char J1, char J2) {
    /* Pareil que JcJ en attendant 1s entre chaque coup */
    int etat;
    while (!grillePleine()) {
        etat = tourBot(J1);
        if (etat == -1) {fin(J2); break;} 
        else if (etat == 1) {fin(J1); break;}
        afficheGrille();
        sleep(1);
        etat = tourBot(J2);
        if (etat == -1) {fin(J1); break;}
        else if (etat == 1) {fin(J2); break;}
        afficheGrille();
        sleep(1);
    }
}




int main() {
    char symb1, symb2;
    int running;

    printf("\nBonjour intrépide joueur de Puissance %d\n\n", Puissance);

    do {
        /* Setup les variables */
        angle = 0;
        initGrille();
        /* Change de mode en fonction du choix */
        switch (choixJoueurIA()) {
        case 1:
            /* Le Joueur choisit son symbole */
            symb1 = choixSymboleJ1();
            /* L'autre symbole est déterminé */
            symb2 = symb1 == 'O' ? 'X' : 'O';
            /* En fonction de qui commence on change la fonction */
            choixQuiCommence() ? IAcJ(symb2, symb1) : JcIA(symb1, symb2);
            break;
        case 2:
            /* Le J1 choisit son symbole */
            symb1 = choixSymboleJ1();
            /* L'autre symbole est déterminé */
            symb2 = symb1 == 'O' ? 'X' : 'O';
            /* En fonction de qui commence on change le symbole */
            choixQuiCommence() ? JcJ(symb2, symb1) : JcJ(symb1, symb2);
            break;
        case 3:
            /* Le symbole est déterminé aléatoirement */
            symb1 = rand() % 2 ? 'X' : 'O';
            /* L'autre symbole est déterminé */
            symb2 = symb1 == 'O' ? 'X' : 'O';
            /* On lance le IAcIA */
            IAcIA(symb1, symb2);
            break;
        default:
            break;
        }
        running = rejouer();
    } while (running);

    return 0;
}
