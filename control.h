#ifndef CONTROL_H
#define CONTROL_H

/** \file control.h
  * Input ertelmezes/vegrehajtas
  */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "main.h"
#include "terminal.h"
#include "file.h"

/** Esemenykezelo ciklus
  * \param conf Konfiguracios struktura
  * \param table Tabla pointer
  * \param states Felfedettsegi tabla
  *
  * Ez a fuggveny elindit egy vegtelen ciklust, ami minden iteracional ellenorzi
  * az inputot, vegrehajtja a megfelelo reakciokat, majd kirajzolja a keprenyot.
  */
  
void main_loop(Conf *conf, Cell **table, CellState **states);

#endif
