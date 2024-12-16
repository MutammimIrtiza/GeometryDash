# include "iGraphics.h"
# include <windows.h>
# include <string.h>

enum PlayerState {ground, jump, fall, on_block, spaceship};
enum GameState {main_menu, nameInput,level_complete, PAUSE, play, game_over, settings, leaderboards, crashed, pauseMenu, playSelect};

#define RecObsCnt 33
#define TriObsCnt 87

Image bg, mario, gnd;
char livesPath[3][100];
int attemptNo = 1;
int portalX = 1850;

int Time = 0;
int playerX = 400;
int playerY = 200;
int playerW = 55;
int playerH = 55;
int onblockNo = -1; // ????????????????
PlayerState playerstate = ground;
GameState gamestate = main_menu;



char cur_player[100];
int nameLen = 0;
int player_cnt = 0;
typedef struct{
	char name[100];
	int attempts;
} Player;

Player players[100];



#define default_dy 24
#define dy_minus 2
int dy = default_dy;

int dyfall = -10;
int gamespeed = 8;

#define rotatedImgNo 96
char monster_idle[rotatedImgNo][100];
int pic_idx = 0;

#define gndImgNo 200
char Gnd[gndImgNo+1][100];
int gndX[200];
Image gnd2[200];
Image lives[3];
#define default_lives 3
int livesCnt = default_lives;
Image pause_menu_bg, game_bg;
Image pmb[2];
Image pmb_hover[2];
Image attNo;
Image digits[10];
char digits_path[10][100];
Image pressR, pressP, newGame, resumeGame, resumeGameHover, newGameHover, portal;
Image pm_resume[2], pm_save[2], pm_exit[2];
Image LB[2], entername, levelComImg;

char sp_com[25][100];

void populate_images(){
    iLoadImage(&gnd, "groundSquare_03.png");
    iLoadImage(&pause_menu_bg, "sprites\\pause_menu\\bg.png");
	iLoadImage(&game_bg, "sprites\\pause_menu\\game_bg.png");
	iLoadImage(&pmb[0], "sprites\\pause_menu\\buttons\\green.png");
	iLoadImage(&pmb[1], "sprites\\pause_menu\\buttons\\blue.png");
	iLoadImage(&pmb_hover[0], "sprites\\pause_menu\\buttons\\greenHover.png");
	iLoadImage(&attNo, "sprites\\Attempt_No.png");
    iLoadImage(&pressR, "sprites\\pressR.png");
    iLoadImage(&pressP, "sprites\\pressP.png");
    iLoadImage(&newGame, "sprites\\play_select\\newGame.png");
    iLoadImage(&resumeGame, "sprites\\play_select\\resumeGame.png");
    iLoadImage(&resumeGameHover, "sprites\\play_select\\resumeGameHover.png");
    iLoadImage(&newGameHover, "sprites\\play_select\\newGameHover.png");
	iLoadImage(&entername, "sprites\\entername.png");
	iLoadImage(&levelComImg, "sprites\\level_complete.png");

	iLoadImage(&pm_resume[0], "sprites\\pause_menu\\resume_1.png");
	iLoadImage(&pm_resume[1], "sprites\\pause_menu\\resume_2.png");
	iLoadImage(&pm_save[0], "sprites\\pause_menu\\save_1.png");
	iLoadImage(&pm_save[1], "sprites\\pause_menu\\save_2.png");
	iLoadImage(&pm_exit[0], "sprites\\pause_menu\\exit_1.png");
	iLoadImage(&pm_exit[1], "sprites\\pause_menu\\exit_2.png");
	iLoadImage(&portal, "sprites\\portal.png");
	iLoadImage(&LB[0], "sprites\\LB1.png");
	iLoadImage(&LB[1], "sprites\\LB2.png");

    // rotation
    for(int i=0;i<rotatedImgNo;i++){
        sprintf(monster_idle[i], "rotated_images\\rotated_%02d.bmp", i);
    }

    // ground
	for(int i = 0; i < gndImgNo; i++){
		sprintf(Gnd[i], "ground_img\\ground_piece_%03d.png", i);
        iLoadImage(&gnd2[i], Gnd[i]);
        gndX[i] = i * 8;
	}

    // lives
    for(int i = 0; i < 3; i++){
        sprintf(livesPath[i], "sprites\\lives\\tile%03d.png", i);
        iLoadImage(&lives[i], livesPath[i]);
    }

	for(int i = 0; i < 10; i++){
		sprintf(digits_path[i], "sprites\\digits\\gold - %d.png", i);
		iLoadImage(&digits[i], digits_path[i]);
	}

	for(int i = 0; i < 25 ; i++){
		sprintf(sp_com[i], "sprites\\spaceship\\output_images_4\\%02d.bmp", i);
	}
}

