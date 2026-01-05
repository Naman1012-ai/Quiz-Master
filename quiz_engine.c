#include "quiz_engine.h"
#include "utils.h"
#include "auth.h"

void display_timer(int seconds, int posy) {
    gotoxy(0, posy);
    if (seconds <= 5) printf(RED BOLD "TIME LEFT:- "RESET"%02d seconds " , seconds);
    else if (seconds <= 10) printf(YELLOW BOLD "TIME LEFT:- "RESET"%02d seconds " , seconds);
    else printf(GREEN "TIME LEFT:- "RESET"%02d seconds ", seconds);
    fflush(stdout);
}

// ... Paste your get_validans_with_timer, save_attempts, update_leaderboard, 
// leaderboard, shuffleques, diffsub, take_quiz, user_profile, and past_attempts here ...