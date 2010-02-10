#include "terminal.h"

void render(Conf *conf, Cell **table, CellState **states)
{
   unsigned i, j;
   char kaboom_flag = 0;
   time_t current_time;
   
   /* Visszaugrasszuk a kurzort az elejere (win eseten kepernyot torlunk)*/
#ifdef POSIX
   cursor_move(conf->height + HEADER_HEIGHT + FOOTER_HEIGHT, UP);
#else
   system("cls");
#endif

   printf(HEADER, NAME, VER); /* Fejlec kiirasa */
   
   for (i = 0; i < conf->height; i++)
   {
      for (j = 0; j < conf->width; j++)
      {
         if (conf->cursor.y == i && conf->cursor.x == j)
            printf(FORMAT_CUSTOM(47));
         switch (states[i][j])
         {
            case FLAGGED:
               printf(FLAG_STR);
               break;
            case KNOWN:
               if (table[i][j] == CELL_MINE)
               {
                  printf(MINE_STR);
                  /* Csak egyszer fusson le kaboom() */
                  if (!conf->iddqd && !conf->dead)
                  {
                     kaboom(conf, table, states);
                     kaboom_flag = 1;
                  }
               }
               else if (table[i][j] == 0)
                  putchar(' ');
               else
                  printf("\033[%dm%d", (36 - (table[i][j]) % 5) /* szam szerint
                         szinezzuk, de csak 5 szint hasznalunk */, table[i][j]);
                         
               break;
            default:
               putchar('_');
               break;
         }
         printf(FORMAT_RESET);
         putchar(' ');
      }
      
      if (i == 1) printf("   Zaszlok: %02d", conf->flags);
      
      if (i == 2 && conf->dead) printf(FORMAT_CUSTOM(31;1) 
                                       "   Sajnalom, de megmurdaltal"
                                       FORMAT_RESET);
      else if (i == 2 && conf->win) printf(FORMAT_CUSTOM(32;1)
                                           "   Gratulalok, nyertel!"
                                           FORMAT_RESET);
      
      if (i == 3 && conf->iddqd) printf(FORMAT_CUSTOM(31;1) "   ISTEN MOD"
                                        FORMAT_RESET);
                                        
      if (i == 4 && conf->saved == 1)
         printf(FORMAT_CUSTOM(32;1) "   Sikeres mentes: %s" 
                FORMAT_RESET, conf->file);
      else if (i == 4 && conf->saved == -1)
         printf(FORMAT_CUSTOM(31;1) "   Sikertelen mentes: %s" 
                FORMAT_RESET, conf->file);
      putchar('\n');
   }
   current_time = time(NULL);
   printf(FOOTER, (current_time - conf->start_time) / 60, 
         (current_time - conf->start_time) % 60);
 
   /* Ha lefutott kaboom, ujrarajzoljuk a screent */
   if (kaboom_flag) render(conf, table, states); 
}

void cursor_move(signed char steps, Direction dir)
{
   printf("\033[%d%c", steps, dir);
}

void terminal_init(Conf *conf, Cell **table, CellState **states)
{
   int i;
#ifdef POSIX
   struct termios new_settings; /* Uj terminalbeallitasok */
#endif

   /* "Lefoglaljuk" a kepernyoteruletet */
   for (i = 0; i < conf->height + HEADER_HEIGHT + FOOTER_HEIGHT; i++)
      putchar('\n');

#ifdef POSIX
   tcgetattr(0, &conf->old_settings); /* Terminalbeallitasok elmentese */
   new_settings = conf->old_settings;
   /* Echo, sorkezeles, es input ertelmezes kikapcsolasa */
   new_settings.c_lflag &= ~(ECHO | ICANON);
   /* 1 karakteres puffer */
   new_settings.c_cc[VTIME] = 0;
   new_settings.c_cc[VMIN] = 0;
   
   tcsetattr(0, TCSANOW, &new_settings);
#endif
   
   render(conf, table, states);
}

void kaboom(Conf *conf, Cell **table, CellState **states)
{
   unsigned i, j;
   for (i = 0; i < conf->height; i++)
   {
      for (j = 0; j < conf->width; j++)
      {
         states[i][j] = KNOWN;
      }
   }
   conf->dead = 1;
}

