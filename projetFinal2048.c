/******************************************
 * PROJET REMISE A NIVEAU : JEU 2048
 * KHADIJA ELFRANI
 * WISSAL BENHAROUGA
 ******************************************/
#include "graphics.h" 

/*  COULEURS UTILISES  */

#define tuile2			0xeee4da
#define tuile4			0xede0c8
#define tuile8			0xf2b179
#define tuile16			0xf59563
#define tuile32			0xf67c5f
#define tuile64			0xf65e3b
#define tuile128		0xedcf72
#define tuile256		0xedcc61
#define tuile512		0xedc850
#define tuile1024		0xedc53f
#define tuile2048		0xedc22e
#define screen			0xFAF8EF
#define fond			0xbbada0
#define cadre 			0x776e65

/*  STRUCTURES  */

typedef struct numCase 
{
	int l, c;
} NUMCASE;

typedef enum 
{
	HAUT,	
	DROIT,	
	BAS,	
	GAUCHE,	
	EXIT,	
	AIDE_ACTIVE,
	AIDE_DESACTIVE,
	HORS_QUADRILLAGE
	
}CHOIX;

/*  VARIABLES GLOBALES  */ 

int plateau[8][8];
int score=0;
BOOL est_aide_active=false;
BOOL est_vue1 = false;
BOOL est_vue2 = false;

/*  PROTOTYPES IMPORTANTS  */ 

POINT transforme_numcase_en_pointBasGauche (NUMCASE);
void decale_droite();
void additionne_classique_droite();
void additionne_classique_bas();	
void additionne_classique_gauche();
void additionne_classique_haut();
void decale_gauche();
void decale_bas();
void decale_haut();
BOOL est_bouton_bas();
BOOL est_bouton_haut();
BOOL est_bouton_gauche();
BOOL est_bouton_droit();
int affiche_resultat();

/* VUE */

void affiche_valeur_score()
{ 	
	POINT P1,P2,P3;
	P1.x=530; P1.y=730;
	P2.x=530; P2.y=705;
	P3.x=580; P3.y=730;
	
	draw_fill_rectangle(P2,P3,screen);		//a chaque changement de valeur de score on efface l'ancien.
	aff_int(score,20,P1,cadre);
}

BOOL aide_desactive()
{
	POINT P1,P2;
	P1.x=60; P1.y=125;
	P2.x=150; P2.y=165;
	
	draw_fill_rectangle(P1,P2,screen);
	draw_fill_rectangle(P1,P2,cadre);
	
	P1.x=75; P1.y=160;
	aff_pol("AIDE",20,P1,blanc);
	affiche_all();
	
	est_aide_active=false;
	
	return est_aide_active;
}

BOOL aide_active()
{
	POINT P1,P2;
	P1.x=60; P1.y=125;
	P2.x=150; P2.y=165;
	
	draw_fill_rectangle(P1,P2,screen);		//effacer l'ancien bouton
	draw_fill_rectangle(P1,P2,vert);
	
	P1.x=75; P1.y=160;
	aff_pol("AIDE",20,P1,blanc);

	affiche_all();
	
	est_aide_active=true;
	
	return est_aide_active;
}

