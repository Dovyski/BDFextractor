# BDFextractor
A command-line tool to extract heart rate (HR) information from BDF/EDF files.

## Getting Started
### Instalation (Windows)

Download the latest release available [here](https://github.com/Dovyski/BDFextractor/releases). Unzip the file then copy `BDFextractor.exe` to a folder included in your command line path, e.g. `C:\Windows\system32\`.

### Usage

You can run `BDFextractor.exe` from the command line as:

```
BDFextractor.exe [options]
```

The available options are:

```
 --input=<val>         Path to the EDF/BDF file to be analyzed.

 --subject=<val>       An integer used as the subject id in
                       the output.

 --label=<val>         A string used as the label for each entry
                       int the output.

 --output=<val>        Path to the output file. If nothing is specified
                       exported data is printed to stdout.

 --signal=<val>         Id of the signal to be used in all calculations.
                       Use --show-signals to list all available signals.

 -f, --show-info       Show information regarding the file, e.g. available.
                       signals. Use this option to identify which signal you
                       want to use with --signal.

 -t, --show-hr-stats   Print some statistics regarding HR, e.g. mean HR.
                       Calculations are based on signal informed by --signal.

 --export-rr           Extract RR values from input file/signal.
                       Calculations are based on signal informed by --signal.

 --export-rr-times     Extract the time of each RR value from input file/signal.
                       Calculations are based on signal informed by --signal.

 --export-hr-all       Extract HR values from input file/signal, including
                       time and timestamp. Calculations are based on signal
                       informed by --signal.

 --export-hr           Extract HR values from input file/signal.
                       Calculations are based on signal informed by --signal.

 --export-timestamp    Add the timestamp of each value extracted from input
                       file/signal. Calculations are based on signal informed
                       by --signal.

 --export-time         Add the time (in seconds) of each value extracted from
                       input file/signal. Time starts at 0. Calculations are
                       based on signal informed by --signal.
```

### Usage examples

List available information contained in the file `my_file.bdf`:

```
BDFextractor.exe --input=my_file.bdf --show-info
```

The output of the command above is:

```
File: my_file.bdf
Subject: X X X X                                                                         
Start date (timestamp): 1247151754
Start date:  10 jul 2016   13:55:18
Duration:  0:02:00

Available signals:
ID  LABEL              SAMPLING
0   Fp1                256 Hz 
1   AF3                256 Hz 
2   F3                 256 Hz 
3   F7                 256 Hz 
4   FC5                256 Hz 
5   FC1                256 Hz 
6   C3                 256 Hz 
7   T7                 256 Hz 
8   CP5                256 Hz 
9   CP1                256 Hz 
10  P3                 256 Hz 
11  P7                 256 Hz 
12  PO3                256 Hz 
13  O1                 256 Hz 
14  Oz                 256 Hz 
15  Pz                 256 Hz 
16  Fp2                256 Hz 
17  AF4                256 Hz 
18  Fz                 256 Hz 
19  F4                 256 Hz 
20  F8                 256 Hz 
21  FC6                256 Hz 
22  FC2                256 Hz 
23  Cz                 256 Hz 
24  C4                 256 Hz 
25  T8                 256 Hz 
26  CP6                256 Hz 
27  CP2                256 Hz 
28  P4                 256 Hz 
29  P8                 256 Hz 
30  PO4                256 Hz 
31  O2                 256 Hz 
32  EXG1               256 Hz 
33  EXG2               256 Hz 
34  EXG3               256 Hz 
35  EXG4               256 Hz 
36  EXG5               256 Hz 
37  EXG6               256 Hz 
38  EXG7               256 Hz 
39  EXG8               256 Hz 
40  GSR1               256 Hz 
41  GSR2               256 Hz 
42  Erg1               256 Hz 
43  Erg2               256 Hz 
44  Resp               256 Hz 
45  Temp               256 Hz 
46  Status             256 Hz 
```

Below is an example of how to extract HR information from a file called `my_file.bdf`. The HR information is calculated from signal with id 33 (corresponding to EXG2 in the input file) and extracted into a file named `data.csv`:

```
BDFextractor.exe --input=my_file.bdf --output=data.csv --signal=33 --export-hr-all
```

Show some HR statistics from a file called `my_file.bdf`, using signal with id 33 (corresponding to EXG2 in the input file) for all calculations:

```
BDFextractor.exe --input=my_file.bdf --signal=33 --show-hr-stats
```

The output of the command above is:

```
Input signal: EXG2
 processing: EXG2
 processing: HR

Heart Rate Statistics

Beats:     141
Mean RR:   810 ms
SDNN RR:   69 ms
RMSSD RR:  65 ms
Mean HR:   75.146 bpm
SDNN HR:   14.080 bpm
NN50:      10
pNN50:     7 %
```

## License

This project is licensed under the GPL 3 License - see the [LICENSE](https://github.com/Dovyski/BDFextractor/LICENSE) file for details.

## Disclaimer

This software is an **extremely simplified** command-line version of [EDFbrowser](http://www.teuniz.net/edfbrowser/). EDFbrowser is a free, opensource, multiplatform, universal viewer and toolbox intended for, but not limited to, timeseries storage files like EEG, EMG, ECG, BioImpedance, etc. 

BDFextractor was developed out of the need to bulk extract HR information from BDF files available in the [MAHNOB-HCI](http://mahnob-db.eu/hci-tagging/) database. Users are advised to refer to EDFbrowser for a complete solution regarding BDF/EDF files.

