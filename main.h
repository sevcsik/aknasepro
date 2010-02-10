#ifndef MAIN_H
#define MAIN_H

/** \file main.h
  * main fuggveny es hasonlok
  *
  * Forditaskor definialni kell a POSIX vagy a WIN32 makrot, platformtol
  * fuggoen.
  */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef POSIX
#  include <termios.h>           /**< POSIX eseten terminalvezerleshez */
#endif

#define NAME	   "Aknasepro"    /**< Program neve                            */
#define VER	      "v1.0"         /**< Verzio                                  */
#define DWIDTH    30             /**< Alapertelmezett tablaszelesseg          */
#define DHEIGHT   18             /**< Alapertelmezett tablamagassag           */
#define DMINES    20             /**< Alapertelmezett aknaszam                */
#define CELL_MINE -1             /**< Specialis ertek az aknara               */

/** Koordinata tipus */
typedef struct Coord_ 
{
   int x;                        /**< Oszlop                                  */
   int y;                        /**< Sor                                     */
} Coord;

/** Ebben a tipusban taroljuk a beallitasokat */
typedef struct Conf_
{
   char width;                   /**< Tabla szelessege (mezo)                 */
   char height;                  /**< Tabla magassaga (mezo)                  */
   time_t start_time;            /**< A jatek inditasanak ideje               */
   Coord cursor;                 /**< Felhasznaloi kurzor pozicioja           */
   char first;                   /**< Az elso lepes lenullazza                */
   unsigned mines;               /**< Aknak szama                             */
   char iddqd;                   /**< Isten mod                               */
   char dead;                    /**< Meg vagyunk -e halva?                   */
   char win;                     /**< Nyertunk -e?                            */
   signed char saved;            /**< Sikeresen elmentettuk a jatekot?        */
   char *file;                   /**< Fajl amibe mentunk, vagy betoltjuk      */
   unsigned flags;               /**< Hany zaszlonk van meg                   */
#ifdef POSIX
   struct termios old_settings;  /**< POSIX regi terminalbeallitasok
                                      a visszaallitashoz                      */
#endif
} 
Conf;

/** Aknak es szamok
  *
  * Ebben a tipusban taroljuk a mezokben a szamot/aknat 
  */
typedef signed char Cell;

/** Felfedettseg 
  * 
  * Ebben a tipusban taroljuk, h felfedtuk -e mar a mezot 
  */
typedef char CellState;

#define KNOWN 0      /**< Felfedettseg: mar felfedtuk */
#define UNKNOWN 1    /**< Felfedettseg: meg nem fedtuk fel */
#define FLAGGED 2    /**< Felfedettseg: zaszlot tettunk ra */

/** Kiirja a helyes hasznalatot
  *
  * \param argv main argv argumentuma 
  */ 
void print_help(char **argv);

/** Feltolti a tablat aknakkal es szamokkal
  *
  * Conf-bol megallapitja az aknak szamat, es annyi aknat helyez el
  * veletlenszeruen a tablan. A kurzor ala nem tesz aknat.
  * \param conf Konfiguracios struktura
  * \param table Tabla pointer
  */
void gen_table(Conf *conf, Cell **table);

/** Ellenorzi, hogy nyertunk -e
  *
  * Vegigfut a tombon, es ellenorzi, hogy van -e olyan akna, ami nincs
  * megzaszlozva. Ha igen, akkor meghalt a user. Csak akkor hivjuk meg, ha
  * elfogytak a zaszlok.
  *
  * \param conf Konfiguracios struktura
  * \param table Tabla pointer
  * \param states felfedettsegi tabla pointer
  */
void check(Conf *conf, Cell **table, CellState **states);

/** Felfedes fuggveny
  *
  * \param conf Konfiguracios struktura
  * \param table Tabla pointer
  * \param states Felfedettsegi tabla pointer
  * \param self Sajat koordinatak
  *
  * Felfedi a szomszedos cellakat, amennyiben talal a kornyezeteben annyi
  * zaszlot, amekkora az erteke. (rekurziv)
  */
void reveal(Conf *conf, Cell **table, CellState **states, Coord self);

/** main fuggveny
  *
  * Ebben a fuggvenyben ertelmezzuk (vagy nem) az argumentumokat, es lefoglaljuk
  * a jatekmezohoz szukseges memoriat, beallitjuk a terminalt, majd elinditjuk 
  * az esemenykezelo ciklust. 
  * \param argc argumentumok szama
  * \param argv argumentumok tombje 
  */
int main(int argc, char **argv);

#include "terminal.h"
#include "control.h"

#endif

