#include "main.h"

int main(int argc, char **argv)
{
   Conf conf;           /* Beallitasok  */
   Cell **table;        /* Aknak/szamok */
   CellState **states;  /* Felfedettseg */
   int i, j;
   
   /* Tablameret megallapitasa */
   if (argc > 2)
   {
      conf.width = atoi(argv[1]);
      conf.height = atoi(argv[2]);
      if (!conf.width || !conf.height) 
      {
         print_help(argv);
         return -1;
      }
      if (argc > 3)
         conf.mines = atoi(argv[3]);
      else conf.mines = DMINES;
   }
   else
   {
      conf.width = DWIDTH;
      conf.height = DHEIGHT;
      conf.mines = DMINES;
   }

   if (argc == 2) conf.file = argv[1];
   else conf.file = "aknasepro.save";
   
   if (conf.mines >= conf.height * conf.width)
   {
      printf("Bocsi, de nem fer el ennyi akna a tablan :)\n");
      return -2;
   }
   
   /* Meg minden zaszlonk megvan */
   conf.flags = conf.mines;
   
   /* Kezdo ido rogzitese */
   conf.start_time = time(NULL);
   
   /* Elso lepes! */
   conf.first = 1;
   
   /* Adunk eselyt a usernek, hogy onszantabol haljon meg */
   conf.dead = 0;
   
   /* De ne is nyerjen rogton... */
   conf.win = 0;
   
   /* Isten mod kikapcsolasa */
   conf.iddqd = 0;
   
   /* Kozeprol kezdunk */
   
   conf.cursor.x = conf.width / 2;
   conf.cursor.y = conf.height / 2;
   
   
   if (!load_state(&conf, &table, &states))
   {
      /* Tablak lefoglalasa, ha nincs betoltendo jatek */
      if (!(table = malloc(sizeof(Cell *) * conf.height)))
      {
            fprintf(stderr, "out of memory :/\n");
            exit(-1);
      }
      if (!(states = malloc(sizeof(CellState *) * conf.height)))
      {
            fprintf(stderr, "out of memory :/\n");
            exit(-1);
      }
      for (i = 0; i < conf.height; i++)
      {
         if (!(table[i] = malloc(sizeof(Cell) * conf.width)))
         {
            fprintf(stderr, "out of memory :/\n");
            exit(-1);
         }
         if (!(states[i] = malloc(sizeof(CellState) * conf.width)))
         {
            fprintf(stderr, "out of memory :/\n");
            exit(-1);
         }
         for (j = 0; j < conf.width; j++)
         {
            table[i][j] = 0;
            states[i][j] = UNKNOWN;
         }
      }
   }   
   /* Kezdokepernyo kirajzolasa */
   terminal_init(&conf, table, states);
   
   /* Main loop inditasa */
   main_loop(&conf, table, states);
   
   /* Tablak felszabaditasa */
   for (i = 0; i < conf.height; i++)
   {
      free(table[i]);
      free(states[i]);
   }
   free(table);
   free(states);
   
   /* Terminal visszaallitasa */
#ifdef POSIX
   tcsetattr(0, TCSANOW, &(conf.old_settings));
#endif

   return 0;
}

void print_help(char **argv)
{
   printf("%s %s\nHasznalat: %s [SZELESSEG] [MAGASSAG] [AKNAK SZAMA]\n"
   "vagy: %s [FAJLNEV] (beolvasasra/mentesre)\n", NAME, VER, argv[0], argv[0]);
}

void gen_table(Conf *conf, Cell **table)
{
   int x, y, i = 0, j;
   
   /* Beteszunk a kurzor ala egy aknat, amit a vegen kiveszunk */
   table[conf->cursor.y][conf->cursor.x] = CELL_MINE;
   
   srand((unsigned) time(NULL)); /* uj random seed */
   
   while (i < conf->mines)
   {
      x = (unsigned) rand() % conf->width;
      y = (unsigned) rand() % conf->height;
      
      if (table[y][x] == CELL_MINE) continue;
      else
      {
         table[y][x] = CELL_MINE;
         i++;
      }
   }
   
   table[conf->cursor.y][conf->cursor.x] = 0;
   
   /* Teleirjuk a tablat szamokkal */
   for (i = 0; i < conf->height; i++)
   {
      for (j = 0; j < conf->width; j++)
      {
         if (table[i][j] == CELL_MINE) continue; /* Aknat nem bantjuk */
         if (i - 1 >= 0)
         {
            if (j - 1 >= 0 && table[i - 1][j - 1] == CELL_MINE) table[i][j]++;
            if (table[i - 1][j] == CELL_MINE) table[i][j]++;
            if (j + 1 < conf->width && table[i - 1][j + 1] == CELL_MINE) 
               table[i][j]++;
         }
         
         if (j - 1 >= 0 && table[i][j - 1] == CELL_MINE) table[i][j]++;
         if (j + 1 < conf->width && table[i][j + 1] == CELL_MINE) table[i][j]++;
         
         if (i + 1 < conf->height)
         {
            if (j - 1 >= 0 && table[i + 1][j - 1] == CELL_MINE) table[i][j]++;
            if (table[i + 1][j] == CELL_MINE) table[i][j]++;
            if (j + 1 < conf->width && table[i + 1][j + 1] == CELL_MINE) 
               table[i][j]++;
         }
      }
   }
}