void affiche_quadrillage_titre()
{
	int i;
	POINT P1,P2,P3;
	fill_screen(screen);
	
	// Titre et score
	
	P1.x=70; P1.y=750;
	aff_pol("2048" , 40 ,P1, noir);
	P1.x=450; P1.y=730;
	aff_pol("score : " , 20 ,P1, noir);
	// Rectangle de l' arriere plan
	P1.x =60;P1.y =220;
	P2.x =540;P2.y =700 ;
	draw_fill_rectangle(P1,P2,fond);
	draw_rectangle(P1,P2,cadre);
	
	// Quadrillage
	
	P1.x = 60; 
	P2.x = 540; 
	
	for (i = 220; i < 700; i=i+60)
	{
		P1.y= i+60;
		P2.y=i+60;
		draw_line(P1,P2,cadre);
	}
	
	
	P1.y = 220;
	P2.y = 700;
	for (i = 60; i < 540; i=i+60)
	{
		P1.x=i+60;
		P2.x=i+60;
		draw_line(P1,P2,cadre);
	}
	// BUTTONS                         

	P1.x=300; P1.y=200;                
	P2.x=250; P2.y=150;
	P3.x=350; P3.y=150;
	draw_fill_triangle(P1,P2,P3,cadre);
	
	P1.x=300; P1.y=95;
	P2.x=250; P2.y=145;
	P3.x=350; P3.y=145;
	draw_fill_triangle(P1,P2,P3,cadre);
	
	P1.x=410; P1.y=145;
	P2.x=360; P2.y=195;
	P3.x=360; P3.y=95;	
	draw_fill_triangle(P1,P2,P3,cadre);

	P1.x=190; P1.y=145;
	P2.x=240; P2.y=195;
	P3.x=240; P3.y=95;	
	draw_fill_triangle(P1,P2,P3,cadre);
	
	P1.x=450; P1.y=125;
	P2.x=540; P2.y=165;
	draw_fill_rectangle(P1,P2,cadre);
	
	P1.x=470; P1.y=160;
	aff_pol("EXIT",20,P1,blanc);
		
	aide_desactive(); //affiche le bouton aide en mode desactive
		
	P1.x=240; P1.y=147;
	P2.x=360; P2.y=202;
	draw_rectangle(P1,P2,cadre); 
	
	P1.x=240; P1.y=92;
	P2.x=360; P2.y=147;
	draw_rectangle(P1,P2,cadre);  
	
	P1.x=360; P1.y=92;
	P2.x=410; P2.y=202;
	draw_rectangle(P1,P2,cadre);  
	
	P1.x=190; P1.y=92;
	P2.x=240; P2.y=202;
	draw_rectangle(P1,P2,cadre);   
	
	      
}
void affiche_tuile(int val, POINT basGauche)
{
	POINT hautDroit;
	POINT centre;
	COULEUR maCouleur=fond;
	//dessiner la tuile
		
	hautDroit.x=basGauche.x+60; 
	hautDroit.y=basGauche.y+60;

	switch(val){
		case 0 :	if(est_vue1) {maCouleur=fond;}		else if(est_vue2){maCouleur=silver;} 		break;
		case 2 : 	if(est_vue1) {maCouleur=tuile2;}	else if(est_vue2){maCouleur=cyan;}			break;
		case 4 : 	if(est_vue1) {maCouleur=tuile4;}	else if(est_vue2){maCouleur=bleumarine;}	break;
		case 8 : 	if(est_vue1) {maCouleur=tuile8;}	else if(est_vue2){maCouleur=violet;}		break;
		case 16 :	if(est_vue1) {maCouleur=tuile16;} 	else if(est_vue2){maCouleur=pink;}			break;
		case 32 :	if(est_vue1) {maCouleur=tuile32;} 	else if(est_vue2){maCouleur=vert;}			break;
		case 64 :	if(est_vue1) {maCouleur=tuile64;}	else if(est_vue2){maCouleur=vertolive;}		break;
		case 128 :  if(est_vue1) {maCouleur=tuile128;}	else if(est_vue2){maCouleur=jaune;}			break;
		case 256 : 	if(est_vue1) {maCouleur=tuile256;} 	else if(est_vue2){maCouleur=orange;}		break;
		case 512 : 	if(est_vue1) {maCouleur=tuile512;} 	else if(est_vue2){maCouleur=marron;}		break;
		case 1024 : if(est_vue1) {maCouleur=tuile1024;} else if(est_vue2){maCouleur=magenta;}		break;
		case 2048 : if(est_vue1) {maCouleur=tuile2048;} else if(est_vue2){maCouleur=rouge;}			break;
		
	}
	draw_fill_rectangle(basGauche,hautDroit,maCouleur);
	if(est_vue1)
	{
		draw_rectangle(basGauche,hautDroit,cadre);
	}
	else if(est_vue2)
	{
		draw_rectangle(basGauche,hautDroit,noir);
	}


	//dessiner la valeur au centre du tuile
	
	centre.x=basGauche.x+15; centre.y=basGauche.y+43;
	if(val>4)
	{
		aff_int(val,20,centre,blanc);
	}
	else if(val>0 && val<=4 && est_vue1)
	{
		aff_int(val,20,centre,cadre);
	}
	else if(val>0 && val<=4 && est_vue2)
	{
		aff_int(val,20,centre,blanc);
	}
	
}
void affiche_toutes_les_tuiles()
{
	POINT P;
	int i,j;
	NUMCASE nc;	
	
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			nc.l = i; nc.c = j;		
			P = transforme_numcase_en_pointBasGauche(nc);
			affiche_tuile (plateau[i][j], P);
		}	
	}
	affiche_all();
	
}