typedef struct{
	int x;
	int y;
	int w;
	int h;
	int ti;
}RecObstacles;

int recLP = 0;
int recRP = 0;

RecObstacles rec_obs[RecObsCnt];



typedef struct{
	double x[3];
	double y[3];
	int ti;

}TriObs;

int triLP = 0;
int triRP = 0;

TriObs tri_obs[TriObsCnt];

void drawLives();
void drawPlayButton();

void updateGround();
void drawGround();
void updatePlayerPos();


void updateRecObstacle();
void drawRecObstacle();
void checkRecCollision();

void updateTriObstacle();
void drawTriObs();
void checkTriCollision();

//void updateTimers();
void removeObs(int i);
void pausetimers();
void resumetimers();

void loadTriObs();
void loadRecObs();
void life_gone();

void SaveGame();
void Exit();
void loadSavedGame();
void showAttemptNo();
void showSpaceShip();
//void nameInput();

void loadLeaderboard();
void levelComplete();


bool play_button_hover = 0;
bool bool_pmb_hover[3] = {0, 0, 0};
bool bool_playSelect_hover[2] = {0, 0};
bool LB_hover = 0;

int sp_y = 300;
int sp_dy = 0;
int sp_idx = 12;

void iDraw() {

	iClear();

	switch(gamestate){

		case play:  
			iShowImage(0,0,&game_bg);
            iShowImage(1170, 830, &pressP);
			showAttemptNo();
			drawGround();
            //drawLives();
            //iShowImage(imx,imy,&gnd );
            //iWrapImage(&gnd, 199);
            iSetColor(52, 128, 235);	
			drawRecObstacle();
			drawTriObs();
			iSetColor(52, 128, 235);	
			if(playerstate == spaceship) showSpaceShip();
			else iShowBMP2(playerX, playerY, monster_idle[pic_idx],0);
			if(Time > 1270) iShowImage2(portalX, 345, &portal, 2);
			break;

		case PAUSE:
            
			iShowImage(0,0,&game_bg);
            iShowImage(1170, 830, &pressR);
            //iText(1400, 800, "Press 'R' to start", GLUT_BITMAP_TIMES_ROMAN_24);
			showAttemptNo();
            //drawLives();
			drawGround();
			drawRecObstacle();
			drawTriObs();
			//iSetColor(252, 231, 3);	
			
			iShowBMP2(playerX, playerY, monster_idle[pic_idx],0);
			break;

		case main_menu:
			iShowBMP(0, 0 ,"sprites\\main_background.bmp"); 
			iShowBMP2(20, 650 ,"sprites\\logo.bmp",0);
			
			drawPlayButton();

			// if(LB_hover) iShowImage2(1230, 50, &LB[1], 0);
			// else iShowImage2(1230, 50, &LB[0], 0);
			
			break;		

        case crashed: 
            //gamestate = PAUSE;
            gamestate = play;
            Sleep(500);
            //pausetimers();
            break;

        case pauseMenu:
            iShowImage(0,0,&pause_menu_bg);

			// if(bool_pmb_hover[0]) iShowImage(420, 400, &pmb_hover[0]);
			// else iShowImage(420, 400, &pmb[0]);
			

			// iShowImage(1020, 400, &pmb[1]);
			if(bool_pmb_hover[0]) iShowImage(600, 600, &pm_resume[1]);
			else iShowImage(600, 600, &pm_resume[0]);

			if(bool_pmb_hover[1]) iShowImage(600, 400, &pm_save[1]);
			else iShowImage(600, 400, &pm_save[0]);

			if(bool_pmb_hover[2]) iShowImage(600, 200, &pm_exit[1]);
			else iShowImage(600, 200, &pm_exit[0]);

			break;

		case playSelect:
			if(bool_playSelect_hover[0]) iShowImage(300, 400, &newGameHover);
            else iShowImage(300, 400, &newGame);

			if(bool_playSelect_hover[1]) iShowImage(1000, 400, &resumeGameHover);
            else iShowImage(1000, 400, &resumeGame);
			break;

		case nameInput:
			iRectangle(600,500, 400, 100);
			if(nameLen == 0) iShowImage2(610, 510, &entername, 0);
			else iText(610, 510, cur_player , GLUT_BITMAP_TIMES_ROMAN_24  );
			break;

		case level_complete:
			iShowImage2(0,600, &levelComImg, 0);
			iShowImage(400,400, &attNo);

			for(int i = 3; i >=1; i--){
				int x = 1 ; for(int j = 1; j < i ; j++) x *= 10;
				int digit = (attemptNo / x) % 10; 
				// 540 620 700 780
				iShowImage2(900 + 70 * (3-i) , 400, &digits[digit], 0);

			}
			iText(1200, 100, "Press Enter to return to Main Menu.", GLUT_BITMAP_TIMES_ROMAN_24);
	}
}