void check(Conf *conf, Cell **table, CellState **states)
{
   unsigned i, j;
   
   for (i = 0; i < conf->height; i++)
   {
      for (j = 0; j < conf->width; j++)
      {
         if (states[i][j] != FLAGGED) states[i][j] = KNOWN;
         else if (table[i][j] != CELL_MINE)
         {
            kaboom(conf, table, states);
            return;
         }
      }
   }
   conf->win = 1;
}

void reveal(Conf *conf, Cell **table, CellState **states, Coord self)
{
   char flags = 0;
   Coord tmp;
   
   /* Felfedjuk onmagunkat */
   if (states[self.y][self.x] == UNKNOWN) states[self.y][self.x] = KNOWN;
   
   /* Zaszlok megszamolasa a kornyezetben */
   if (self.y - 1 >= 0)
   {
      if (self.x - 1 >= 0 && states[self.y- 1][self.x - 1] == FLAGGED) 
         flags++;
      if (states[self.y - 1][self.x] == FLAGGED) flags++;
      if (self.x + 1 < conf->width && states[self.y - 1][self.x + 1] 
                                       == FLAGGED) 
         flags++;
   }
   
   if (self.x - 1 >= 0 && states[self.y][self.x - 1] == FLAGGED) 
      flags++;
   if (self.x + 1 < conf->width && states[self.y][self.x + 1] == FLAGGED) 
      flags++;
   
   if (self.y + 1 < conf->height)
   {
      if (self.x - 1 >= 0 && states[self.y + 1][self.x - 1] == FLAGGED) 
         flags++;
      if (states[self.y + 1][self.x] == FLAGGED) flags++;
      if (self.x + 1 < conf->width && states[self.y + 1][self.x + 1] 
                                       == FLAGGED) 
         flags++;
   }
   
   /* Van eleg flag? */
   if (flags >= table[self.y][self.x])
   {
      /* Ha igen, meghivjuk magunkat felfedetlen szomszedainkra is! */
      if (self.y - 1 >= 0)
      {
         if (self.x - 1 >= 0 && states[self.y - 1][self.x - 1] == UNKNOWN) 
         {
            tmp.y = self.y - 1;
            tmp.x = self.x - 1;
            reveal(conf, table, states, tmp);
         }
         if (states[self.y - 1][self.x] == UNKNOWN)
         {
            tmp.y = self.y - 1;
            tmp.x = self.x;
            reveal(conf, table, states, tmp);
         }
         if (self.x + 1 < conf->width && states[self.y - 1][self.x + 1] 
                                          == UNKNOWN) 
         {
            tmp.y = self.y - 1;
            tmp.x = self.x + 1;
            reveal(conf, table, states, tmp);
         }
      }
      if (self.x - 1 >= 0 && states[self.y][self.x - 1] == UNKNOWN)
      {
         tmp.y = self.y;
         tmp.x = self.x - 1;
         reveal(conf, table, states, tmp);
      }
      if (self.x + 1 < conf->width && states[self.y][self.x + 1] 
                                          == UNKNOWN)
      {
         tmp.y = self.y;
         tmp.x = self.x + 1;
         reveal(conf, table, states, tmp);
      }
      if (self.y + 1 < conf->height)
      {
         if (self.x - 1 >= 0 && states[self.y + 1][self.x - 1] == UNKNOWN)
         {
            tmp.y = self.y + 1;
            tmp.x = self.x - 1;
            reveal(conf, table, states, tmp);
         }
         if (states[self.y + 1][self.x] == UNKNOWN)
         {
            tmp.y = self.y + 1;
            tmp.x = self.x;
            reveal(conf, table, states, tmp);
         }
         if (self.x + 1 < conf->width && states[self.y + 1][self.x + 1] 
                                          == UNKNOWN)
         {
            tmp.y = self.y + 1;
            tmp.x = self.x + 1;
            reveal(conf, table, states, tmp);
         }
      }
   }

}


