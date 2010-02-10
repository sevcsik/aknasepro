#include "control.h"

#define LONGBUF_SIZE 6

void main_loop(Conf *conf, Cell **table, CellState **states)
{
   char buf[4];
   static char longbuf[LONGBUF_SIZE]; 
   char ok;
   unsigned i;
   
   while(1)
   {
      ok = 0;
      
      /* Billentyuparancsok beolvasasa */
      if (!fgets(buf, 5, stdin)) continue;
      
      /* Harom karakteres kodok */
      buf[3] = 0;
      
      if (!strcmp(buf, "\033[A")) /* Felfele nyil */
      {
         conf->cursor.y = (int) conf->cursor.y - 1 >= 0 ? conf->cursor.y - 1 
                                                        : conf->height - 1;
         ok = 1;
      }
      else if (!strcmp(buf, "\033[B")) /* Lefele nyil */
      {
         conf->cursor.y = (conf->cursor.y + 1) % conf->height;
         ok = 1;
      }         
      else if (!strcmp(buf, "\033[C")) /* Jobbra nyil */
      {
         conf->cursor.x = (conf->cursor.x + 1) % conf->width;
         ok = 1;
      }         
      else if (!strcmp(buf, "\033[D")) /* Balra nyil */
      {
         conf->cursor.x = (int) conf->cursor.x - 1 >= 0 ? conf->cursor.x - 1 
                                                        : conf->width - 1;
         ok = 1;
      }                                                        
      /* Ket karakteres kodok */
      buf[2] = 0;
      
      /* Egy karakteres kodok */
      if (buf[0] == ' ')
      {
         if (conf->first)
         {
            conf->first = 0;
            states[conf->cursor.y][conf->cursor.x] = KNOWN;
            /* Elso lepesnel generaljuk a tablat, h biztos ne lepjunk aknara */
            gen_table(conf, table);
         }
         else states[conf->cursor.y][conf->cursor.x] = KNOWN;
         reveal(conf, table, states, conf->cursor);
         ok = 1;
      }
      else if (buf[0] == 'f')
      {
         if (states[conf->cursor.y][conf->cursor.x] == UNKNOWN)
         { /* Lerakjuk a zaszlot */
            states[conf->cursor.y][conf->cursor.x] = FLAGGED;
            conf->flags--;     
         }
         else if (states[conf->cursor.y][conf->cursor.x] == FLAGGED)
         { /* Felszedjuk a zaszlot */
            states[conf->cursor.y][conf->cursor.x] = UNKNOWN;
            conf->flags++;         
         }
         if (conf->flags == 0) check(conf, table, states);
      }
      else if (buf[0] == 'S')
      {
         conf->saved = save_state(conf, table, states);
         if (conf->saved == 1)
         {
            render(conf, table, states);
            return;
         }
      }
      else if (buf[0] == 'Q') return; /* Kilepes */
      
      if (!ok) /* Ha nem sikerult ertelmezni, betesszuk az elso karaktert
                  longbufba */
      {
         /* Elshifteljuk eggyel longbufot, es az utolso karakter helyere
            beszurjuk */
         for (i = 0; i < LONGBUF_SIZE - 2; i++)
            longbuf[i] = longbuf[i + 1];
         longbuf[LONGBUF_SIZE - 2] = buf[0];
         longbuf[LONGBUF_SIZE - 1] = 0;
         
         /* Ellenorizzuk a tobb betus kodokat */
         if (!strcmp(longbuf, "iddqd"))
         {
            conf->iddqd = !conf->iddqd;
            ok = 1;
         }         
      }

      render(conf, table, states);
      if (conf->win || conf->dead) return;
   }
}