void showAttemptNo(){
	iShowImage(20,800, &attNo);
	for(int i = 3; i >=1; i--){
		int x = 1 ; for(int j = 1; j < i ; j++) x *= 10;
		int digit = (attemptNo / x) % 10; 
		// 540 620 700 780
		iShowImage2(520 + 70 * (3-i) , 795, &digits[digit], 0);

	}
}


void drawLives(){
    iShowImage2(50, 800, &lives[livesCnt-1], 0);
}

void drawPlayButton(){
    if(play_button_hover) iShowBMP2(600, 250 ,"sprites\\play_button_transparent_hover.bmp",0);
	else iShowBMP2(650, 300 ,"sprites\\play_button_transparent.bmp",0);
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my) {
	
	//place your codes here
	//printf("%d %d\n", mx, my);
}

/*
	function iPassiveMouseMove() is called automatically when the mouse pointer is in motion
*/
void iPassiveMouseMove(int mx, int my)
{
	//place your code here
	if(gamestate == main_menu){
		if(mx > 650 and mx < 900 && my > 300 && my < 550){
			play_button_hover = 1;
		}
		else play_button_hover = 0;

		if(mx > 1230 and mx < 1600 and my > 50 and my < 94) LB_hover = 1;
		else LB_hover = 0;
	}

	if(gamestate == pauseMenu){
		if(mx > 600 and mx < 1088 && my > 600 && my < 707){
			bool_pmb_hover[0] = 1;
		}
		else bool_pmb_hover[0] = 0;

		if(mx > 600 and mx < 916 and my > 400 and my < 507) bool_pmb_hover[1] = 1;
		else bool_pmb_hover[1] = 0;

		if(mx > 600 and mx < 895 and my > 200 and my < 303) bool_pmb_hover[2] = 1;
		else bool_pmb_hover[2] = 0; 

	}

    if(gamestate == playSelect){
        if(mx > 1000 and my > 400 and mx < 1280 and my < 610) bool_playSelect_hover[1] = 1;
        else bool_playSelect_hover[1] = 0;

        if(mx > 300 and my > 400 and mx < 580 and my < 588) bool_playSelect_hover[0] = 1;
        else bool_playSelect_hover[0] = 0;
    }

}


/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		//place your codes here
		//	printf("x = %d, y= %d\n",mx,my);
		
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		//place your codes here
		if(gamestate == main_menu && 
			mx > 650 && mx < 900 && my > 300 && my < 550){
				//gamestate = PAUSE;
				gamestate = playSelect;
			}

		if(gamestate == main_menu and
		mx > 1230 and mx < 1600 and my > 50 and my < 94){
			gamestate = leaderboards;
		}

		if(gamestate == pauseMenu){
			if(mx > 600 and mx < 1088 && my > 600 && my < 707){		
				gamestate = play;
				resumetimers();
			}

			if(mx > 600 and mx < 916 and my > 400 and my < 507){
				SaveGame();
			}

			if(mx > 600 and mx < 895 and my > 200 and my < 303){
				Exit();
			}
		}


		if(gamestate == playSelect){
			if(mx > 420 and mx < 620 && my > 400 && my < 600){
				nameLen = 0;
				gamestate = nameInput;
				//nameInput();
			}

			else if(mx > 1020 && mx < 1220 && my > 400 && my < 600){
				loadSavedGame();
			}
		}

		
	}
}



