#ifndef TERMINAL_H
#define TERMINAL_H

/** \file terminal.h
  * Terminal inicializalasara szolgalo fuggvenyek 
  */

#include "main.h"

#ifdef POSIX
#  define FORMAT_BOLD "\033[1m"             /**< Felkover betu                */
#  define FORMAT_NORMAL "\033[0m"           /**< Normal betu                  */
#  define FORMAT_CUSTOM(X) "\033[" #X "m"   /**< Egyeni formazas              */
#  define FORMAT_RESET "\033[m"             /**< Formazas visszaallitasa      */
#else
#  define FORMAT_BOLD ""                    /**< Felkover betu                */
#  define FORMAT_NORMAL ""                  /**< Normal betu                  */
#  define FORMAT_CUSTOM(X) ""               /**< Egyeni formazas              */
#  define FORMAT_RESET ""                   /**< Formazas visszaallitasa      */
#endif

#define UNKNOWN_STR "_"                   /**< Fel nem fedett mezo jelolese   */
#define MINE_STR FORMAT_CUSTOM(31;1) "*"  /**< Akna jelolese                  */
#define FLAG_STR FORMAT_CUSTOM(31;1) "F"  /**< Zaszlo jelolese                */

/** Fejlec szovege */
#define HEADER FORMAT_CUSTOM(1;34) "%s %s\n\n" FORMAT_RESET
#define HEADER_HEIGHT 2                   /**< Fejlec magassaga (sor)         */

/** Lablec szovege */
#define FOOTER "\nFelfedes: SPACE | Zaszlo: F | Mozgas: kurzorbillentyuk\n"\
               "Mentes: SHIFT+S | Kilepes: SHIFT+Q\n%ld:%02ld\n"
#define FOOTER_HEIGHT 4                   /**< Lablec magassaga (sor)         */

#define DELAY 40000                       /**< Kirajzolasok kozti kesleltetes
                                               us-ban (=25 FPS)               */

/** Iranyok a kurzormozgatashoz (vagy mashoz akar) */
typedef enum Direction_ 
{
   UP = 'A',   /**< Fel       */
   DOWN = 'B', /**< Le        */
   LEFT = 'C', /**< Balra     */
   RIGHT = 'D' /**< Jobbra    */
} Direction;

/** Kiirjuk a kezdokepernyot
  * \param conf Konfiguracios struktura
  * \param table Tabla pointer 
  * \param states Felfedettsegi tabla
  */

void terminal_init(Conf *conf, Cell **table, CellState **states);

/** Kirajzoljuk a kepernyot
  * \param conf Konfiguracios struktura
  * \param table Tabla pointer
  * \param states Felfedettsegi tabla pointer
  *
  * TODO: leiras 
  */
void render(Conf *conf, Cell **table, CellState **states);

/** Terminal kurzor mozgatasa
  *
  * \param steps Lepesek szama
  * \param dir Irany
  */
void cursor_move(signed char steps, Direction dir);


/** Meghalas fuggveny
  * 
  * \param conf Konfiguracios struktura
  * \param table Tabla pointer
  * \param states Felfedettsegi tabla pointer
  *
  * Amennyiben a felhasznalo elhalalozasanak esete forog fenn, meghivjuk ezt a
  * fuggvenyt, es o felfedette allitja az osszes mezot.
  */
void kaboom(Conf *conf, Cell **table, CellState **states);

#endif

