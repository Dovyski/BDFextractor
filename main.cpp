#include <iostream>
#include <string.h>

#include "mainwindow.h"
#include "signals_dialog.h"
#include "viewcurve.h"
#include "ecg_export.h"
#include "statistics_dialog.h"
#include "jump_dialog.h"
#include "third_party/getopt/getopt.h"

#define VERSION	"1.0.0"

typedef struct {
	int subject_id;
	std::string label;
	std::string input_file;
	std::string output_file;
	int signal;
	bool export_rr;
	bool export_rr_times;
	bool export_hr;
	bool export_timestamp;
	bool export_time;
} config_t;

void copyright() {
	std::cout << "BDFextractor - Version " << VERSION << " (build: " << __DATE__ << ")" << std::endl;
	std::cout << "Copyright (c) 2016, Fernando Bevilacqua <fernando.bevilacqua@his.se>" << std::endl << std::endl;
}

void usage() {
	std::cout << "Usage:\n  BDFextractor.exe [options]" << std::endl << std::endl;
	std::cout << "Options:" << std::endl;

	std::cout << " -b, --subject         An integer used as the subject id in" << std::endl;
	std::cout << "                       the output." << std::endl;
	std::cout << std::endl;

	std::cout << " -l, --label           A string used as the label for each entry" << std::endl;
	std::cout << "                       int the output." << std::endl;
	std::cout << std::endl;

	std::cout << " -i, --input           Path to the EDF/BDF file to be analyzed." << std::endl;
	std::cout << std::endl;

	std::cout << " -o, --output          Path to the output file. If nothing is informed," << std::endl;
	std::cout << "                       content is printed to stdout." << std::endl;
	std::cout << std::endl;

	std::cout << " -s, --signal          Id of the signal to be used in all calculations." << std::endl;
	std::cout << "                       Use --show-signals to list all available signals." << std::endl;
	std::cout << std::endl;

	std::cout << " -g, --show-signals    Show which signals are available in the input file." << std::endl;
	std::cout << "                       Use this option to identify which signal you want to" << std::endl;
	std::cout << "                       use with --signal." << std::endl;
	std::cout << std::endl;

	std::cout << " -f, --show-file-info  Show information regarding the file, e.g. date." << std::endl;
	std::cout << "                       Calculations are based on signal informed by --signal." << std::endl;
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
		exit(2);
	}
	
	config_t config;

	config.subject_id		= getarg(-1, "-b", "--subject");
	config.label			= getarg("unknown", "-l", "--label");
	config.input_file		= getarg("Part_1_S_Trial1_emotion.bdf", "-i", "--input");
	config.output_file		= getarg("", "-o", "--output");
	config.signal			= getarg(0, "-s", "--signal");
	config.export_rr		= getarg(false, "--export-rr");
	config.export_rr_times	= getarg(false, "--export-rr-times");
	config.export_hr		= getarg(false, "--export-hr");
	config.export_timestamp = getarg(false, "--export-timestamp");
	config.export_time		= getarg(false, "--export-time");
	
	bool show_signals		= getarg(false, "-g", "--show-signals");
	bool show_hr_stats		= getarg(false, "-t", "--show-hr-stats");
	bool show_file_info		= getarg(false, "-f", "--show-file-info");

	UI_Mainwindow mainwindow;
	mainwindow.open_new_file(config.input_file.c_str());
	
	UI_Signalswindow signalsw(&mainwindow);

	if (show_signals) {
		signalsw.show_signals(0);
		exit(0);
	}
	
	signalsw.DisplayButtonClicked();

	ViewCurve viewcurve(&mainwindow);
	viewcurve.ECGdetectButton();

	UI_ECGExport ecgexport(&mainwindow);
	ecgexport.Export_RR_intervals();

	if (show_hr_stats) {
		UI_StatisticWindow show_stats_window(mainwindow.signalcomp[1], mainwindow.pagetime);
		exit(0);
	}

	getchar();
	exit(0);
}