void affiche_menu()
{
	fill_screen(screen);
	POINT P1,P2;
	P1.x=155; P1.y=750;
	aff_pol("JEU 2048" , 80 ,P1, cadre);
	
	P1.x=70; P1.y=560;
	P2.x=280; P2.y=600;
	draw_fill_rectangle(P1,P2,cadre); 
	
	P1.x=320; P1.y=560;
	P2.x=530; P2.y=600;
	draw_fill_rectangle(P1,P2,cadre); 
	
	P1.x=70; P1.y=480;
	P2.x=280; P2.y=520;
	draw_fill_rectangle(P1,P2,cadre); 
	
	P1.x=70; P1.y=400;
	P2.x=280; P2.y=440;
	draw_fill_rectangle(P1,P2,cadre); 
	
	P1.x=320; P1.y=480;
	P2.x=530; P2.y=520;
	draw_fill_rectangle(P1,P2,cadre); 
	
	P1.x=320; P1.y=400;
	P2.x=530; P2.y=440;
	draw_fill_rectangle(P1,P2,cadre); 
	
	P1.x=180; P1.y=320;
	P2.x=420; P2.y=360;
	draw_fill_rectangle(P1,P2,cadre); 
	
	P1.x=230; P1.y=365;
	aff_pol("JOUER",40,P1,blanc);
	
	P1.x=360; P1.y=442;
	aff_pol("TOTALE",35,P1,blanc);
	
	P1.x=340; P1.y=522;
	aff_pol("DIFFICILE",35,P1,blanc);
	
	P1.x=380; P1.y=602;
	aff_pol("VUE2",35,P1,blanc); 
	
	P1.x=110; P1.y=602;
	aff_pol("VUE1",35,P1,blanc); 
	
	P1.x=100; P1.y=522;
	aff_pol("FACILE",35,P1,blanc); 
	
	P1.x=75; P1.y=442;
	aff_pol("CLASSIQUE",35,P1,blanc); 
	
	affiche_all();
	
	
}

/* MODELE */

void ajoute_deux_aleatoirement()
{
	BOOL estVide = false; 
	int randL; //ligne aleatoire
	int randC; //colonne aleatoire
		
	while(estVide == false)
	{	
		randL=(alea_int(8)); //affecter un nbr aleatoire entre 0 et 7
		randC=(alea_int(8));
		
		if(plateau[randL][randC]==0)
		{
			plateau[randL][randC]=2;
			estVide = true;
		} 
	}
}

void init_plateau()
{
	int i,j;
	
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			plateau[i][j] = 0;
		}
	}
	ajoute_deux_aleatoirement ();
	ajoute_deux_aleatoirement ();
		
}

void additionne_classique_droite()
{ 
	int i,j,k;
	
		for (i = 0; i < 8; i++)
		{
			
			for (j = 8-1; j>=0; j--)
			{ 
				for(k=j-1;k>=0;k--)
				{
					if(plateau[i][j]!=0)
					{
						if (plateau[i][j]==plateau[i][k])
						{
							plateau[i][j]=2*plateau[i][k];
							plateau[i][k]=0;
							score+=plateau[i][j];
							affiche_valeur_score();
						}
						else if ( (plateau[i][j]!=plateau[i][k]) && (plateau[i][k]!=0) )
						{
							 break;
						}
					} 
					
				}
			}
		}
	
}