/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key) {
	if (key == 'q') {
		//exit(0);
	}
	if(key == 'p'){
		if(gamestate == play){
			gamestate = pauseMenu; 
			pausetimers();
		}
	}
	if(key == 'r'){
		if(gamestate == PAUSE || gamestate == pauseMenu){
			//printf("r");
			gamestate = play;
			resumetimers();
		}
	}

	if(gamestate == nameInput){
		if((key >= 65 and key <= 90) || (key >= 97 and key <= 122)){
			cur_player[nameLen] = key;
			nameLen++; cur_player[nameLen] = '\0';
		}

		else if(key == '\b') {
			if(nameLen > 0){
				nameLen--;
				cur_player[nameLen] = '\0';
			}
		}
		else if(key == '\r') gamestate = PAUSE;	
	}
	
	//place your codes for other keys here
}

void iSpecialKeyboard(unsigned char key) {
    if (key == GLUT_KEY_END) {
        exit(0);
    }
    if (key == GLUT_KEY_UP) {
        if (playerstate == ground || playerstate == on_block ) {
			
            playerstate = jump;
            dy = default_dy;  // Ensure dy is reset when jump begins
        }

		if(playerstate == spaceship){
			sp_dy = 11;

		}
    }
}


// dy_minus  default_dy dy

void updatePlayerPos() {
	Time += 1;
	if(Time > 1270) portalX -= gamespeed;

	if(Time > 1820) {
		//printf("gggggggggggggg\n");
		levelComplete();
		pausetimers();
	}

    switch (playerstate) {
        case jump:
            playerY += dy;
            dy -= dy_minus; 
            pic_idx = (pic_idx + 1) % rotatedImgNo;
			//printf("%d\n", playerY);
			//cout << playerY << endl;

            if (playerY < 220) {  // Landing on the ground 
				
                playerY = 200;
                playerstate = ground;
                dy = default_dy;  // Reset only if no immediate jump is detected
				pic_idx = ((pic_idx) / (rotatedImgNo/4)  ) * (rotatedImgNo/4) ; //printf("%d\n", pic_idx);
            }

				if(Time > 1450){
					playerstate = spaceship;
					sp_y = playerY;
					sp_dy = 11;
					playerW = 80;
					playerH = 59;
				}
            break;

        case on_block:
            if (playerX > rec_obs[onblockNo].x + rec_obs[onblockNo].w) {  // > or >= ???
                dy = dyfall;
                playerstate = jump;
            }

			if(Time > 1450){
				playerstate = spaceship;
				sp_y = playerY;
				sp_dy = 11;
				playerW = 80;
				playerH = 59;
			}

            break;

		case spaceship:
			playerY += sp_dy; if(playerY < 200) playerY = 200;

			sp_dy -= 1; 
			if(sp_dy < 0) {
				sp_idx += 1; 
				if(sp_idx > 24) sp_idx = 24;
			}
			else {
				sp_idx -= 1;
				if(sp_idx < 0 ) sp_idx = 0;
			}
			//printf("%d\n", sp_idx);
			break;
    }
}


void loadLeaderboard(){
	FILE *p = fopen("leaderboard.txt", "r");
	while((fscanf(p, "%s %d", players[player_cnt].name, &players[player_cnt].attempts)) == 2 ){
		
		printf("%s %d %d\n", players[player_cnt].name, players[player_cnt].attempts, player_cnt );
		player_cnt++;	
	}
	fclose(p);
}


void levelComplete(){
	
	strcpy(players[player_cnt].name , cur_player);
	players[player_cnt].attempts = attemptNo;
	//printf("%s %d %d\n", cur_player, attemptNo, player_cnt);
	player_cnt++;

	

	//sorting
	for(int i = 0; i < player_cnt-1; i++){
		for(int j = 0; j < player_cnt-1-i; j++){
			// printf("%d %s %d\n", j,players[j].name, players[j].attempts );
			// printf("%d %s %d\n", j+1,players[j+1].name, players[j+1].attempts );

			if(players[j].attempts > players[j+1].attempts){
				Player tem = players[j];
				players[j] = players[j+1];
				players[j+1] = tem;
			}
		}
	}


	FILE *p = fopen("leaderboard.txt", "w");
	for(int i = 0; i < player_cnt; i++){
		printf("%d %s %d\n", i,players[i].name, players[i].attempts );
		fprintf( p,"%s %d\n", players[i].name, players[i].attempts);
	}
	fclose(p);
	gamestate = level_complete;
}

