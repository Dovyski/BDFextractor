#include <iostream>
#include <string.h>

#include "mainwindow.h"
#include "signals_dialog.h"
#include "viewcurve.h"
#include "ecg_export.h"
#include "statistics_dialog.h"
#include "jump_dialog.h"
#include "global.h"
#include "third_party/getopt/getopt.h"

void copyright() {
	std::cout << "BDFextractor - Version " << VERSION << " (build: " << __DATE__ << ")" << std::endl;
	std::cout << "Copyright (c) 2016, Fernando Bevilacqua <fernando.bevilacqua@his.se>" << std::endl << std::endl;
}

void usage() {
	std::cout << "Usage:\n  BDFextractor.exe [options]" << std::endl << std::endl;
	std::cout << "Options:" << std::endl;

	std::cout << " --input               Path to the EDF/BDF file to be analyzed." << std::endl;
	std::cout << std::endl;

	std::cout << " -b, --subject         An integer used as the subject id in" << std::endl;
	std::cout << "                       the output." << std::endl;
	std::cout << std::endl;

	std::cout << " -l, --label           A string used as the label for each entry" << std::endl;
	std::cout << "                       int the output." << std::endl;
	std::cout << std::endl;

	std::cout << " -o, --output          Path to the output file. If nothing is specified" << std::endl;
	std::cout << "                       exported data is printed to stdout." << std::endl;
	std::cout << std::endl;

	std::cout << " -s, --signal          Id of the signal to be used in all calculations." << std::endl;
	std::cout << "                       Use --show-signals to list all available signals." << std::endl;
	std::cout << std::endl;

	std::cout << " -f, --show-info       Show information regarding the file, e.g. available." << std::endl;
	std::cout << "                       signals. Use this option to identify which signal you want to" << std::endl;
	std::cout << "                       use with --signal." << std::endl;
	std::cout << std::endl;

	std::cout << " -t, --show-hr-stats   Print some statistics regarding HR, e.g. mean HR." << std::endl;
	std::cout << "                       Calculations are based on signal informed by --signal." << std::endl;
	std::cout << std::endl;

	std::cout << " --export-rr           Extract RR values from input file/signal." << std::endl;
	std::cout << "                       Calculations are based on signal informed by --signal." << std::endl;
	std::cout << std::endl;

	std::cout << " --export-rr-times     Extract the time of each RR value from input file/signal." << std::endl;
	std::cout << "                       Calculations are based on signal informed by --signal." << std::endl;
	std::cout << std::endl;

	std::cout << " --export-hr-all       Extract HR values from input file/signal, including" << std::endl;
	std::cout << "                       time and timestamp. Calculations are based on signal" << std::endl;
	std::cout << "                       informed by --signal." << std::endl;
	std::cout << std::endl;

	std::cout << " --export-hr           Extract HR values from input file/signal." << std::endl;
	std::cout << "                       Calculations are based on signal informed by --signal." << std::endl;
	std::cout << std::endl;

	std::cout << " --export-timestamp    Add the timestamp of each value extracted from input" << std::endl;
	std::cout << "                       file/signal. Calculations are based on signal informed" << std::endl;
	std::cout << "                       by --signal." << std::endl;
	std::cout << std::endl;

	std::cout << " --export-time         Add the time (in seconds) of each value extracted from" << std::endl;
	std::cout << "                       input file/signal. Time starts at 0. Calculations are" << std::endl;
	std::cout << "                       based on signal informed by --signal." << std::endl;
	std::cout << std::endl;
}

int main(int argc, const char *argv[])
{
	bool help = getarg(false, "-h", "--help", "-?") || argc == 1;

	if (help) {
		copyright();
		usage();
		exit(1);
	}
	
	config_t config;

	config.subject_id		= getarg(-1, "-b", "--subject");
	config.label			= getarg("unknown", "--label");
	config.input_file		= getarg("", "--input");
	config.output_file		= getarg("", "-o", "--output");
	config.signal			= getarg(-1, "-s", "--signal");
	config.export_rr		= getarg(false, "--export-rr");
	config.export_rr_times	= getarg(false, "--export-rr-times");
	config.export_hr		= getarg(false, "--export-hr");
	config.export_timestamp = getarg(false, "--export-timestamp");
	config.export_time		= getarg(false, "--export-time");
	
	bool show_hr_stats		= getarg(false, "-t", "--show-hr-stats");
	bool show_file_info		= getarg(false, "-f", "--show-info");

	// Were we told to export everything regarding HR?
	if (getarg(false, "--export-hr-all")) {
		config.export_hr = true;
		config.export_timestamp = true;
		config.export_time = true;
	}

	// Check if the user told the program to export HR and RR at the same time.
	if (config.export_rr && config.export_hr) {
		printf("It is not possible to export HR and RR data at the same time.");
		exit(1);
	}

	UI_Mainwindow mainwindow;
	mainwindow.open_new_file(config.input_file.c_str());
	
	UI_Signalswindow signalsw(&mainwindow);

	if (show_file_info) {
		signalsw.show_signals(0);
		exit(0);
	}

	signalsw.DisplayButtonClicked(config.signal);

	ViewCurve viewcurve(&mainwindow);
	viewcurve.ECGdetectButton();

	UI_ECGExport ecgexport(&mainwindow);
	ecgexport.Export_RR_intervals(&config);

	if (show_hr_stats) {
		UI_StatisticWindow show_stats_window(mainwindow.signalcomp[1], mainwindow.pagetime);
		exit(0);
	}

	exit(0);
}