void decale_droite()
{
	int i,j,k;
	
	for (i = 0; i < 8; i++)
	{
		k=8-1;
		for (j = 8-1; j >= 0 ; j--)
		{
			if(plateau[i][j] != 0)
			{
				plateau[i][k]=plateau[i][j];
				if(k>j)
				{
					plateau[i][j]=0;
				}
			k--;
			}
		}
	} 
	ajoute_deux_aleatoirement();
}

void additionne_classique_gauche()
{ 
	int i,j,k;
	
		for (i = 0; i < 8; i++)
		{
			
			for (j = 0; j < 8; j++)
			{ 
				for(k=j+1;k<8;k++)
				{
					if(plateau[i][j]!=0)
					{
						if (plateau[i][j]==plateau[i][k])
						{
							plateau[i][j]=2*plateau[i][k];
							plateau[i][k]=0;
							score+=plateau[i][j];
							affiche_valeur_score();
						}
						else if ( (plateau[i][j]!=plateau[i][k]) && (plateau[i][k]!=0) )
						{
							 break;
						}
					}
					
				}
			}
		}
	
}

void decale_gauche()
{
	
	int i,j,k;
	
	for (i = 0; i < 8; i++)
	{
		k=0;
		for (j = 0; j <8 ; j++)
		{
			if(plateau[i][j] != 0)
			{
				plateau[i][k]=plateau[i][j];
				if(k<j)
				{
					plateau[i][j]=0;
				}
			k++;
			}
		}
	}
	ajoute_deux_aleatoirement();
}
void additionne_classique_haut()
{
	int i,j,k;
	
		for (j = 0; j < 8; j++)
		{
			
			for (i = 8-1; i>=0; i--)
			{ 
				for(k=i-1;k>=0;k--)
				{
					if(plateau[i][j]!=0)
					{
						if (plateau[i][j]==plateau[k][j])
						{
							plateau[i][j]=2*plateau[k][j];
							plateau[k][j]=0;
							score+=plateau[i][j];
						}
						else if ( (plateau[i][j]!=plateau[i][k]) && (plateau[i][k]!=0) )
						{
							 break;
						}
					}
					
				}
			}
		}
		affiche_valeur_score();
}

void decale_haut()
{
	int i,j,k;
	
	for (j = 0; j < 8; j++)
	{
		k=8-1;
		for (i = 8-1; i >= 0 ; i--)
		{
			if(plateau[i][j] != 0)
			{
				plateau[k][j]=plateau[i][j];
				if(k>i)
				{
					plateau[i][j]=0;
				}
			k--;
			}
		}
	} 
	ajoute_deux_aleatoirement();
}

void additionne_classique_bas()
{
	int i,j,k;
	
		for (j = 0; j < 8; j++)
		{
			for (i = 0; i < 8; i++)
			{ 
				for(k=i+1;k<8;k++)
				{
					if(plateau[i][j]!=0)
					{
						if (plateau[i][j]==plateau[k][j])
						{
							plateau[i][j]=2*plateau[k][j];
							plateau[k][j]=0;
							score+=plateau[i][j];
							affiche_valeur_score();
						}
						else if ( (plateau[i][j]!=plateau[i][k]) && (plateau[i][k]!=0) )
						{
							 break;
						}
						
					}
					
				}
			}
		}
}

void decale_bas()
{
	int i,j,k;
		
	for (j = 0; j < 8; j++)
	{
		k=0;
		for (i = 0; i <8 ; i++)
		{
			if(plateau[i][j] != 0)
			{
				plateau[k][j]=plateau[i][j];
				if(k<i)
				{
					plateau[i][j]=0;
				}
			k++;
			}
		}
	}
	ajoute_deux_aleatoirement();
}

/* CONTROLEUR */

POINT transforme_numcase_en_pointBasGauche (NUMCASE nc)
{
	POINT basGauche;
	
	basGauche.y = 220 + 60 * nc.l;
	basGauche.x = 60 + 60 * nc.c;
	
	return basGauche;
	
}

