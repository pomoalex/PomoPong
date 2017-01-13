#pragma once

enum class game_state { IN_GAME, MENU, PLAY_MENU, OPTIONS_MENU,PLAYING, DIFFICULTY_MENU,GAME_MODE,FINISHED, GAMEPLAY_OPTIONS, SOUND_OPTIONS, INSTRUCTIONS, EXIT };
game_state _game_state;


enum class play_state { PLAYING, PAUSED, FINISHED };
play_state _play_state;


enum class position_of_object { TOP, MIDDLE };


enum class game_difficulty { EASY, MEDIUM, HARD };
game_difficulty _game_difficulty;


enum class music_state { ON, OFF };
music_state _music_state;


enum class effects_state { ON, OFF };
effects_state _effects_state;


enum class assist_state {ON,OFF};
assist_state _assist_state;


enum class play_mode {SINGLEPLAYER,MULTIPLAYER};
play_mode _play_mode;


enum class game_mode {TIME_ATTACK,CHAMPIONSHIP,ENDLESS};
game_mode _game_mode;