void showSpaceShip(){
	//iShowBMP(0, 0 ,"C:\\Users\\User\\Desktop\\L1T1-project\\sprites\\groundSquare_01_001.bmp");	
	iShowBMP2(playerX, playerY, sp_com[sp_idx],0 );
}


void drawRecObstacle(){

	int i;
	iSetColor(52, 223, 235);	
	for(i = recLP; i <= recRP; i++){
		iFilledRectangle(rec_obs[i].x, rec_obs[i].y, rec_obs[i].w, rec_obs[i].h);
	}

}

void updateRecObstacle(){
	
	// update left pointer
	if(rec_obs[recLP].w < 0) {
		recLP++;		
        //removeObs();
	}

	// update right pointer
	if(rec_obs[recRP+1].ti <= Time) {		
		recRP++;
	}

	// update obstacles within left and right pointers 
	int i;
	for(i = recLP; i <= recRP; i++){
		rec_obs[i].x -= gamespeed;
        if(rec_obs[i].x < 0 ) removeObs(i);
	}

}

void removeObs(int i){
    if(rec_obs[i].w > 0){
        rec_obs[i].x = 0;
        rec_obs[i].w -= gamespeed;
    }
}

void checkRecCollision(){

	// checks collision as well as landing
	int i;
	for(i = recLP; i <= recRP; i++){

		// collision with floating blocks left

		// overlapping widths
		if(playerX < rec_obs[i].x + rec_obs[i].w 
			&& playerX + playerW > rec_obs[i].x){

				
				// landing
				if(dy < 0 && abs(playerY - (rec_obs[i].y + rec_obs[i].h)) <= 16
        			&& playerY > rec_obs[i].y){
						pic_idx = ((pic_idx) / 24 ) * 24 ; //printf("%d\n", pic_idx);
						dy = 0;
						playerY = rec_obs[i].y + rec_obs[i].h;
						playerstate = on_block;

					
					onblockNo = i;
					
					dy = default_dy;
				}

				else if(playerY < rec_obs[i].y + rec_obs[i].h 
						&& playerY + playerH > rec_obs[i].y){
						//printf("%d %d %d %d %d\n",playerY,rec_obs[i].y,rec_obs[i].h, abs(playerY - (rec_obs[i].y + rec_obs[i].h)),playerY > rec_obs[i].y);


                        life_gone();

				}

		}

	}
}

void life_gone(){
    ///livesCnt--;
	attemptNo++;
    //if(livesCnt > 0){
        gamestate = crashed;
        Time = 0;
        loadRecObs();
	    loadTriObs();
        playerX = 400;
        playerY = 200;
		playerW = 55;
		playerH = 55;
        playerstate = ground;
        dy = default_dy;
        dyfall = -10;
        recLP = 0;
        recRP = 0;
        triLP = 0;
        triRP = 0;
        pic_idx = 0;
    //}
    // else{
    //     gamestate = game_over;
    // }
    
}


void drawTriObs(){
	int i;
	iSetColor(52, 223, 235);
	for(i = triLP; i <= triRP; i++){
		iFilledPolygon(tri_obs[i].x, tri_obs[i].y, 3);
	}
}

void updateTriObstacle(){
	// update left pointer
	if(tri_obs[triLP].x[0] < 0) triLP++;

	// update right pointer
	if(tri_obs[triRP+1].ti <= Time) triRP++;

	// update obstacles within left and right pointers 
	int i, j;
	for(i = triLP; i <= triRP; i++){
		for(j = 0; j <3; j++){
			tri_obs[i].x[j] -= gamespeed;
		}
	}
}