int affiche_aide()
{
	int i,j,k;
	BOOL trouve = false;
	POINT P1,P2;
	P1.x=60; P1.y=780;
	
		for (i = 0; i < 8; i++)
		{
			for (j = 0; j < 8; j++)
			{
				for(k = 1; k < 8; k++)
				{
					if((plateau[i][j] != 0 )&&(plateau[i][j] == plateau[i][j+k]))
					{
						if((plateau[i][j]!=plateau[i][j+k]) && (plateau[i][j+k]!=0))
						{
							break;
						}
						else
						{	
							P1.x=60; P1.y=750;
							P2.x=500; P2.y=800;
							draw_fill_rectangle(P1,P2,screen);
							
							P1.x=60; P1.y=780;
							aff_pol("AIDE : droit ou gauche",20,P1,cadre);
							
							
							affiche_all();
							printf("**************************\n");
							printf("AIDE : droit ou gauche\n");
							printf("**************************\n");
							score--;
							affiche_valeur_score();
							
							trouve=true;
							return 0;
						}
					}
				
				
					else if ((plateau[i][j] != 0 )&&(plateau[i][j] == plateau[i+k][j]))
					{
						if((plateau[i][j]!=plateau[i+k][j]) && (plateau[i+k][j]!=0))
						{
							break;
						}
						else
						{
							P1.x=60; P1.y=750;
							P2.x=500; P2.y=800;
							draw_fill_rectangle(P1,P2,screen);
							
							P1.x=60; P1.y=780;
							aff_pol("AIDE : haut ou bas",20,P1,cadre);
							affiche_all();
							
							printf("**************************\n");
							printf("AIDE : haut ou bas\n");
							printf("**************************\n");
							score--;
							affiche_valeur_score();
								
							trouve=true;
							return 0;
						}
					
					}
					else
					{
						continue;
					}
				}
			}
		}	
		
		if(trouve==false) 
		{
			P1.x=60; P1.y=750;
			P2.x=500; P2.y=800;
			draw_fill_rectangle(P1,P2,screen);
			
			P1.x=60; P1.y=780;
			aff_pol("Aucun mouvement n'est profitable",20,P1,cadre);
			affiche_all();
			printf("Aucun mouvement n'est profitable\n");
		}
		
	return 0;
}

BOOL est_bouton_exit(POINT P)
{
	return ((P.x < 540 && P.x > 450) && (P.y < 165 && P.y > 125));
}

BOOL est_bouton_aide(POINT P)
{
	return ((P.x < 150 && P.x > 60) && (P.y < 165 && P.y > 125));
}

BOOL est_bouton_haut(POINT P)
{
	return ((P.x < 360 && P.x > 240) && (P.y < 202 && P.y > 147));
}

BOOL est_bouton_bas(POINT P)
{
	return ((P.x < 360 && P.x > 240) && (P.y < 147 && P.y > 92));
}

BOOL est_bouton_droit(POINT P)
{
	return ((P.x < 410 && P.x > 360) && (P.y < 202 && P.y > 92));
}

BOOL est_bouton_gauche(POINT P)
{
	return ((P.x < 240 && P.x > 190) &&  (P.y < 202 && P.y > 92));
}
BOOL est_bouton_vue1(POINT P)
{
	return ((P.x < 280 && P.x > 70) &&  (P.y < 600 && P.y > 560));
}

BOOL est_bouton_vue2(POINT P)
{
	return ((P.x < 530 && P.x > 320) &&  (P.y < 600 && P.y > 560));
}
BOOL est_bouton_jouer(POINT P)
{

	return ((P.x < 400 && P.x > 200) && (P.y < 360 && P.y > 320));

}

