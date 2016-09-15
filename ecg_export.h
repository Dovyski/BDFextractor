/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2011, 2012, 2013, 2014, 2015, 2016 Teunis van Beelen
*
* Email: teuniz@gmail.com
*
***************************************************************************
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************
*/




#ifndef ECGEXPORTCLASS_H
#define ECGEXPORTCLASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "mainwindow.h"
#include "viewcurve.h"
#include "utils.h"
//#include "filteredblockread.h"
//#include "edf_annot_list.h"

class UI_Mainwindow;
class ViewCurve;

class UI_ECGExport
{
public:
  UI_ECGExport(UI_Mainwindow *parent);
  UI_Mainwindow *mainwindow;
  void Export_RR_intervals(config_t *config);

private:
	void output_hr_entry(FILE * file, config_t * config, int subject, long long timestamp, int time, int hr, const char * label);
	void load_signalcomps(void);
};

#endif




