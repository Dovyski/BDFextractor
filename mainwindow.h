/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016 Teunis van Beelen
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


#ifndef UI_VIEW_MAINFORM_H
#define UI_VIEW_MAINFORM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "global.h"

class UI_Mainwindow
{

public:
  UI_Mainwindow();
  ~UI_Mainwindow();

  int files_open,
      signalcomps,
      totalviewbufsize,
      sel_viewtime,
      viewtime_sync,
      print_to_edf_active,
      annot_editor_active,
      show_annot_markers,
      show_baselines,
      annotations_edited,
      annotations_onset_relative,
      exit_in_progress,
      maxdftblocksize,
      dpix,
      dpiy,
      auto_dpi,
      clip_to_pane,
      auto_reload_mtg,
      read_biosemi_status_signal,
      read_nk_trigger_signal,
      live_stream_active,
      playback_realtime_active,
      live_stream_update_interval,
      signal_averaging_active,
      powerlinefreq,
      mousewheelsens,
      average_ratio,
      average_upsidedown,
      average_bw,
      spectrum_bw,
      spectrum_sqrt,
      spectrum_vlog,
      spectrumdock_sqrt,
      spectrumdock_vlog,
      use_threads,
      check_for_updates,
      amplitude_doubler,
      timescale_doubler,
      viewtime_indicator_type,
      mainwindow_title_type,
      linear_interpol;

  long long pagetime,
            maxfilesize_to_readin_annotations;

  char *viewbuf,
       viewtime_string[128],
       pagetime_string[64],
       recent_montagedir[MAX_PATH_LENGTH],
       recent_savedir[MAX_PATH_LENGTH],
       recent_opendir[MAX_PATH_LENGTH],
       predefined_mtg_path[MAXPREDEFINEDMONTAGES][MAX_PATH_LENGTH],
       recent_file_mtg_path[MAX_RECENTFILES][MAX_PATH_LENGTH],
       recent_colordir[MAX_PATH_LENGTH],
       cfg_app_version[17];

  double pixelsizefactor,
         x_pixelsizefactor,
         average_period,
         default_amplitude;

  struct{
          double crossoverfreq;
          double z_threshold;
          double z_hysteresis;
          int zscore_page_len;
          int zscore_error_detection;
        } z_score_var;

  //struct raw2edf_var_struct raw2edf_var;

  struct edfhdrblock *edfheaderlist[MAXFILES];

  struct signalcompblock *signalcomp[MAXSIGNALS];

  struct zoomhistoryblock *zoomhistory;

  struct annotationblock *annotationlist[MAXFILES];

  struct annotationblock *annotationlist_backup;

  struct spectrum_markersblock *spectrum_colorbar;

  struct import_annotations_var_block *import_annotations_var;

  struct export_annotations_var_block *export_annotations_var;

  struct video_player_struct *video_player;

  void setup_viewbuf();

  //int file_is_opened(const char *);

  //void remove_recent_file_mtg_path(const char *);

  struct signalcompblock * create_signalcomp_copy(struct signalcompblock *);

private:
  char path[MAX_PATH_LENGTH],
       videopath[MAX_PATH_LENGTH],
       montagepath[MAX_PATH_LENGTH],
       recent_file_path[MAX_RECENTFILES][MAX_PATH_LENGTH],
       option_str[MAX_PATH_LENGTH];

  /*
  void read_color_settings();
  void read_recent_file_settings();
  void read_general_settings();
  void write_settings();
  long long check_edf_file_datarecords(struct edfhdrblock *);
  inline void mpr_write(const char *);
  inline int mpr_read(char *, int);

public:
  void remove_all_signals();
  void edfplus_annotation_remove_duplicates();
  void video_player_seek(int);
  void video_player_toggle_pause();
 */

public:
  void open_new_file();
  /*
  void show_file_info();
  //void close_file_action_func(QAction *);
  void close_all_files();
  void exit_program();
  void signalproperties_dialog();
  void filterproperties_dialog();
  void add_signals_dialog();
  void show_about_dialog();
  //void set_display_time(QAction *);
  void set_page_div2();
  void set_page_mult2();
*/
  void set_display_time_whole_rec();
  //void set_amplitude(QAction *);
/*
  void set_amplitude_div2();
  void set_amplitude_mult2();
  void fit_signals_to_pane();
  void fit_signals_dc_offset();
  void former_page();
  void shift_page_left();
  void shift_page_right();
  void playback_realtime();
  void next_page();
  void shift_page_up();
  void shift_page_down();
  void zoomback();
  void forward();
  void show_splashscreen();
  void export_to_ascii();
  void check_edf_compatibility();
  void add_new_filter();
  void add_spike_filter();
  void remove_all_filters();
  void remove_all_spike_filters();
//  void add_new_math_func();
//  void remove_all_math_funcs();
  void jump_to_dialog();
*/
  void jump_to_start();
 // void jump_to_end();
  //void jump_to_time_millisec(long long);
/*  
  void show_annotations();
  void show_options_dialog();
  long long get_long_time(char *);
  void nk2edf_converter();
  //void set_timesync(QAction *);
  //void set_timesync_reference(QAction *);
  //void recent_file_action_func(QAction *);
  void sync_by_crosshairs();
  void save_montage();
  void load_montage();
  void view_montage();
  void show_this_montage();
  void show_help();
  void show_kb_shortcuts();
  void print_to_edf();
  void set_user_defined_display_time();
  void print_to_bdf();
  void print_to_img_640x480();
  void print_to_img_800x600();
  void print_to_img_1024x768();
  void print_to_img_1280x1024();
  void print_to_img_1600x1200();
  void convert_ascii_to_edf();
  void convert_fino_to_edf();
  void convert_wave_to_edf();
  void convert_fm_audio_to_edf();
  void convert_nexfin_to_edf();
  void edfd_converter();
  void slider_moved(int);
  void convert_emsa_to_edf();
  void bdf2edf_converter();
  void set_dc_offset_to_zero();
  void annotation_editor();
  void save_file();
  void unisens2edf_converter();
  void BI98002edf_converter();
  void export_annotations();
  //void load_predefined_mtg(QAction *);
  void edit_predefined_montages();
  void show_spectrum_dock();
  void page_3cmsec();
  void page_25mmsec();
  void page_50mmsec();
  void reduce_signals();
  void edit_header();
  void biosemi2bdfplus_converter();
  void import_annotations();
  void open_stream();
  void start_stop_video();
  void stop_video_generic();
  void live_stream_timer_func();
  void video_poll_timer_func();
  void playback_realtime_timer_func();
  void stop_playback_realtime();
  void organize_signals();
  void Escape_fun();
  void export_ecg_rr_interval_to_ascii();
  void convert_binary_to_edf();
  void convert_manscan_to_edf();
  void convert_scpecg_to_edf();
  void convert_mit_to_edf();
  void convert_biox_to_edf();
  //void video_process_error(QProcess::ProcessError);
//  void search_pattern();
*/
};


#endif