void checkTriCollision(){


    for(int i = triLP; i <= triRP; i++){
        int x0 = tri_obs[i].x[0];
        int x1 = tri_obs[i].x[1];
        int x2 = tri_obs[i].x[2];
        int y0 = tri_obs[i].y[0];
        int y1 = tri_obs[i].y[1];
        int y2 = tri_obs[i].y[2];

        // bottom right
        int px1 = playerX + playerW;
        int py1 = playerY;
        int t1 = ((x1-x0)*(py1-y0) - (px1-x0)*(y1-y0)) ;
        int t2 = ((x2-x1)*(py1-y1) - (px1-x1)*(y2-y1)) ;
        int t3 = ((x0-x2)*(py1-y2) - (px1-x2)*(y0-y2)) ;
        if((t1>0 and t2>0 and t3>=0) || (t1<0 and t2<0 and t3<=0)) life_gone();

        // bottom left
        px1 = playerX ;
        py1 = playerY;
        t1 = ((x1-x0)*(py1-y0) - (px1-x0)*(y1-y0)) ;
        t2 = ((x2-x1)*(py1-y1) - (px1-x1)*(y2-y1)) ;
        t3 = ((x0-x2)*(py1-y2) - (px1-x2)*(y0-y2)) ;
        if((t1>0 and t2>0 and t3>=0) || (t1<0 and t2<0 and t3<=0)) life_gone();
    }





}


void pausetimers(){
	iPauseTimer(0);
	iPauseTimer(1);
	iPauseTimer(2);
	iPauseTimer(3);
	iPauseTimer(4);
	iPauseTimer(5);

}

void resumetimers(){
	iResumeTimer(0);
	iResumeTimer(1);
	iResumeTimer(2);
	iResumeTimer(3);
	iResumeTimer(4);
	iResumeTimer(5);
}

void updateGround(){
	
	// gnd_2nd_x -= gamespeed;
	// gnd_1st_width -= gamespeed; 
	// if(gnd_2nd_x <= 10){

	// 	gnd_2nd_x = 230;
	// 	gnd_1st_width = 220;
	// }
	for(int i = 0; i < gndImgNo; i++){
		gndX[i] -= gamespeed;
		if(gndX[i] <=0 ) gndX[i] = 1600 - gamespeed ;
		//cout << gndX[i] << " " << endl;
	}
}

void drawGround(){
	// iSetColor(3, 169, 252);
	// iFilledRectangle(0, 0, 1920,200);

	
	// iSetColor(3, 11, 252);
	// iFilledRectangle(0, 10, gnd_1st_width, 180);

	
	// int cnt;
	// for(cnt = 0; cnt <= 7 ; cnt++){
	// 	iFilledRectangle(gnd_2nd_x + cnt * 230, 10, 220, 180);
	// }
	for(int i = 0; i < gndImgNo; i++){
		iShowImage(gndX[i], 0, &gnd2[i]);
	}
}

void loadRecObs(){
	FILE* fptrRecObs = fopen("recobs.txt", "r");
	int i;
	for(i = 0; i < RecObsCnt; i++){
		fscanf(fptrRecObs, "%d %d %d %d %d", 
		&rec_obs[i].x, &rec_obs[i].y, &rec_obs[i].w, &rec_obs[i].h, &rec_obs[i].ti);
        
        // printf("rec_obs: \n");
        // printf("%d %d %d %d %d\n", 
		// rec_obs[i].x, rec_obs[i].y, rec_obs[i].w, rec_obs[i].h, rec_obs[i].ti);

	}
	fclose(fptrRecObs);
}

void loadTriObs(){
	FILE* fptrTriObs = fopen("triobs.txt", "r");
	int i;
	for(i = 0; i < TriObsCnt; i++){
		fscanf(fptrTriObs, "%lf %lf %lf %lf %lf %lf %d", 
		&tri_obs[i].x[0], &tri_obs[i].x[1], &tri_obs[i].x[2],
		&tri_obs[i].y[0], &tri_obs[i].y[1], &tri_obs[i].y[2], &tri_obs[i].ti);

        // printf("tri_obs: \n");
		// printf("%lf %lf %lf %lf %lf %lf %d\n", 
		// tri_obs[i].x[0], tri_obs[i].x[1], tri_obs[i].x[2],
		// tri_obs[i].y[0], tri_obs[i].y[1], tri_obs[i].y[2], tri_obs[i].ti);
	}
	fclose(fptrTriObs);
}

