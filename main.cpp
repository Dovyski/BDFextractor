#include <iostream>
#include <string.h>

#include "mainwindow.h"
#include "signals_dialog.h"
#include "viewcurve.h"
#include "ecg_export.h"
#include "statistics_dialog.h"

#define VERSION	"1.0.0"

void copyright() {
	std::cout << "BDFextractor - Version " << VERSION << " (build: " << __DATE__ << ")" << std::endl;
	std::cout << "Copyright (c) 2016, Fernando Bevilacqua <fernando.bevilacqua@his.se>" << std::endl << std::endl;
}

void usage() {
	std::cout << "Usage: BDFextractor.exe [options]" << std::endl << std::endl;
	std::cout << "options:" << std::endl << std::endl;

	std::cout << " --option            Info about option." << std::endl;
	std::cout << " --option2           Another info this option as well." << std::endl;
}

int main(int argc, const char *argv[])
{
	if (argc > 1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
		copyright();
		usage();
		exit(2);
	}

	UI_Mainwindow mainwindow;

	mainwindow.open_new_file();
	UI_Signalswindow signalsw(&mainwindow);

	signalsw.show_signals(0);
	signalsw.DisplayButtonClicked();

	ViewCurve viewcurve(&mainwindow);
	viewcurve.ECGdetectButton();

	UI_ECGExport ecgexport(&mainwindow);
	ecgexport.Export_RR_intervals();

	UI_StatisticWindow show_stats_window(mainwindow.signalcomp[1], mainwindow.pagetime);

	std::cout << "Hello world!" << std::endl;
	getchar();

	exit(0);
}