#include "file.h"

char load_state(Conf *conf, Cell ***table_, CellState ***states_)
{
   FILE *fp;
   time_t duration;
   unsigned i;
   char buf[sizeof(MAGIC_NUMBER)];
   Cell **table;
   CellState **states;
   
   fp = fopen(conf->file, "rb");
   if (!fp) return 0;
   
   fread(buf, strlen(MAGIC_NUMBER), 1, fp);
   buf[sizeof(MAGIC_NUMBER) - 1] = 0;
   if (strcmp(buf, MAGIC_NUMBER)) return 0;
   
   fread(&conf->height, sizeof(conf->height), 1, fp);
   fread(&conf->width, sizeof(conf->width), 1, fp);
   fread(&conf->mines, sizeof(conf->mines), 1, fp);
   fread(&conf->flags, sizeof(conf->flags), 1, fp);
   fread(&duration, sizeof(duration), 1, fp);
   
   /* DEBUG */
   /*
   fprintf(stderr, "h: %d\n", conf->height);
   fprintf(stderr, "w: %d\n", conf->width);
   fprintf(stderr, "m: %d\n", conf->mines);
   fprintf(stderr, "f: %d\n", conf->flags);
   fprintf(stderr, "d: %d\n", duration);
   */
   conf->start_time = time(NULL) - duration;
   
   /* Itt mallocoljuk a tablakat, mivel a main() nem tudja meg h mekkorak */
   
   table = malloc(sizeof(Cell *) * conf->height);
   states = malloc(sizeof(CellState *) * conf->height);
   
   if (!table || !states)
   {
         fprintf(stderr, "out of memory :/\n");
         exit(-1);
   }
   
   for (i = 0; i < conf->height; i++)
   {
      table[i] = malloc(sizeof(Cell) * conf->width);
      states[i] = malloc(sizeof(CellState) * conf->width);
      if (!table[i] || !states[i])
      {
         fprintf(stderr, "out of memory :/\n");
         exit(-1);
      }
      
      fread(table[i], sizeof(Cell), conf->width, fp);
      fread(states[i], sizeof(CellState), conf->width, fp);
   }
   
   conf->first = 0;
   
   *table_ = table;
   *states_ = states;
   
   return 1;
}

signed char save_state(Conf *conf, Cell **table, CellState **states)
{
   FILE *fp;
   time_t duration;
   unsigned i;
   
   duration = time(NULL) - conf->start_time; /* Jatekido */
   
   fp = fopen(conf->file, "wb");
   if (!fp) return -1;
   
   /* Az strlen miatt nem taroljuk a 0-t, juhhu! */
   fwrite(MAGIC_NUMBER, strlen(MAGIC_NUMBER), 1, fp);
   
   fwrite(&conf->height, sizeof(conf->height), 1, fp);
   fwrite(&conf->width, sizeof(conf->width), 1, fp);
   fwrite(&conf->mines, sizeof(conf->mines), 1, fp);
   fwrite(&conf->flags, sizeof(conf->flags), 1, fp);
   fwrite(&duration, sizeof(duration), 1, fp);
   
   for (i = 0; i < conf->height; i++) 
   {
      fwrite(table[i], sizeof(Cell), conf->width, fp);
      fwrite(states[i], sizeof(CellState), conf->width, fp);
   }   
   fclose(fp);
 
   return 1;
}