void SaveGame(){

	FILE *savefilePtr = fopen("saveGameState.txt", "w");
	fprintf(savefilePtr, "%d %d %d %d %d %d %d\n", gamestate, playerstate, Time, playerX, playerY, playerW, playerH);
	fprintf(savefilePtr, "%d %d %d %d %d\n", recLP, recRP, triLP, triRP, pic_idx);
	fprintf(savefilePtr, "%d %d %d %d\n", dy, dyfall, attemptNo, onblockNo);
	fprintf(savefilePtr, "%d %d %d %d %s", sp_y, sp_dy, sp_idx,portalX, cur_player);
	fclose(savefilePtr);


	FILE *saveRecObsPtr = fopen("saveRecObs.txt", "w");
	for(int i = 0; i < RecObsCnt; i++){
		fprintf(saveRecObsPtr, "%d %d %d %d %d\n", 
		rec_obs[i].x, rec_obs[i].y, rec_obs[i].w, rec_obs[i].h, rec_obs[i].ti);
	}
	fclose(saveRecObsPtr);

	FILE * saveTriObsPtr = fopen("saveTriObs.txt", "w");
	for(int i = 0; i < TriObsCnt; i++){
		fprintf(saveTriObsPtr, " %lf %lf %lf %lf %lf %lf %d\n", 
		tri_obs[i].x[0], tri_obs[i].x[1], tri_obs[i].x[2],
		tri_obs[i].y[0], tri_obs[i].y[1], tri_obs[i].y[2], tri_obs[i].ti);
	}
	fclose(saveTriObsPtr);

    //     loadRecObs();
	//     loadTriObs();

}

void Exit(){
	gamestate = main_menu;

		attemptNo = 1;
		Time = 0;
        loadRecObs();
	    loadTriObs();
        playerX = 400;
        playerY = 200;
		playerW = 55;
		playerH = 55;
        playerstate = ground;
        dy = default_dy;
        dyfall = -10;
        recLP = 0;
        recRP = 0;
        triLP = 0;
        triRP = 0;
        pic_idx = 0;
}

void loadSavedGame(){
	// just copy from saveAndExit function
	FILE *savefilePtr = fopen("saveGameState.txt", "r");
	fscanf(savefilePtr, "%d %d %d %d %d %d %d\n", &gamestate, &playerstate, &Time, &playerX, &playerY, &playerW, &playerH);
	fscanf(savefilePtr, "%d %d %d %d %d\n", &recLP, &recRP, &triLP, &triRP, &pic_idx);
	fscanf(savefilePtr, "%d %d %d %d", &dy, &dyfall, &attemptNo, &onblockNo);
	fscanf(savefilePtr, "%d %d %d %d %s", &sp_y, &sp_dy, &sp_idx, &portalX, cur_player);
	fclose(savefilePtr);


	FILE *saveRecObsPtr = fopen("saveRecObs.txt", "r");
	for(int i = 0; i < RecObsCnt; i++){
		fscanf(saveRecObsPtr, "%d %d %d %d %d\n", 
		&rec_obs[i].x, &rec_obs[i].y, &rec_obs[i].w, &rec_obs[i].h, &rec_obs[i].ti);
	}
	fclose(saveRecObsPtr);


	FILE * saveTriObsPtr = fopen("saveTriObs.txt", "r");
	for(int i = 0; i < TriObsCnt; i++){
		fscanf(saveTriObsPtr, " %lf %lf %lf %lf %lf %lf %d\n", 
		&tri_obs[i].x[0], &tri_obs[i].x[1], &tri_obs[i].x[2],
		&tri_obs[i].y[0], &tri_obs[i].y[1], &tri_obs[i].y[2], &tri_obs[i].ti);
	}
	fclose(saveTriObsPtr);

	gamestate = PAUSE;
}


int main() {

	loadRecObs();
	loadTriObs();
	//loadLeaderboard();

	populate_images();
	iSetTimer(15, updatePlayerPos);
	iSetTimer(15, updateRecObstacle);
	iSetTimer(15, updateGround);
	iSetTimer(15, updateTriObstacle);
	iSetTimer(15, checkRecCollision);
	iSetTimer(15, checkTriCollision);
	pausetimers();

	iInitialize(1600, 900, "Geometry Dash");
	return 0;
}