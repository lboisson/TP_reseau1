
#ifndef CHIFOUMI_H

#define	CHIFOUMI_H

#define PIERRE 0
#define FEUILLE 1
#define CISEAU 2

#define PLAYER 0
#define SERVER 1
#define NONE 2

typedef struct choice_s {
	char CHOICE[6];
	char SPACE;
	unsigned char choice;
	char ZERO;
} choice_t;

typedef struct score_s {
	char SCORE[5];
	char SPACE;
	unsigned char player_score;
	unsigned char server_score;
	char ZERO;
} score_t;

typedef struct win_s {
	char WIN[3];
	char SPACE;
	unsigned char winner;
	char ZERO;
} win_t;

typedef struct end_s {
	char END[3];
	char SPACE;
	unsigned char winner;
	char ZERO;
} end_t;

#endif