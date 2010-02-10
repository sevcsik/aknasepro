#ifndef FILE_H
#define FILE_H

/** \file file.h
  * Filekezelesre valo fuggvenyek
  */
  
#include "main.h"

#define MAGIC_NUMBER "!#SA" /**< Igy kezdodik egy aknasepro file */

/** Megprobalja betolteni a jatekallapotot egy filebol
  *
  * \param conf Konfiguracios struktura
  * \param table_ Tabla pointer pointer
  * \param states_ Felfedettsegi tabla pointer pointer
  *
  * Ha nem sikerul, uj jatekot kezdunk.
  */
char load_state(Conf *conf, Cell ***table_, CellState ***states_);

/** Megprobalja elmenteni a jatekot
  *
  * \param conf Konfiguracios struktura
  * \param table Tabla pointer
  * \param states Felfedettsegi tabla pointer
  *
  * Ha nem sikerul, kiirjuk, de a user benezte mert menet kozben nem tudja
  * megvaltoztatni a fajlnevet! ahahah
  *
  * 1-et adunk vissza sikeres menteskor, -1-et sikertelennel, mert a 0-t a
  * nem mentett jatek jelzesere tartjuk fenn.
  */
signed char save_state(Conf *conf, Cell **table, CellState **states);

#endif
