#pragma once

enum class game_state { IN_GAME, MENU, PLAY_MENU, OPTIONS_MENU, SINGLEPLAYER, MULTIPLAYER, DIFFICULTY_MENU, GAMEPLAY_OPTIONS, SOUND_OPTIONS, INSTRUCTIONS, EXIT };
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