CHOIX recupere_choix_direction()
{ 
	POINT clic;
	clic = wait_clic();
	
	  if (est_bouton_exit(clic))
	  {
		  return EXIT;
	  }
	  else if (est_bouton_bas(clic))
	  {
		  return BAS;
	  }
	  else if (est_bouton_haut(clic))
	  {
		  return HAUT;
	  }
	  else if (est_bouton_gauche(clic))
	  {
		  return GAUCHE;
	  }
	  else if (est_bouton_droit(clic))
	  {
		  return DROIT;
	  }
	  else if ((est_bouton_aide(clic))&&(est_aide_active==false))
	  {
		  return AIDE_ACTIVE;
	  }
	   else if ((est_bouton_aide(clic))&&(est_aide_active==true))
	  {
		  return AIDE_DESACTIVE;
	  }
	 else return HORS_QUADRILLAGE;
}


void execute_choix_direction()
{
	POINT P1,P2;
	CHOIX choix_joueur;
	
	while(1)
	{
		if(est_aide_active)
		{ 
			affiche_aide(); 
		}
		else if(est_aide_active == false)
		{
			P1.x=60; P1.y=750;
			P2.x=500; P2.y=800;
			draw_fill_rectangle(P1,P2,screen);
			affiche_all();
		}
		
		affiche_resultat();
		choix_joueur = recupere_choix_direction();
		
		switch(choix_joueur)
		{
			case HAUT	:	additionne_classique_haut();	decale_haut();		affiche_toutes_les_tuiles(); break;
			case DROIT	:	additionne_classique_droite();	decale_droite();	affiche_toutes_les_tuiles(); break;
			case BAS	:	additionne_classique_bas();		decale_bas();		affiche_toutes_les_tuiles(); break;
			case GAUCHE	:	additionne_classique_gauche();	decale_gauche();	affiche_toutes_les_tuiles(); break;
			case AIDE_ACTIVE	:	aide_active(); break;
			case AIDE_DESACTIVE	:	aide_desactive(); break;
			case EXIT	:	exit(0);	break;
			case HORS_QUADRILLAGE : printf("Hors quadrillage \n");
		}
	}
}

int affiche_resultat()
{
	POINT P1;
	int i,j;
	BOOL estPleine=false;
	BOOL est2048=false;
	
	P1.x=60; P1.y=790;
	
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if(plateau[i][j] == 2048)
			{
				est2048=true;
				return 0;
			}
		}
	}
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if(plateau[i][j] == 0)
			{
				estPleine=false;
				return 0;
			}
			else
			{
				estPleine=true;
				continue;
			}
		}
	}
	
	if(estPleine==true)
	{
		aff_pol("VOUS AVEZ PERDU !",40,P1,noir);
	}
	else if(est2048==true)
	{
		aff_pol("VOUS AVEZ GAGNE !",40,P1,noir);
	}
	return 0;
	
}


void commence_jeu()
{
	
	init_plateau();
	affiche_quadrillage_titre();
	affiche_toutes_les_tuiles();
	affiche_valeur_score();
	aide_desactive();
	execute_choix_direction();

}

void lire_menu()
{
	POINT clic1;
	POINT clic2;
	POINT P1,P2;
	
		clic1 = wait_clic();
	
		if(est_bouton_vue1(clic1))
		{
			est_vue1=true;
			P1.x=70; P1.y=560;
			P2.x=280; P2.y=600;
			draw_fill_rectangle(P1,P2,vert); 
			
			P1.x=110; P1.y=602;
			aff_pol("VUE1",35,P1,blanc); 
			affiche_all();
				
		}
		else if(est_bouton_vue2(clic1))
		{
			est_vue2=true;
			P1.x=320; P1.y=560;
			P2.x=530; P2.y=600;
			draw_fill_rectangle(P1,P2,vert); 
			
			P1.x=380; P1.y=602;
			aff_pol("VUE2",35,P1,blanc); 
			affiche_all();
		}
		
		clic2 = wait_clic();
		
		if(est_bouton_jouer(clic2))
		{
		commence_jeu();
		}
}

int main()
{ 
	init_graphics(600,800); /* (largeur, hauteur) */
	affiche_auto_off(); // Passe en mode d'affichage manuel
	affiche_menu();
	lire_menu();
	
	wait_escape(); 
}
