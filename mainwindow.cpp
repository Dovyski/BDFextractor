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


#include "mainwindow.h"
#include "check_edf_file.h"

UI_Mainwindow::~UI_Mainwindow()
{
	/*
  delete pixmap;
  delete splash;
  delete myfont;
  delete monofont;
  delete maincurve;
  delete annotationEditDock;
  for(int i=0; i<MAXSPECTRUMDOCKS; i++)
  {
    delete spectrumdock[i];
  }
  delete live_stream_timer;
  delete video_poll_timer;
  delete playback_realtime_time;
  delete playback_realtime_timer;
  if(update_checker != NULL)  delete update_checker;
  */
}

/*
void UI_Mainwindow::Escape_fun()
{
  int i;

  for(i=0; i<signalcomps; i++)
  {
    signalcomp[i]->hascursor1 = 0;
    signalcomp[i]->hascursor2 = 0;
    signalcomp[i]->hasoffsettracking = 0;
  }
  maincurve->crosshair_1.active = 0;
  maincurve->crosshair_2.active = 0;
  maincurve->crosshair_1.moving = 0;
  maincurve->crosshair_2.moving = 0;
  maincurve->use_move_events = 0;
  maincurve->setMouseTracking(false);

  for(i=0; i<signalcomps; i++)
  {
    signalcomp[i]->hasruler = 0;
  }
  maincurve->ruler_active = 0;
  maincurve->ruler_moving = 0;

  maincurve->update();
}


long long UI_Mainwindow::check_edf_file_datarecords(struct edfhdrblock *hdr)
{
  long long datarecords;


  if(fseeko(hdr->file_hdl, 0LL, SEEK_END) == -1LL)
  {
    hdr->datarecords = 0LL;

    return(0LL);
  }

  datarecords = ftello(hdr->file_hdl);

  if(datarecords < 1LL)
  {
    hdr->datarecords = 0LL;

    return(0LL);
  }

  datarecords -= (long long)hdr->hdrsize;

  datarecords /= (long long)hdr->recordsize;

  if(datarecords < 1LL)
  {
    hdr->datarecords = 0LL;

    return(0LL);
  }

  hdr->datarecords = datarecords;

  return(datarecords);
}


void UI_Mainwindow::slider_moved(int value)
{
  int i;

  long long new_viewtime,
            tmp;

  if(!signalcomps)  return;

  new_viewtime = edfheaderlist[sel_viewtime]->long_data_record_duration * edfheaderlist[sel_viewtime]->datarecords;

  new_viewtime -= pagetime;

  if(new_viewtime<0)
  {
    new_viewtime = 0;
  }
  else
  {
    new_viewtime /= 1000000;

    new_viewtime *= value;
  }

  if(pagetime >= (300LL * TIME_DIMENSION))
  {
    tmp = new_viewtime % (30LL * TIME_DIMENSION);

    new_viewtime -= tmp;
  }
  else
    if(pagetime >= (60LL * TIME_DIMENSION))
    {
      tmp = new_viewtime % (6LL * TIME_DIMENSION);

      new_viewtime -= tmp;
    }
    else
      if(pagetime >= (30LL * TIME_DIMENSION))
      {
        tmp = new_viewtime % (3LL * TIME_DIMENSION);

        new_viewtime -= tmp;
      }
      else
        if(pagetime >= (20LL * TIME_DIMENSION))
        {
          tmp = new_viewtime % (2LL * TIME_DIMENSION);

          new_viewtime -= tmp;
        }
        else
          if(pagetime >= (10LL * TIME_DIMENSION))
          {
            tmp = new_viewtime % TIME_DIMENSION;

            new_viewtime -= tmp;
          }
          else
            if(pagetime >= TIME_DIMENSION)
            {
              tmp = new_viewtime % (TIME_DIMENSION / 10LL);

              new_viewtime -= tmp;
            }

  if(video_player->status == VIDEO_STATUS_PLAYING)
  {
    video_player_seek((int)(new_viewtime / TIME_DIMENSION));

    return;
  }

  if(video_player->status == VIDEO_STATUS_PAUSED)
  {
    video_player_seek((int)(new_viewtime / TIME_DIMENSION));
  }

  if(viewtime_sync==VIEWTIME_SYNCED_OFFSET)
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime = new_viewtime;
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime = new_viewtime;
  }

  if(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)
  {
    edfheaderlist[sel_viewtime]->viewtime = new_viewtime;

    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime = edfheaderlist[sel_viewtime]->viewtime - ((edfheaderlist[i]->utc_starttime - edfheaderlist[sel_viewtime]->utc_starttime) * TIME_DIMENSION);
      }
    }
  }

  if(viewtime_sync==VIEWTIME_USER_DEF_SYNCED)
  {
    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime -= (edfheaderlist[sel_viewtime]->viewtime - new_viewtime);
      }
    }

    edfheaderlist[sel_viewtime]->viewtime = new_viewtime;
  }

  setup_viewbuf();
}


void UI_Mainwindow::set_timesync_reference(QAction *action)
{
  int i;

  for(i=0; i<files_open; i++)
  {
    if(!strcmp(edfheaderlist[i]->filename, action->text().toLocal8Bit().data()))
    {
      break;
    }
  }

  sel_viewtime = i;

  setMainwindowTitle(edfheaderlist[sel_viewtime]);

  setup_viewbuf();
}


void UI_Mainwindow::set_timesync(QAction *)
{
  int i;


  if(no_timesync_act->isChecked())
  {
    viewtime_sync = VIEWTIME_UNSYNCED;
  }

  if(offset_timesync_act->isChecked())
  {
    viewtime_sync = VIEWTIME_SYNCED_OFFSET;

    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime = edfheaderlist[sel_viewtime]->viewtime;
    }
  }

  if(absolut_timesync_act->isChecked())
  {
    viewtime_sync = VIEWTIME_SYNCED_ABSOLUT;

    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime = edfheaderlist[sel_viewtime]->viewtime + edfheaderlist[sel_viewtime]->starttime_offset - ((edfheaderlist[i]->utc_starttime - edfheaderlist[sel_viewtime]->utc_starttime) * TIME_DIMENSION) - edfheaderlist[i]->starttime_offset;
      }
    }
  }

  if(user_def_sync_act->isChecked())
  {
    viewtime_sync = VIEWTIME_USER_DEF_SYNCED;
  }

  setup_viewbuf();
}


void UI_Mainwindow::sync_by_crosshairs()
{
  if(files_open<2)  return;
  if(signalcomps<2)  return;

  if(maincurve->crosshair_1.active&&maincurve->crosshair_2.active)
  {
    if(maincurve->crosshair_1.file_num!=maincurve->crosshair_2.file_num)
    {
      viewtime_sync = VIEWTIME_USER_DEF_SYNCED;

      edfheaderlist[maincurve->crosshair_2.file_num]->viewtime
      -= ((maincurve->crosshair_1.time - edfheaderlist[maincurve->crosshair_1.file_num]->l_starttime - edfheaderlist[maincurve->crosshair_1.file_num]->viewtime - edfheaderlist[maincurve->crosshair_1.file_num]->starttime_offset)
      - (maincurve->crosshair_2.time - edfheaderlist[maincurve->crosshair_2.file_num]->l_starttime - edfheaderlist[maincurve->crosshair_2.file_num]->viewtime - edfheaderlist[maincurve->crosshair_2.file_num]->starttime_offset));

      maincurve->crosshair_2.x_position = maincurve->crosshair_1.x_position;

      user_def_sync_act->setChecked(true);

      viewtime_sync = VIEWTIME_USER_DEF_SYNCED;

      setup_viewbuf();
    }
  }
}


void UI_Mainwindow::show_options_dialog()
{
  UI_OptionsDialog OptionsDialog(this);
}


void UI_Mainwindow::nk2edf_converter()
{
  UI_NK2EDFwindow nk2edf(recent_opendir);
}


void UI_Mainwindow::convert_ascii_to_edf()
{
  UI_ASCII2EDFapp ascii2edf(this, recent_opendir, recent_savedir);
}


void UI_Mainwindow::convert_binary_to_edf()
{
  UI_RAW2EDFapp bin2edf(this, &raw2edf_var, recent_opendir, recent_savedir);
}


void UI_Mainwindow::convert_fino_to_edf()
{
  UI_FINO2EDFwindow fino2edf(recent_opendir, recent_savedir);
}


void UI_Mainwindow::convert_wave_to_edf()
{
  UI_WAV2EDFwindow wav2edf(recent_opendir, recent_savedir);
}


void UI_Mainwindow::convert_fm_audio_to_edf()
{
  UI_FMaudio2EDFwindow fma2edf(recent_opendir, recent_savedir);
}


void UI_Mainwindow::convert_nexfin_to_edf()
{
  UI_NEXFIN2EDFwindow nexfin2edf(recent_opendir, recent_savedir);
}


void UI_Mainwindow::convert_emsa_to_edf()
{
  UI_EMSA2EDFwindow emsa2edf(recent_opendir, recent_savedir);
}


void UI_Mainwindow::convert_manscan_to_edf()
{
  UI_MANSCAN2EDFwindow manscan2edf(recent_opendir, recent_savedir);
}


void UI_Mainwindow::convert_mit_to_edf()
{
  UI_MIT2EDFwindow mit2edf(recent_opendir, recent_savedir);
}


void UI_Mainwindow::convert_biox_to_edf()
{
  UI_BIOX2EDFwindow biox2edf(recent_opendir, recent_savedir);
}


void UI_Mainwindow::convert_scpecg_to_edf()
{
  UI_SCPECG2EDFwindow scpecg2edf(recent_opendir, recent_savedir);
}


void UI_Mainwindow::edfd_converter()
{
  UI_EDFDwindow edfplusd2edfplusc(recent_opendir, recent_savedir);
}


void UI_Mainwindow::bdf2edf_converter()
{
  UI_BDF2EDFwindow bdf2edfconv(this);
}


void UI_Mainwindow::biosemi2bdfplus_converter()
{
  UI_BIOSEMI2BDFPLUSwindow biosemi2bdfplusconv(this);
}


void UI_Mainwindow::reduce_signals()
{
  if(!files_open)
  {
    printf("You have to open the file first.");
    exit(2);
    return;
  }

  UI_ReduceSignalsWindow reduceSignals(this);
}


void UI_Mainwindow::edit_header()
{
  UI_headerEditorWindow header_edit(this);
}


void UI_Mainwindow::unisens2edf_converter()
{
  UI_UNISENS2EDFwindow unisens2edfconv(recent_opendir, recent_savedir);
}


void UI_Mainwindow::BI98002edf_converter()
{
  UI_BI98002EDFwindow BI98002edfconv(recent_opendir, recent_savedir);
}


void UI_Mainwindow::edit_predefined_montages()
{
  UI_edit_predefined_mtg_window edit_predef_mtgs_app(this);
}


void UI_Mainwindow::jump_to_dialog()
{
  if(!files_open)  return;

  UI_JumpMenuDialog jumpmenu(this);
}

*/
void UI_Mainwindow::jump_to_start()
{
  int i;

  if(!files_open)  return;

  if(viewtime_sync==VIEWTIME_SYNCED_OFFSET)
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime = 0;
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime = 0;
  }

  if(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)
  {
    edfheaderlist[sel_viewtime]->viewtime = 0;

    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime = ((edfheaderlist[sel_viewtime]->utc_starttime - edfheaderlist[i]->utc_starttime) * TIME_DIMENSION) + edfheaderlist[sel_viewtime]->starttime_offset - edfheaderlist[i]->starttime_offset;
      }
    }
  }

  if(viewtime_sync==VIEWTIME_USER_DEF_SYNCED)
  {
    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime -= edfheaderlist[sel_viewtime]->viewtime;
      }
    }

    edfheaderlist[sel_viewtime]->viewtime = 0;
  }

  setup_viewbuf();
}

/*

void UI_Mainwindow::jump_to_time_millisec(long long milliseconds)
{
  int i;

  if(!files_open)  return;

  if(viewtime_sync==VIEWTIME_SYNCED_OFFSET)
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime = milliseconds * (TIME_DIMENSION / 1000);
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime = milliseconds * (TIME_DIMENSION / 1000);
  }

  if(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)
  {
    edfheaderlist[sel_viewtime]->viewtime = milliseconds * (TIME_DIMENSION / 1000);

    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime = edfheaderlist[sel_viewtime]->viewtime - ((edfheaderlist[i]->utc_starttime - edfheaderlist[sel_viewtime]->utc_starttime) * TIME_DIMENSION);
      }
    }
  }

  if(viewtime_sync==VIEWTIME_USER_DEF_SYNCED)
  {
    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime -= (edfheaderlist[sel_viewtime]->viewtime - milliseconds * (TIME_DIMENSION / 1000));
      }
    }

    edfheaderlist[sel_viewtime]->viewtime = milliseconds * (TIME_DIMENSION / 1000);
  }

  setup_viewbuf();
}


void UI_Mainwindow::jump_to_end()
{
  int i;

  long long new_viewtime;


  if(!files_open)  return;

  new_viewtime = edfheaderlist[sel_viewtime]->datarecords * edfheaderlist[sel_viewtime]->long_data_record_duration - pagetime;

  if(viewtime_sync==VIEWTIME_SYNCED_OFFSET)
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime = new_viewtime;
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime = new_viewtime;
  }

  if(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)
  {
    edfheaderlist[sel_viewtime]->viewtime = new_viewtime;

    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime = edfheaderlist[sel_viewtime]->viewtime + edfheaderlist[sel_viewtime]->starttime_offset - ((edfheaderlist[i]->utc_starttime - edfheaderlist[sel_viewtime]->utc_starttime) * TIME_DIMENSION) - edfheaderlist[i]->starttime_offset;
      }
    }
  }

  if(viewtime_sync==VIEWTIME_USER_DEF_SYNCED)
  {
    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime -= (edfheaderlist[sel_viewtime]->viewtime - new_viewtime);
      }
    }

    edfheaderlist[sel_viewtime]->viewtime = new_viewtime;
  }

  setup_viewbuf();
}


void UI_Mainwindow::add_new_filter()
{
  if(!files_open)  return;

  UI_FilterDialog filterdialog(this);
}


void UI_Mainwindow::add_spike_filter()
{
  if(!files_open)  return;

  UI_SpikeFilterDialog spikefilterdialog(this);
}


// void UI_Mainwindow::add_new_math_func()
// {
// }


// void UI_Mainwindow::remove_all_math_funcs()
// {
// }


void UI_Mainwindow::zoomback()
{
  int i, j;

  if(!files_open)  return;

  if(!zoomhistory->history_size_tail)
  {
    return;
  }
  zoomhistory->history_size_front++;
  zoomhistory->history_size_tail--;

  for(i=0; i<files_open; i++)
  {
    zoomhistory->viewtime[zoomhistory->pntr][i] = edfheaderlist[i]->viewtime;
  }
  zoomhistory->pagetime[zoomhistory->pntr] = pagetime;
  for(i=0; i<signalcomps; i++)
  {
    zoomhistory->voltpercm[zoomhistory->pntr][i] = signalcomp[i]->voltpercm;
    zoomhistory->screen_offset[zoomhistory->pntr][i] = signalcomp[i]->screen_offset;

    for(j=0; j<signalcomp[i]->num_of_signals; j++)
    {
      zoomhistory->sensitivity[zoomhistory->pntr][i][j] = signalcomp[i]->sensitivity[j];
    }
  }

  zoomhistory->pntr--;
  zoomhistory->pntr += MAXZOOMHISTORY;
  zoomhistory->pntr %= MAXZOOMHISTORY;

  for(i=0; i<files_open; i++)
  {
    edfheaderlist[i]->viewtime = zoomhistory->viewtime[zoomhistory->pntr][i];
  }
  pagetime = zoomhistory->pagetime[zoomhistory->pntr];

  for(i=0; i<signalcomps; i++)
  {
    signalcomp[i]->voltpercm = zoomhistory->voltpercm[zoomhistory->pntr][i];
    signalcomp[i]->screen_offset = zoomhistory->screen_offset[zoomhistory->pntr][i];

    for(j=0; j<signalcomp[i]->num_of_signals; j++)
    {
      signalcomp[i]->sensitivity[j] = zoomhistory->sensitivity[zoomhistory->pntr][i][j];
    }
  }

  setup_viewbuf();
}


void UI_Mainwindow::forward()
{
  int i, j;

  if(!files_open)  return;

  if(!zoomhistory->history_size_front)
  {
    return;
  }
  zoomhistory->history_size_front--;
  zoomhistory->history_size_tail++;

  zoomhistory->pntr++;
  zoomhistory->pntr %= MAXZOOMHISTORY;

  for(i=0; i<files_open; i++)
  {
    edfheaderlist[i]->viewtime = zoomhistory->viewtime[zoomhistory->pntr][i];
  }
  pagetime = zoomhistory->pagetime[zoomhistory->pntr];

  for(i=0; i<signalcomps; i++)
  {
    signalcomp[i]->voltpercm = zoomhistory->voltpercm[zoomhistory->pntr][i];
    signalcomp[i]->screen_offset = zoomhistory->screen_offset[zoomhistory->pntr][i];

    for(j=0; j<signalcomp[i]->num_of_signals; j++)
    {
      signalcomp[i]->sensitivity[j] = zoomhistory->sensitivity[zoomhistory->pntr][i][j];
    }
  }

  setup_viewbuf();
}


void UI_Mainwindow::former_page()
{
  int i;

  if(!files_open)  return;

  if(video_player->status == VIDEO_STATUS_PLAYING)
  {
    video_player_seek((int)((edfheaderlist[sel_viewtime]->viewtime - pagetime) / TIME_DIMENSION));

    return;
  }

  if(video_player->status == VIDEO_STATUS_PAUSED)
  {
    video_player_seek((int)((edfheaderlist[sel_viewtime]->viewtime - pagetime) / TIME_DIMENSION));
  }

  if((viewtime_sync==VIEWTIME_SYNCED_OFFSET)||(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)||(viewtime_sync==VIEWTIME_USER_DEF_SYNCED))
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime -= pagetime;
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime -= pagetime;
  }

  setup_viewbuf();
}


void UI_Mainwindow::shift_page_left()
{
  int i;

  if(!files_open)  return;

  if(video_player->status == VIDEO_STATUS_PLAYING)
  {
    video_player_seek((int)((edfheaderlist[sel_viewtime]->viewtime - (pagetime / 10)) / TIME_DIMENSION));

    return;
  }

  if(video_player->status == VIDEO_STATUS_PAUSED)
  {
    video_player_seek((int)((edfheaderlist[sel_viewtime]->viewtime - (pagetime / 10)) / TIME_DIMENSION));
  }

  if((viewtime_sync==VIEWTIME_SYNCED_OFFSET)||(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)||(viewtime_sync==VIEWTIME_USER_DEF_SYNCED))
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime -= (pagetime / 10);
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime -= (pagetime / 10);
  }

  setup_viewbuf();
}


void UI_Mainwindow::shift_page_right()
{
  int i;

  if(!files_open)  return;

  if(video_player->status == VIDEO_STATUS_PLAYING)
  {
    video_player_seek((int)((edfheaderlist[sel_viewtime]->viewtime + (pagetime / 10)) / TIME_DIMENSION));

    return;
  }

  if(video_player->status == VIDEO_STATUS_PAUSED)
  {
    video_player_seek((int)((edfheaderlist[sel_viewtime]->viewtime + (pagetime / 10)) / TIME_DIMENSION));
  }

  if((viewtime_sync==VIEWTIME_SYNCED_OFFSET)||(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)||(viewtime_sync==VIEWTIME_USER_DEF_SYNCED))
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime += (pagetime / 10);
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime += (pagetime / 10);
  }

  setup_viewbuf();
}


void UI_Mainwindow::playback_realtime()
{
  if(!signalcomps)
  {
    return;
  }

  if(live_stream_active)
  {
    return;
  }

  if(video_player->status == VIDEO_STATUS_PLAYING)
  {
    return;
  }

  if(playback_realtime_active)
  {
    stop_playback_realtime();
  }
  else
  {
    playback_realtime_time->start();

    playback_realtime_timer->start();

    playback_realtime_Act->setText("[stop]");

    playback_realtime_active = 1;
  }
}


void UI_Mainwindow::stop_playback_realtime()
{
  playback_realtime_timer->stop();

  playback_realtime_Act->setText("[play]");

  playback_realtime_active = 0;
}


void UI_Mainwindow::playback_realtime_timer_func()
{
  if((viewtime_sync==VIEWTIME_SYNCED_OFFSET)||(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)||(viewtime_sync==VIEWTIME_USER_DEF_SYNCED))
  {
    for(int i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime += (playback_realtime_time->restart() * 10000);
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime += (playback_realtime_time->restart() * 10000);
  }

  setup_viewbuf();
}


void UI_Mainwindow::next_page()
{
  int i;

  if(!files_open)  return;

  if(video_player->status == VIDEO_STATUS_PLAYING)
  {
    video_player_seek((int)((edfheaderlist[sel_viewtime]->viewtime + pagetime) / TIME_DIMENSION));

    return;
  }

  if(video_player->status == VIDEO_STATUS_PAUSED)
  {
    video_player_seek((int)((edfheaderlist[sel_viewtime]->viewtime + pagetime) / TIME_DIMENSION));
  }

  if((viewtime_sync==VIEWTIME_SYNCED_OFFSET)||(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)||(viewtime_sync==VIEWTIME_USER_DEF_SYNCED))
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime += pagetime;
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime += pagetime;
  }

  setup_viewbuf();
}


void UI_Mainwindow::shift_page_up()
{
  int i;

  if(!files_open)  return;

  for(i=0; i<signalcomps; i++)
  {
    signalcomp[i]->screen_offset += (height() / 20.0);

  }

  maincurve->drawCurve_stage_1();
}


void UI_Mainwindow::shift_page_down()
{
  int i;

  if(!files_open)  return;

  for(i=0; i<signalcomps; i++)
  {
    signalcomp[i]->screen_offset -= (height() / 20.0);

  }

  maincurve->drawCurve_stage_1();
}


void UI_Mainwindow::show_annotations()
{
  int i;

  EDF_annotations annotations;

  if(!files_open)  return;

  for(i=0; i<files_open; i++)
  {
    if(edfheaderlist[i]->annots_not_read)
    {
      if((edfheaderlist[i]->edfplus) || (edfheaderlist[i]->bdfplus))
      {
        edfheaderlist[i]->annots_not_read = 0;

        annotations.get_annotations(i, edfheaderlist[i], &annotationlist[i], read_nk_trigger_signal);

        if(edfheaderlist[i]->annots_not_read)
        {
          edfplus_annotation_delete_list(&annotationlist[i]);
        }
        else
        {
          if(annotations_dock[i] == NULL)
          {
            annotations_dock[i] = new UI_Annotationswindow(i, this);

            addDockWidget(Qt::RightDockWidgetArea, annotations_dock[i]->docklist, Qt::Vertical);
          }
        }
      }
    }

    if(annotationlist[i] != NULL)
    {
      if(annotations_dock[i] != NULL)
      {
        annotations_dock[i]->docklist->show();
      }
    }
  }
}


void UI_Mainwindow::annotation_editor()
{
  stop_video_generic();

  stop_playback_realtime();

  if(!files_open)  return;

  if(files_open==1)
  {
    if(edfheaderlist[0]->annots_not_read)
    {
      edfplus_annotation_delete_list(&annotationlist[0]);

      if(annotations_dock[0] != NULL)
      {
        annotations_dock[0]->docklist->close();
        delete annotations_dock[0];
        annotations_dock[0] = NULL;
      }

      edfheaderlist[0]->annots_not_read = 0;

      EDF_annotations annotations;

      annotations.get_annotations(0, edfheaderlist[0], &annotationlist[0], read_nk_trigger_signal);

      if(edfheaderlist[0]->annots_not_read)
      {
        printf("Editing annotations is not possible when you abort the scanning of the file.");
        exit(2);

        return;
      }
    }

    if(annotations_dock[0]==NULL)
    {
      annotations_dock[0] = new UI_Annotationswindow(0, this);

      addDockWidget(Qt::RightDockWidgetArea, annotations_dock[0]->docklist, Qt::Vertical);
    }

    annotations_dock[0]->docklist->show();

    annotationEditDock->dockedit->show();
  }
  else
  {
    printf("Editing annotations is possible when you have opened one file only.");
    exit(2);
  }
}


void UI_Mainwindow::show_spectrum_dock()
{
  if(!files_open)  return;

  UI_SignalChooser signalchooserdialog(this, 1);
}

*/

void UI_Mainwindow::open_new_file(const char *path)
{
  FILE *newfile;

  int i, len, present, position, button_nr=0;

  char str[2048];

  str[0] = 0;

  struct edfhdrblock *edfhdr=NULL;
  /*
  if(annot_editor_active && files_open)
  {
    printf("You can not open multiple files when editing annotations. Close the annotation edit window first.");
    exit(2);
  }
  
  if((files_open > 0) && (live_stream_active))
  {
    printf("You can not open multiple files while a streaming file is open. Close the streaming file first.");
    exit(2);

    return;
  }
  */
  if(files_open >= MAXFILES)
  {
    printf("There are too many files opened.");
    exit(2);
  }

  present = 0;

  if(!present)
  {
    len = strlen(path);

    if((strcmp(path + (len - 4), ".edf"))
       &&(strcmp(path + (len - 4), ".EDF"))
       &&(strcmp(path + (len - 4), ".rec"))
       &&(strcmp(path + (len - 4), ".REC"))
       &&(strcmp(path + (len - 4), ".bdf"))
       &&(strcmp(path + (len - 4), ".BDF")))
    {
      printf("File has an unknown extension:  \"%s\"", path + (len - 4));
      exit(2);
    }

    newfile = fopeno(path, "rb");
    if(newfile==NULL)
    {
      printf("Can not open file for reading:\n\"%s\" .Check if you have the right permissions.", path);
      exit(2);
    }

    //strcpy(recent_opendir, path);

    if(len)
    {
      for(i=len-1; i>=0; i--)
      {
        if((path[i] == '/')||(path[i] == '\\'))
        {
          break;
        }
      }
    }

    //recent_opendir[i] = 0;

    EDFfileCheck EDFfilechecker;
    edfhdr = EDFfilechecker.check_edf_file(newfile, str);

    if(edfhdr==NULL)
    {
      fclose(newfile);
      printf("Input is not a valid EDF or BDF file.");
      exit(2);
    }

    if(edfhdr->discontinuous)
    {
      if(edfhdr->edf)
      {
        printf("EDFbrowser can not show EDF+D (discontiguous) files.\n"
                                                                  "Convert this file to EDF+C first. You can find this converter\n"
                                                                  "in the Tools menu (EDF+D to EDF+C converter).");
      }

      if(edfhdr->bdf)
      {
        printf("EDFbrowser can not show BDF+D (discontiguous) files.\n"
                                                                  "Convert this file to BDF+C first. You can find this converter\n"
                                                                  "in the Tools menu (EDF+D to EDF+C converter).");
      }

      free(edfhdr->edfparam);
      free(edfhdr);
      fclose(newfile);

	  exit(2);
    }

    strcpy(edfhdr->filename, path);

    edfhdr->file_hdl = newfile;

    edfhdr->file_num = files_open;

    edfheaderlist[files_open] = edfhdr;

    annotationlist[files_open] = NULL;
	/*
    annotations_dock[files_open] = NULL;

    if((edfhdr->edfplus || edfhdr->bdfplus) && (!live_stream_active))
    {
      if((edfhdr->datarecords * (long long)edfhdr->recordsize) <= maxfilesize_to_readin_annotations)
      {
        EDF_annotations annotations;

        annotations.get_annotations(files_open, edfhdr, &annotationlist[files_open], read_nk_trigger_signal);

        if(edfhdr->annots_not_read)
        {
          edfplus_annotation_delete_list(&annotationlist[files_open]);
        }
        else
        {
          annotations_dock[files_open] = new UI_Annotationswindow(files_open, this);

          addDockWidget(Qt::RightDockWidgetArea, annotations_dock[files_open]->docklist, Qt::Vertical);

          if(!annotationlist[files_open])
          {
            annotations_dock[files_open]->docklist->hide();
          }
        }
      }
      else
      {
        edfhdr->annots_not_read = 1;
      }
    }

    if((edfhdr->bdf && (!edfhdr->bdfplus) && read_biosemi_status_signal) && (!live_stream_active))
    {
      if((edfhdr->datarecords * (long long)edfhdr->recordsize) <= maxfilesize_to_readin_annotations)
      {
        BDF_triggers bdf_triggers_obj;

        bdf_triggers_obj.get_triggers(edfhdr, &annotationlist[files_open]);

        annotations_dock[files_open] = new UI_Annotationswindow(files_open, this);

        addDockWidget(Qt::RightDockWidgetArea, annotations_dock[files_open]->docklist, Qt::Vertical);

        if(!annotationlist[files_open])
        {
          annotations_dock[files_open]->docklist->hide();
        }
      }
    }

	*/

    if(!files_open)
    {
      edfheaderlist[0]->viewtime = 0;

      //setMainwindowTitle(edfhdr);
    }
    else
    {
      if(viewtime_sync==VIEWTIME_SYNCED_OFFSET)
      {
        edfheaderlist[files_open]->viewtime = edfheaderlist[sel_viewtime]->viewtime;
      }

      if(viewtime_sync==VIEWTIME_UNSYNCED)
      {
        edfheaderlist[files_open]->viewtime = 0;
      }

      if(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)
      {
        edfheaderlist[files_open]->viewtime = edfheaderlist[sel_viewtime]->viewtime + edfheaderlist[sel_viewtime]->starttime_offset - edfheaderlist[files_open]->starttime_offset - ((edfheaderlist[files_open]->utc_starttime - edfheaderlist[sel_viewtime]->utc_starttime) * TIME_DIMENSION);
      }

      if(viewtime_sync==VIEWTIME_USER_DEF_SYNCED)
      {
        edfheaderlist[files_open]->viewtime = 0;
      }
    }

	/*
	sel_viewtime_act[files_open] = new QAction(QString::fromLocal8Bit(edfhdr->filename), this);
    sel_viewtime_act[files_open]->setCheckable(true);
    if(!files_open)
    {
      sel_viewtime_act[files_open]->setChecked(true);
    }
    sel_viewtime_act_group->addAction(sel_viewtime_act[files_open]);
    timemenu->addAction(sel_viewtime_act[files_open]);
	*/
    files_open++;
  }

  /*
  if((montagepath[0]!=0)&&(cmdlineargument==2))
  {
    UI_LoadMontagewindow load_mtg(this, montagepath);
    strcpy(&recent_file_mtg_path[0][0], montagepath);
  }
  else
  {
    if((recent_file_mtg_path[0][0] != 0) && (files_open == 1) && auto_reload_mtg)
    {
      printf;
      messagewindow.setText("Load last used montage?");
      messagewindow.setInformativeText(QString::fromLocal8Bit(&recent_file_mtg_path[0][0]));
      messagewindow.setIcon(QMessageBox::Question);
      messagewindow.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      messagewindow.setDefaultButton(QMessageBox::Yes);
      button_nr = exit(2);

      if(button_nr == QMessageBox::No)
      {
        recent_file_mtg_path[0][0] = 0;

        UI_Signalswindow signalwindow(this);
      }
      else
      {
        UI_LoadMontagewindow load_mtg(this, &recent_file_mtg_path[0][0]);

        if(recent_file_mtg_path[0][0] == 0)
        {
          UI_Signalswindow signalwindow(this);
        }
      }
    }
    else
    {
      UI_Signalswindow signalwindow(this);
    }
  }

  cmdlineargument = 0;

  close_filemenu->addAction(QString::fromLocal8Bit(path));
  */
}


/*
void UI_Mainwindow::remove_recent_file_mtg_path(const char *mtg_path)
{
  int i;


  for(i=0; i<MAX_RECENTFILES; i++)
  {
    if(!strcmp(&recent_file_mtg_path[i][0], mtg_path))
    {
      recent_file_mtg_path[i][0] = 0;
    }
  }
}


void UI_Mainwindow::setMainwindowTitle(struct edfhdrblock *edfhdr)
{
  int i, len;

  char str[MAX_PATH_LENGTH + 64];

  struct date_time_struct date_time;


  if(edfhdr==NULL)
  {
    setWindowTitle(PROGRAM_NAME);

    return;
  }

  str[0] = 0;

  if(mainwindow_title_type == 0)
  {
    if(edfhdr->edfplus || edfhdr->bdfplus)
    {
      snprintf(str, 256, PROGRAM_NAME "  subject %s  birthdate %s  startdate %s",
                    edfhdr->plus_patient_name,
                    edfhdr->plus_birthdate,
                    edfhdr->plus_startdate);
    }
    else
    {
      utc_to_date_time(edfhdr->utc_starttime, &date_time);

      date_time.month_str[0] += 32;
      date_time.month_str[1] += 32;
      date_time.month_str[2] += 32;

      snprintf(str, 256, PROGRAM_NAME "  %s  startdate %i %s %i",
                    edfhdr->patient,
                    date_time.day,
                    date_time.month_str,
                    date_time.year);
      len = strlen(str);
      for(i=0; i<len; i++)
      {
        if(str[i]=='_')
        {
          str[i] = ' ';
        }
      }
    }
  }

  if(mainwindow_title_type == 1)
  {
    get_filename_from_path(str, edfhdr->filename, MAX_PATH_LENGTH);

    strcat(str, " - " PROGRAM_NAME);
  }

  if(mainwindow_title_type == 2)
  {
    strcpy(str, edfhdr->filename);

    strcat(str, " - " PROGRAM_NAME);
  }

  setWindowTitle(str);
}


void UI_Mainwindow::signalproperties_dialog()
{
  if(!files_open)  return;

  UI_SignalChooser signalchooserdialog(this, 0);
}


void UI_Mainwindow::filterproperties_dialog()
{
  if(!files_open)  return;

  UI_SignalChooser signalchooserdialog(this, 2);
}


void UI_Mainwindow::add_signals_dialog()
{
  if(!files_open)  return;

  UI_Signalswindow signalwindow(this);
}


void UI_Mainwindow::show_splashscreen()
{
  splash = new QSplashScreen(this, *pixmap, Qt::WindowStaysOnTopHint);

  splash->show();
}


void UI_Mainwindow::show_file_info()
{
  if(!files_open)  return;

  UI_EDFhdrwindow showhdr(this);
}


void UI_Mainwindow::remove_all_filters()
{
  int i, j,
      update_scr=0;

  if(!files_open)  return;

  for(i=0; i<signalcomps; i++)
  {
    for(j=0; j<signalcomp[i]->filter_cnt; j++)
    {
      free(signalcomp[i]->filter[j]);

      update_scr = 1;
    }

    signalcomp[i]->filter_cnt = 0;

    for(j=0; j<signalcomp[i]->fidfilter_cnt; j++)
    {
      free(signalcomp[i]->fidfilter[j]);
      fid_run_free(signalcomp[i]->fid_run[j]);
      fid_run_freebuf(signalcomp[i]->fidbuf[j]);
      fid_run_freebuf(signalcomp[i]->fidbuf2[j]);

      update_scr = 1;
    }

    signalcomp[i]->fidfilter_cnt = 0;

    for(j=0; j<signalcomp[i]->ravg_filter_cnt; j++)
    {
      free_ravg_filter(signalcomp[i]->ravg_filter[j]);

      update_scr = 1;
    }

    signalcomp[i]->ravg_filter_cnt = 0;

    if(signalcomp[i]->ecg_filter != NULL)
    {
      free_ecg_filter(signalcomp[i]->ecg_filter);

      signalcomp[i]->ecg_filter = NULL;

      strcpy(signalcomp[i]->signallabel, signalcomp[i]->signallabel_bu);
      signalcomp[i]->signallabellen = signalcomp[i]->signallabellen_bu;
      strcpy(signalcomp[i]->physdimension, signalcomp[i]->physdimension_bu);
    }

    if(signalcomp[i]->zratio_filter != NULL)
    {
      free_zratio_filter(signalcomp[i]->zratio_filter);

      signalcomp[i]->zratio_filter = NULL;

      strcpy(signalcomp[i]->signallabel, signalcomp[i]->signallabel_bu);
      signalcomp[i]->signallabellen = signalcomp[i]->signallabellen_bu;
      strcpy(signalcomp[i]->physdimension, signalcomp[i]->physdimension_bu);
    }
  }

  if(update_scr)
  {
    setup_viewbuf();
  }
}


void UI_Mainwindow::remove_all_spike_filters()
{
  int i,
      update_scr=0;

  if(!files_open)  return;

  for(i=0; i<signalcomps; i++)
  {
    if(signalcomp[i]->spike_filter)
    {
      free_spike_filter(signalcomp[i]->spike_filter);

      signalcomp[i]->spike_filter = NULL;

      update_scr = 1;
    }
  }

  if(update_scr)
  {
    setup_viewbuf();
  }
}


void UI_Mainwindow::remove_all_signals()
{
  int i;

  stop_video_generic();

  stop_playback_realtime();

  for(i=0; i<MAXSPECTRUMDOCKS; i++)
  {
    spectrumdock[i]->clear();
    spectrumdock[i]->dock->hide();
  }

  for(i=0; i<MAXSPECTRUMDIALOGS; i++)
  {
    if(spectrumdialog[i] != NULL)
    {
      delete spectrumdialog[i];

      spectrumdialog[i] = NULL;
    }
  }

  for(i=0; i<MAXAVERAGECURVEDIALOGS; i++)
  {
    if(averagecurvedialog[i] != NULL)
    {
      delete averagecurvedialog[i];

      averagecurvedialog[i] = NULL;
    }
  }

  for(i=0; i<MAXZSCOREDIALOGS; i++)
  {
    if(zscoredialog[i] != NULL)
    {
      delete zscoredialog[i];

      zscoredialog[i] = NULL;
    }
  }

  maincurve->crosshair_1.active = 0;
  maincurve->crosshair_2.active = 0;
  maincurve->crosshair_1.moving = 0;
  maincurve->crosshair_2.moving = 0;

  remove_all_filters();

  remove_all_spike_filters();

  for(i=0; i<signalcomps; i++)
  {
    free(signalcomp[i]);
  }

  signalcomps = 0;

  if(viewbuf!=NULL)
  {
    free(viewbuf);
    viewbuf = NULL;
  }

  slidertoolbar->setEnabled(false);
  positionslider->blockSignals(true);

  setup_viewbuf();
}


void UI_Mainwindow::close_file_action_func(QAction *action)
{
  int i, j, k, p,
      file_n;

  char f_path[MAX_PATH_LENGTH];

  if(files_open < 2)
  {
    close_all_files();

    return;
  }

  strcpy(f_path, action->text().toLocal8Bit().data());

  for(file_n=0; file_n<files_open; file_n++)
  {
    if(!strcmp(f_path, edfheaderlist[file_n]->filename))
    {
      break;
    }
  }

  if(file_n == files_open)  return;

  delete action;

  stop_video_generic();

  stop_playback_realtime();

  for(j=0; j<signalcomps; )
  {
    if(signalcomp[j]->filenum == file_n)
    {
      for(i=0; i<MAXSPECTRUMDOCKS; i++)
      {
        if(spectrumdock[i]->signalcomp == signalcomp[j])
        {
          spectrumdock[i]->clear();
          spectrumdock[i]->dock->hide();
        }
      }

      for(i=0; i<MAXSPECTRUMDIALOGS; i++)
      {
        p = signalcomp[j]->spectr_dialog[i];

        if(p != 0)
        {
          delete spectrumdialog[p - 1];

          spectrumdialog[p - 1] = NULL;
        }
      }

      for(i=0; i<MAXAVERAGECURVEDIALOGS; i++)
      {
        p = signalcomp[j]->avg_dialog[i];

        if(p != 0)
        {
          delete averagecurvedialog[p - 1];

          averagecurvedialog[p - 1] = NULL;
        }
      }

      for(i=0; i<MAXZSCOREDIALOGS; i++)
      {
        p = signalcomp[j]->zscoredialog[i];

        if(p != 0)
        {
          delete zscoredialog[p - 1];

          zscoredialog[p - 1] = NULL;
        }
      }

      if(signalcomp[j]->hascursor2)
      {
        maincurve->crosshair_2.active = 0;
        maincurve->crosshair_2.moving = 0;
      }

      if(signalcomp[j]->hascursor1)
      {
        maincurve->crosshair_1.active = 0;
        maincurve->crosshair_2.active = 0;
        maincurve->crosshair_1.moving = 0;
        maincurve->crosshair_2.moving = 0;

        for(i=0; i<signalcomps; i++)
        {
          signalcomp[i]->hascursor2 = 0;
        }
      }

      if(signalcomp[j]->hasruler)
      {
        maincurve->ruler_active = 0;
        maincurve->ruler_moving = 0;
      }

      for(k=0; k<signalcomp[j]->filter_cnt; k++)
      {
        free(signalcomp[j]->filter[k]);
      }

      signalcomp[j]->filter_cnt = 0;

      if(signalcomp[j]->spike_filter)
      {
        free_spike_filter(signalcomp[j]->spike_filter);

        signalcomp[j]->spike_filter = NULL;
      }

      for(k=0; k<signalcomp[j]->ravg_filter_cnt; k++)
      {
        free_ravg_filter(signalcomp[j]->ravg_filter[k]);
      }

      signalcomp[j]->ravg_filter_cnt = 0;

      if(signalcomp[j]->ecg_filter != NULL)
      {
        free_ecg_filter(signalcomp[j]->ecg_filter);

        signalcomp[j]->ecg_filter = NULL;

        strcpy(signalcomp[j]->signallabel, signalcomp[j]->signallabel_bu);
        signalcomp[j]->signallabellen = signalcomp[j]->signallabellen_bu;
        strcpy(signalcomp[j]->physdimension, signalcomp[j]->physdimension_bu);
      }

      for(k=0; k<signalcomp[j]->fidfilter_cnt; k++)
      {
        free(signalcomp[j]->fidfilter[k]);
        fid_run_free(signalcomp[j]->fid_run[k]);
        fid_run_freebuf(signalcomp[j]->fidbuf[k]);
        fid_run_freebuf(signalcomp[j]->fidbuf2[k]);
      }

      signalcomp[j]->fidfilter_cnt = 0;

      free(signalcomp[j]);

      for(i=j; i<signalcomps - 1; i++)
      {
        signalcomp[i] = signalcomp[i + 1];
      }

      signalcomps--;
    }
    else
    {
      j++;
    }
  }

  for(j=0; j<signalcomps; j++)
  {
    if(signalcomp[j]->filenum > file_n)
    {
      signalcomp[j]->filenum--;
    }
  }

  fclose(edfheaderlist[file_n]->file_hdl);
  free(edfheaderlist[file_n]->edfparam);
  free(edfheaderlist[file_n]);
  edfplus_annotation_delete_list(&annotationlist[file_n]);

  if(annotations_dock[file_n] != NULL)
  {
    annotations_dock[file_n]->docklist->close();
    delete annotations_dock[file_n];
    annotations_dock[file_n] = NULL;
  }

  if((file_n == sel_viewtime) && (files_open > 1))
  {
    if(file_n > 0)
    {
      sel_viewtime = 0;
    }
    else
    {
      sel_viewtime = 1;
    }

    sel_viewtime_act[sel_viewtime]->setChecked(true);

    setMainwindowTitle(edfheaderlist[sel_viewtime]);
  }

  delete sel_viewtime_act[file_n];

  for(i=file_n; i<files_open - 1; i++)
  {
    edfheaderlist[i] = edfheaderlist[i + 1];

    annotationlist[i] = annotationlist[i + 1];

    annotations_dock[i] = annotations_dock[i + 1];

    sel_viewtime_act[i] = sel_viewtime_act[i + 1];
  }

  files_open--;

  setup_viewbuf();
}


void UI_Mainwindow::close_all_files()
{
  int i, j, k,
      button_nr=0;


  live_stream_active = 0;
  live_stream_timer->stop();
  toolsmenu->setEnabled(true);
  timemenu->setEnabled(true);
//  windowmenu->setEnabled(true);
  windowmenu->actions().at(0)->setEnabled(true);  // Annotations window
  windowmenu->actions().at(1)->setEnabled(true);  // Annotation editor
  former_page_Act->setEnabled(true);
  shift_page_left_Act->setEnabled(true);
  shift_page_right_Act->setEnabled(true);
  next_page_Act->setEnabled(true);
  shift_page_up_Act->setEnabled(true);
  shift_page_down_Act->setEnabled(true);
  printmenu->setEnabled(true);
  recent_filesmenu->setEnabled(true);
  playback_realtime_Act->setEnabled(true);

  if(annotations_edited)
  {
    printf;
    messagewindow.setText("There are unsaved annotations,\n are you sure you want to close this file?");
    messagewindow.setIcon(QMessageBox::Question);
    messagewindow.setStandardButtons(QMessageBox::Cancel | QMessageBox::Close);
    messagewindow.setDefaultButton(QMessageBox::Cancel);
    button_nr = exit(2);
  }

  if(button_nr == QMessageBox::Cancel)
  {
    return;
  }

  annotations_edited = 0;

  remove_all_signals();

  while(files_open)
  {
    files_open--;
    if(edfheaderlist[files_open]->file_hdl != NULL)
    {
      fclose(edfheaderlist[files_open]->file_hdl);
    }
    free(edfheaderlist[files_open]->edfparam);
    free(edfheaderlist[files_open]);
    edfplus_annotation_delete_list(&annotationlist[files_open]);

    if(annotations_dock[files_open] != NULL)
    {
      annotations_dock[files_open]->docklist->close();
      delete annotations_dock[files_open];
      annotations_dock[files_open] = NULL;
    }

    delete sel_viewtime_act[files_open];
  }

  edfplus_annotation_delete_list(&annotationlist_backup);

  sel_viewtime = 0;

  pagetime = 10 * TIME_DIMENSION;

  timescale_doubler = 10;

  amplitude_doubler = 10;

  for(i=0; i<MAXZOOMHISTORY; i++)
  {
    zoomhistory->pntr = 0;
    zoomhistory->pagetime[i] = 10 * TIME_DIMENSION;
    for(j=0; j<MAXFILES; j++)
    {
      zoomhistory->viewtime[i][j] = 0;
    }
    for(j=0; j<MAXSIGNALS; j++)
    {
      zoomhistory->voltpercm[i][j] = 70;
      zoomhistory->screen_offset[i][j] = 0.0;
      for(k=0; k<MAXSIGNALS; k++)
      {
        zoomhistory->sensitivity[i][j][k] = 0.0475;
      }
    }
  }

  annotationEditDock->dockedit->hide();

  save_act->setEnabled(false);

  annotations_edited = 0;

  setWindowTitle(PROGRAM_NAME);

  close_filemenu->clear();

  if(!exit_in_progress)
  {
    setup_viewbuf();
  }
}


void UI_Mainwindow::show_about_dialog()
{
  UI_Aboutwindow aboutwindow(this);
}


void UI_Mainwindow::page_3cmsec()
{
  double mm;

  if(auto_dpi)
  {
    mm = maincurve->widthMM();

    pagetime = mm * 333333.3333;
  }
  else
  {
    pagetime = (long long)((((double)maincurve->width()) / (1.0 / x_pixelsizefactor) / 3.0) * TIME_DIMENSION);
  }

  setup_viewbuf();
}


void UI_Mainwindow::page_25mmsec()
{
  double mm;

  if(auto_dpi)
  {
    mm = maincurve->widthMM();

    pagetime = mm * 400000.0;
  }
  else
  {
    pagetime = (long long)((((double)maincurve->width()) / (1.0 / x_pixelsizefactor) / 2.5) * TIME_DIMENSION);
  }

  setup_viewbuf();
}


void UI_Mainwindow::page_50mmsec()
{
  double mm;

  if(auto_dpi)
  {
    mm = maincurve->widthMM();

    pagetime = mm * 200000.0;
  }
  else
  {
    pagetime = (long long)((((double)maincurve->width()) / (1.0 / x_pixelsizefactor) / 5.0) * TIME_DIMENSION);
  }

  setup_viewbuf();
}


void UI_Mainwindow::set_page_div2()
{
  int i;

  long long l_tmp, trshld=100LL;

  if((viewtime_sync==VIEWTIME_SYNCED_OFFSET)||(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)||(viewtime_sync==VIEWTIME_USER_DEF_SYNCED))
  {
    for(i=0; i<files_open; i++)
    {
      if(timescale_doubler == 50)
      {
        edfheaderlist[i]->viewtime += (pagetime * 0.3);
      }
      else
      {
        edfheaderlist[i]->viewtime += (pagetime / 4);
      }

      if(viewtime_sync!=VIEWTIME_USER_DEF_SYNCED)
      {
        l_tmp = edfheaderlist[i]->viewtime % TIME_DIMENSION;

        if(l_tmp < trshld)
        {
          edfheaderlist[i]->viewtime -= l_tmp;
        }

        if(l_tmp > (TIME_DIMENSION - trshld))
        {
          edfheaderlist[i]->viewtime += (TIME_DIMENSION - l_tmp);
        }
      }
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    if(timescale_doubler == 50)
    {
      edfheaderlist[sel_viewtime]->viewtime += (pagetime * 0.3);
    }
    else
    {
      edfheaderlist[sel_viewtime]->viewtime += (pagetime / 4);
    }

    l_tmp = edfheaderlist[sel_viewtime]->viewtime % TIME_DIMENSION;

    if(l_tmp < trshld)
    {
      edfheaderlist[sel_viewtime]->viewtime -= l_tmp;
    }

    if(l_tmp > (TIME_DIMENSION - trshld))
    {
      edfheaderlist[sel_viewtime]->viewtime += (TIME_DIMENSION - l_tmp);
    }
  }

  if(timescale_doubler == 10)
  {
    timescale_doubler = 50;

    pagetime /= 2;
  }
  else
  {
    if(timescale_doubler == 50)
    {
      timescale_doubler = 20;

      pagetime /= 2.5;
    }
    else
    {
      timescale_doubler = 10;

      pagetime /= 2;
    }
  }

  setup_viewbuf();
}


void UI_Mainwindow::set_page_mult2()
{
  int i;

  long long l_tmp, trshld=100LL;

  if((viewtime_sync==VIEWTIME_SYNCED_OFFSET)||(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)||(viewtime_sync==VIEWTIME_USER_DEF_SYNCED))
  {
    for(i=0; i<files_open; i++)
    {
      if(timescale_doubler == 20)
      {
        edfheaderlist[i]->viewtime -= (pagetime * 0.75);
      }
      else
      {
        edfheaderlist[i]->viewtime -= (pagetime / 2);
      }

      if(viewtime_sync!=VIEWTIME_USER_DEF_SYNCED)
      {
        l_tmp = edfheaderlist[i]->viewtime % TIME_DIMENSION;

        if(l_tmp < trshld)
        {
          edfheaderlist[i]->viewtime -= l_tmp;
        }

        if(l_tmp > (TIME_DIMENSION - trshld))
        {
          edfheaderlist[i]->viewtime += (TIME_DIMENSION - l_tmp);
        }
      }
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    if(timescale_doubler == 20)
    {
      edfheaderlist[sel_viewtime]->viewtime -= (pagetime * 0.75);
    }
    else
    {
      edfheaderlist[sel_viewtime]->viewtime -= (pagetime / 2);
    }

    l_tmp = edfheaderlist[sel_viewtime]->viewtime % TIME_DIMENSION;

    if(l_tmp < trshld)
    {
      edfheaderlist[sel_viewtime]->viewtime -= l_tmp;
    }

    if(l_tmp > (TIME_DIMENSION - trshld))
    {
      edfheaderlist[sel_viewtime]->viewtime += (TIME_DIMENSION - l_tmp);
    }
  }

  if(timescale_doubler == 10)
  {
    timescale_doubler = 20;

    pagetime *= 2;
  }
  else
  {
    if(timescale_doubler == 20)
    {
      timescale_doubler = 50;

      pagetime *= 2.5;
    }
    else
    {
      timescale_doubler = 10;

      pagetime *= 2;
    }
  }

  setup_viewbuf();
}


void UI_Mainwindow::set_display_time(QAction *action)
{
  if(action==page_10u)
  {
    pagetime = TIME_DIMENSION / 100000;

    timescale_doubler = 10;
  }
  if(action==page_20u)
  {
    pagetime = TIME_DIMENSION / 50000;

    timescale_doubler = 20;
  }
  if(action==page_50u)
  {
    pagetime = TIME_DIMENSION / 20000;

    timescale_doubler = 50;
  }
  if(action==page_100u)
  {
    pagetime = TIME_DIMENSION / 10000;

    timescale_doubler = 10;
  }
  if(action==page_200u)
  {
    pagetime = TIME_DIMENSION / 5000;

    timescale_doubler = 20;
  }
  if(action==page_500u)
  {
    pagetime = TIME_DIMENSION / 2000;

    timescale_doubler = 50;
  }
  if(action==page_1m)
  {
    pagetime = TIME_DIMENSION / 1000;

    timescale_doubler = 10;
  }
  if(action==page_2m)
  {
    pagetime = TIME_DIMENSION / 500;

    timescale_doubler = 20;
  }
  if(action==page_5m)
  {
    pagetime = TIME_DIMENSION / 200;

    timescale_doubler = 50;
  }
  if(action==page_10m)
  {
    pagetime = TIME_DIMENSION / 100;

    timescale_doubler = 10;
  }
  if(action==page_20m)
  {
    pagetime = TIME_DIMENSION / 50;

    timescale_doubler = 20;
  }
  if(action==page_50m)
  {
    pagetime = TIME_DIMENSION / 20;

    timescale_doubler = 50;
  }
  if(action==page_100m)
  {
    pagetime = TIME_DIMENSION / 10;

    timescale_doubler = 10;
  }
  if(action==page_200m)
  {
    pagetime = TIME_DIMENSION / 5;

    timescale_doubler = 20;
  }
  if(action==page_500m)
  {
    pagetime = TIME_DIMENSION / 2;

    timescale_doubler = 50;
  }
  if(action==page_1)
  {
    pagetime = TIME_DIMENSION;

    timescale_doubler = 10;
  }
  if(action==page_2)
  {
    pagetime = TIME_DIMENSION * 2;

    timescale_doubler = 20;
  }
  if(action==page_5)
  {
    pagetime = TIME_DIMENSION * 5;

    timescale_doubler = 50;
  }
  if(action==page_10)
  {
    pagetime = TIME_DIMENSION * 10;

    timescale_doubler = 10;
  }
  if(action==page_15)    pagetime = TIME_DIMENSION * 15;
  if(action==page_20)
  {
    pagetime = TIME_DIMENSION * 20;

    timescale_doubler = 20;
  }
  if(action==page_30)    pagetime = TIME_DIMENSION * 30;
  if(action==page_60)    pagetime = TIME_DIMENSION * 60;
  if(action==page_300)   pagetime = TIME_DIMENSION * 300;
  if(action==page_1200)  pagetime = TIME_DIMENSION * 1200;
  if(action==page_3600)  pagetime = TIME_DIMENSION * 3600;

  setup_viewbuf();
}


void UI_Mainwindow::set_user_defined_display_time()
{
  UI_Userdefined_timepage_Dialog set_displaytime_dialog(this);
}

*/
void UI_Mainwindow::set_display_time_whole_rec()
{
  int i;

  if(!files_open)  return;

  if(viewtime_sync==VIEWTIME_SYNCED_OFFSET)
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime = 0;
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime = 0;
  }

  if(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)
  {
    edfheaderlist[sel_viewtime]->viewtime = 0;

    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime = ((edfheaderlist[sel_viewtime]->utc_starttime - edfheaderlist[i]->utc_starttime) * TIME_DIMENSION) + edfheaderlist[sel_viewtime]->starttime_offset - - edfheaderlist[i]->starttime_offset;
      }
    }
  }

  if(viewtime_sync==VIEWTIME_USER_DEF_SYNCED)
  {
    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime -= edfheaderlist[sel_viewtime]->viewtime;
      }
    }

    edfheaderlist[sel_viewtime]->viewtime = 0;
  }

  pagetime = edfheaderlist[sel_viewtime]->datarecords * edfheaderlist[sel_viewtime]->long_data_record_duration;

  setup_viewbuf();
}

/*
void UI_Mainwindow::fit_signals_to_pane()
{
  int i, j,
      pane_size;

  if(!signalcomps)  return;

  pane_size = maincurve->height() / (signalcomps + 1);

  for(i=0; i<signalcomps; i++)
  {
    for(j=0; j<signalcomp[i]->num_of_signals; j++)
    {
      if(signalcomp[i]->max_dig_value!=signalcomp[i]->min_dig_value)
      {
        signalcomp[i]->sensitivity[j] = (double)pane_size / (double)(signalcomp[i]->max_dig_value - signalcomp[i]->min_dig_value);
      }
      else
      {
        signalcomp[i]->sensitivity[j] = pane_size;
      }
    }

    signalcomp[i]->voltpercm =
     signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].bitvalue
     / (signalcomp[i]->sensitivity[0] * pixelsizefactor);

    signalcomp[i]->screen_offset = ((signalcomp[i]->max_dig_value + signalcomp[i]->min_dig_value) / 2.0) * signalcomp[i]->sensitivity[0] * signalcomp[i]->polarity;
  }

  maincurve->drawCurve_stage_1();
}


void UI_Mainwindow::fit_signals_dc_offset()
{
  int i;

  if(!signalcomps)  return;

  for(i=0; i<signalcomps; i++)
  {
    signalcomp[i]->screen_offset = ((signalcomp[i]->max_dig_value + signalcomp[i]->min_dig_value) / 2.0) * signalcomp[i]->sensitivity[0] * signalcomp[i]->polarity;
  }

  maincurve->drawCurve_stage_1();
}


void UI_Mainwindow::set_amplitude(QAction *action)
{
  int i, j;

  double value=100.0, original_value, value2=100.0;


  if(action==amp_00001)
  {
    value = 0.0001;
    amplitude_doubler = 10;
  }
  if(action==amp_00002)
  {
    value = 0.0002;
    amplitude_doubler = 20;
  }
  if(action==amp_00005)
  {
    value = 0.0005;
    amplitude_doubler = 50;
  }
  if(action==amp_0001)
  {
    value = 0.001;
    amplitude_doubler = 10;
  }
  if(action==amp_0002)
  {
    value = 0.002;
    amplitude_doubler = 20;
  }
  if(action==amp_0005)
  {
    value = 0.005;
    amplitude_doubler = 50;
  }
  if(action==amp_001)
  {
    value = 0.01;
    amplitude_doubler = 10;
  }
  if(action==amp_002)
  {
    value = 0.02;
    amplitude_doubler = 20;
  }
  if(action==amp_005)
  {
    value = 0.05;
    amplitude_doubler = 50;
  }
  if(action==amp_01)
  {
    value = 0.1;
    amplitude_doubler = 10;
  }
  if(action==amp_02)
  {
    value = 0.2;
    amplitude_doubler = 20;
  }
  if(action==amp_05)
  {
    value = 0.5;
    amplitude_doubler = 50;
  }
  if(action==amp_1)
  {
    value = 1.0;
    amplitude_doubler = 10;
  }
  if(action==amp_2)
  {
    value = 2.0;
    amplitude_doubler = 20;
  }
  if(action==amp_5)
  {
    value = 5.0;
    amplitude_doubler = 50;
  }
  if(action==amp_10)
  {
    value = 10.0;
    amplitude_doubler = 10;
  }
  if(action==amp_20)
  {
    value = 20.0;
    amplitude_doubler = 20;
  }
  if(action==amp_50)
  {
    value = 50.0;
    amplitude_doubler = 50;
  }
  if(action==amp_100)
  {
    value = 100.0;
    amplitude_doubler = 10;
  }
  if(action==amp_200)
  {
    value = 200.0;
    amplitude_doubler = 20;
  }
  if(action==amp_500)
  {
    value = 500.0;
    amplitude_doubler = 50;
  }
  if(action==amp_1000)
  {
    value = 1000.0;
    amplitude_doubler = 10;
  }
  if(action==amp_2000)
  {
    value = 2000.0;
    amplitude_doubler = 20;
  }
  if(action==amp_5000)
  {
    value = 5000.0;
    amplitude_doubler = 50;
  }
  if(action==amp_10000)
  {
    value = 10000.0;
    amplitude_doubler = 10;
  }
  if(action==amp_20000)
  {
    value = 20000.0;
    amplitude_doubler = 20;
  }
  if(action==amp_50000)
  {
    value = 50000.0;
    amplitude_doubler = 50;
  }

  for(i=0; i<signalcomps; i++)
  {
    value2 = value;

    if(signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].bitvalue < 0.0)
    {
      value2 *= -1.0;
    }

    for(j=0; j<signalcomp[i]->num_of_signals; j++)
    {
      signalcomp[i]->sensitivity[j] = (signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[j]].bitvalue / value2) / pixelsizefactor;
    }

    original_value = signalcomp[i]->voltpercm;

    signalcomp[i]->voltpercm = value2;

    signalcomp[i]->screen_offset *= (original_value / value2);
  }

  maincurve->drawCurve_stage_1();
}


void UI_Mainwindow::set_amplitude_mult2()
{
  int i, j;

  for(i=0; i<signalcomps; i++)
  {
    if(signalcomp[i]->voltpercm < 0)
    {
      if(signalcomp[i]->voltpercm < -5000000.0)
      {
        return;
      }
    }
    else
    {
      if(signalcomp[i]->voltpercm > 5000000.0)
      {
        return;
      }
    }
  }

  for(i=0; i<signalcomps; i++)
  {
    if(amplitude_doubler != 20)
    {
      signalcomp[i]->voltpercm *= 2;

      signalcomp[i]->screen_offset /= 2;
    }
    else
    {
      signalcomp[i]->voltpercm *= 2.5;

      signalcomp[i]->screen_offset /= 2.5;
    }

    for(j=0; j<signalcomp[i]->num_of_signals; j++)
    {
      signalcomp[i]->sensitivity[j] = (signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[j]].bitvalue / signalcomp[i]->voltpercm) / pixelsizefactor;
    }
  }

  if(amplitude_doubler == 10)
  {
    amplitude_doubler = 20;
  }
  else
  {
    if(amplitude_doubler == 20)
    {
      amplitude_doubler = 50;
    }
    else
    {
      amplitude_doubler = 10;
    }
  }

  maincurve->drawCurve_stage_1();
}


void UI_Mainwindow::set_amplitude_div2()
{
  int i, j;

  for(i=0; i<signalcomps; i++)
  {
    if(signalcomp[i]->voltpercm < 0)
    {
      if(signalcomp[i]->voltpercm > -0.000001)
      {
        return;
      }
    }
    else
    {
      if(signalcomp[i]->voltpercm < 0.000001)
      {
        return;
      }
    }
  }

  for(i=0; i<signalcomps; i++)
  {
    if(amplitude_doubler != 50)
    {
      signalcomp[i]->voltpercm /= 2;

      signalcomp[i]->screen_offset *= 2;
    }
    else
    {
      signalcomp[i]->voltpercm /= 2.5;

      signalcomp[i]->screen_offset *= 2.5;
    }

    for(j=0; j<signalcomp[i]->num_of_signals; j++)
    {
      signalcomp[i]->sensitivity[j] = (signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[j]].bitvalue / signalcomp[i]->voltpercm) / pixelsizefactor;
    }
  }

  if(amplitude_doubler == 10)
  {
    amplitude_doubler = 50;
  }
  else
  {
    if(amplitude_doubler == 50)
    {
      amplitude_doubler = 20;
    }
    else
    {
      amplitude_doubler = 10;
    }
  }

  maincurve->drawCurve_stage_1();
}


void UI_Mainwindow::load_predefined_mtg(QAction *action)
{
  int i;

  if(!files_open)
  {
    return;
  }

  for(i=0; i < MAXPREDEFINEDMONTAGES; i++)
  {
    if(action==load_predefined_mtg_act[i])
    {
      if(predefined_mtg_path[i][0] != 0)
      {
        strcpy(montagepath, &predefined_mtg_path[i][0]);

        UI_LoadMontagewindow load_mtg(this, montagepath);

        return;
      }
    }
  }
}


void UI_Mainwindow::export_to_ascii()
{
  if(!files_open)
  {
    printf("You have to open a file first.");
    exit(2);
    return;
  }

  UI_AsciiExportwindow exportdialog(this);
}


void UI_Mainwindow::export_ecg_rr_interval_to_ascii()
{
  UI_ECGExport ECG_export(this);
}


void UI_Mainwindow::export_annotations()
{
  if(!files_open)
  {
    printf("You have to open a file first.");
    exit(2);
    return;
  }

  UI_ExportAnnotationswindow exportAnnotsDialog(this);
}


void UI_Mainwindow::import_annotations()
{
  UI_ImportAnnotationswindow importAnnotsDialog(this);
}


void UI_Mainwindow::check_edf_compatibility()
{
  if(!files_open)
  {
    printf("You have to open the file first.");
    exit(2);
    return;
  }

  UI_EDFCompatwindow edfcompatdialog(this);
}


void UI_Mainwindow::print_to_img_640x480()
{
  maincurve->print_to_image(640, 480);
}


void UI_Mainwindow::print_to_img_800x600()
{
  maincurve->print_to_image(800, 600);
}


void UI_Mainwindow::print_to_img_1024x768()
{
  maincurve->print_to_image(1024, 768);
}


void UI_Mainwindow::print_to_img_1280x1024()
{
  maincurve->print_to_image(1280, 1024);
}


void UI_Mainwindow::print_to_img_1600x1200()
{
  maincurve->print_to_image(1600, 1200);
}


long long UI_Mainwindow::get_long_time(char *str)
{
  int i, len, hasdot=0, dotposition=0;

  long long value=0, radix;

  str = str + 1;

  len = strlen(str);

  for(i=0; i<len; i++)
  {
    if(str[i]=='.')
    {
      hasdot = 1;
      dotposition = i;
      break;
    }
  }

  if(hasdot)
  {
    radix = TIME_DIMENSION;

    for(i=dotposition-1; i>=0; i--)
    {
        value += ((long long)(str[i] - 48)) * radix;
        radix *= 10;
    }

    radix = TIME_DIMENSION / 10;

    for(i=dotposition+1; i<len; i++)
    {
        value += ((long long)(str[i] - 48)) * radix;
        radix /= 10;
    }
  }
  else
  {
    radix = TIME_DIMENSION;

    for(i=len-1; i>=0; i--)
    {
        value += ((long long)(str[i] - 48)) * radix;
        radix *= 10;
    }
  }

  if(str[-1]=='-')  value = -value;

  return(value);
}


void UI_Mainwindow::recent_file_action_func(QAction *action)
{
  strcpy(path, action->text().toLocal8Bit().data());

  cmdlineargument = 1;

  open_new_file();
}


void UI_Mainwindow::save_montage()
{
  if(!files_open)  return;

  if(!signalcomps)  return;

  UI_SaveMontagewindow save_mtg(this);
}


void UI_Mainwindow::load_montage()
{
  if(!files_open)  return;

  UI_LoadMontagewindow load_mtg(this);
}


void UI_Mainwindow::view_montage()
{
  UI_ViewMontagewindow view_mtg(this);
}


void UI_Mainwindow::show_this_montage()
{
  UI_ShowActualMontagewindow show_mtg(this);
}


void UI_Mainwindow::print_to_edf()
{
  print_screen_to_edf(this);
}


void UI_Mainwindow::print_to_bdf()
{
  print_screen_to_bdf(this);
}


void UI_Mainwindow::set_dc_offset_to_zero()
{
  int i;

  if(!signalcomps)
  {
    return;
  }

  for(i=0; i<signalcomps; i++)
  {
    signalcomp[i]->screen_offset = 0.0;
  }

  maincurve->drawCurve_stage_1();
}


void UI_Mainwindow::show_help()
{
#ifdef Q_OS_LINUX
  QDesktopServices::openUrl(QUrl("file:///usr/share/doc/edfbrowser/manual.html"));
#endif

#ifdef Q_OS_WIN32
  char p_path[MAX_PATH_LENGTH];

  strcpy(p_path, "file:///");
  strcat(p_path, specialFolder(CSIDL_PROGRAM_FILES).toLocal8Bit().data());
  strcat(p_path, "\\EDFbrowser\\manual.html");
  QDesktopServices::openUrl(QUrl(p_path));
#endif
}


void UI_Mainwindow::show_kb_shortcuts()
{
  printf(QMessageBox::NoIcon,
   "Keyboard shortcuts",
   "PgDn\t\tnext page\n"
   "PgUp\t\tformer page\n"
   "Right Arrow\t\tshift right one tenth of pagetime\n"
   "Left Arrow\t\tshift left one tenth of pagetime\n"
   "Plus\t\tincrease sensitivity\n"
   "Minus\t\tdecrease sensitivity\n"
   "Up Arrow\t\tshift up\n"
   "Down Arrow\t\tshift down\n"
   "Ctrl-Home\t\tgo to start of file\n"
   "Ctrl-End\t\tgo to end of file\n"
   "Ctrl++\t\tzoom in\n"
   "Ctrl+-\t\tzoom out\n"
   "F1 - F8\t\tload predefined montage\n"
   "Esc\t\tremove crosshairs or floating ruler\n"

   "\nafter zooming in by dragging a rectangle:\n"
   "Backspace\t\tzoom back\n"
   "Insert\t\tzoom in\n"
#ifdef Q_OS_WIN32
   "\nCtrl+O\t\tOpen a file\n"
   "Ctrl+Shift+O\t\tOpen a stream\n"
   "Ctrl+F4\tClose all files\n"
   "Alt+F4\tExit program\n"
#else
   "\nCtrl+O\t\tOpen a file\n"
   "Ctrl+Shift+O\t\tOpen a stream\n"
   "Ctrl+W\t\tClose all files\n"
   "Ctrl+Q\t\tExit program\n"
#endif
   "\nMousewheel\t\tshift left or right\n"
   "Ctrl+Mousewheel\tzoom in or out\n"
   "Keep middle mousebutton pressed to drag horizontally\n"

   "\nCtrl+Space\t\tToggle Playback\n"
#ifdef Q_OS_LINUX
   "Ctrl+Shift+V\t\tPlayback with video"
#endif
  );

  exit(2);
}


#ifdef Q_OS_WIN32

QString UI_Mainwindow::specialFolder(int type)
{
    QString result;

    QLibrary library(QLatin1String("shell32"));
    QT_WA( {
        typedef BOOL (WINAPI*GetSpecialFolderPath)(HWND, LPTSTR, int, BOOL);
        GetSpecialFolderPath SHGetSpecialFolderPath = (GetSpecialFolderPath)library.resolve("SHGetSpecialFolderPathW");
        if (SHGetSpecialFolderPath) {
            TCHAR path[MAX_PATH];
            SHGetSpecialFolderPath(0, path, type, false);
            result = QString::fromUtf16((ushort*)path);
        }
    } , {
        typedef BOOL (WINAPI*GetSpecialFolderPath)(HWND, char*, int, BOOL);
        GetSpecialFolderPath SHGetSpecialFolderPath = (GetSpecialFolderPath)library.resolve("SHGetSpecialFolderPathA");
        if (SHGetSpecialFolderPath) {
            char path[MAX_PATH];
            SHGetSpecialFolderPath(0, path, type, false);
            result = QString::fromLocal8Bit(path);
        }
    } );

    if (result.isEmpty()) {
         result = QLatin1String("C:\\temp");
    }

    return result;
}

#endif



int UI_Mainwindow::file_is_opened(const char *o_path)
{
  int i;


  for(i=0; i<files_open; i++)
  {
    if(!strcmp(edfheaderlist[i]->filename, o_path))
    {
      return(1);
    }
  }

  return(0);
}


void UI_Mainwindow::organize_signals()
{
  UI_SignalChooser signalchooser(this, 3);
}


void UI_Mainwindow::edfplus_annotation_remove_duplicates()
{
  int i, j, k, list_size, dup_cnt=0;

  char str[256];

  struct annotationblock **list, *annot, *annot_cmp;

  if(!files_open)
  {
    printf("You have to open a file first.");
    exit(2);
    return;
  }

  QProgressDialog progress("Checking for duplicates...", "Abort", 0, 10, this);
  progress.setWindowModality(Qt::WindowModal);
  progress.setMinimumDuration(200);

  for(k=0; k<files_open; k++)
  {
    list = &annotationlist[k];

    if(*list==NULL)
    {
      continue;
    }

    list_size = edfplus_annotation_count(list);

    if(list_size < 2)  continue;

    progress.setMaximum(list_size);

    for(j=0; j<list_size; j++)
    {
      progress.setValue(j);

      qApp->processEvents();

      if(progress.wasCanceled() == true)
      {
        break;
      }

      annot_cmp = edfplus_annotation_item(list, j);

      for(i=j; i<list_size; i++)
      {
        if(i!=j)
        {
          annot = edfplus_annotation_item(list, i);

          if(annot->onset != annot_cmp->onset)  continue;

          if(annot->file_num != annot_cmp->file_num)  continue;

          if(strcmp(annot->annotation, annot_cmp->annotation))  continue;

          if(strcmp(annot->duration, annot_cmp->duration))  continue;

          edfplus_annotation_delete(list, i);

          dup_cnt++;

          list_size--;

          if(j)  j--;

          if(i)  i--;
        }
      }
    }
  }

  progress.reset();

  if(dup_cnt)
  {
    for(k=0; k<files_open; k++)
    {
      if(annotations_dock[k] != NULL)
      {
        annotations_dock[k]->updateList();
      }
    }

    annotations_edited = 1;

    save_act->setEnabled(true);
  }

  snprintf(str, 256, "Removed %i duplicates from the annotationlist(s)", dup_cnt);
  printf(QMessageBox::Information, "Ready", str);
  exit(2);
}
*/

struct signalcompblock * UI_Mainwindow::create_signalcomp_copy(struct signalcompblock *original_signalcomp)
{
  int i;

  char spec_str[256],
       *filter_spec,
       *err;

  struct signalcompblock *newsignalcomp;

  if(signalcomps >= MAXSIGNALS)
  {
	  return(NULL);
  }

  if(original_signalcomp->ecg_filter != NULL)
  {
    return(NULL);
  }

  //if(original_signalcomp->zratio_filter != NULL)
  //{
  //  return(NULL);
  //}

  newsignalcomp = (struct signalcompblock *)calloc(1, sizeof(struct signalcompblock));
  if(newsignalcomp == NULL)
  {
    printf("malloc() error");
    exit(2);
    return(NULL);
  }

  memcpy(newsignalcomp, original_signalcomp, sizeof(struct signalcompblock));

// TODO: investigate if all the filters below are important

  /*
  if(newsignalcomp->spike_filter)
  {
    newsignalcomp->spike_filter = create_spike_filter_copy(original_signalcomp->spike_filter);
    if(newsignalcomp->spike_filter == NULL)
    {
      printf("malloc() error");
      exit(2);
      free(signalcomp);
      return(NULL);
    }
  }
  

  for(i=0; i<newsignalcomp->filter_cnt; i++)
  {
    newsignalcomp->filter[i] = create_filter_copy(original_signalcomp->filter[i]);
    if(newsignalcomp->filter[i] == NULL)
    {
      printf("malloc() error");
      exit(2);
      free(signalcomp);
      return(NULL);
    }
  }

  for(i=0; i<newsignalcomp->ravg_filter_cnt; i++)
  {
    newsignalcomp->ravg_filter[i] = create_ravg_filter_copy(original_signalcomp->ravg_filter[i]);
    if(newsignalcomp->ravg_filter[i] == NULL)
    {
      printf("malloc() error");
      exit(2);
      free(signalcomp);
      return(NULL);
    }
  }

  for(i=0; i<newsignalcomp->fidfilter_cnt; i++)
  {
    if(newsignalcomp->fidfilter_type[i] == 0)
    {
      if(newsignalcomp->fidfilter_model[i] == 0)
      {
        sprintf(spec_str, "HpBu%i/%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i]);
      }

      if(newsignalcomp->fidfilter_model[i] == 1)
      {
        sprintf(spec_str, "HpCh%i/%f/%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_ripple[i], newsignalcomp->fidfilter_freq[i]);
      }

      if(newsignalcomp->fidfilter_model[i] == 2)
      {
        sprintf(spec_str, "HpBe%i/%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i]);
      }
    }

    if(newsignalcomp->fidfilter_type[i] == 1)
    {
      if(newsignalcomp->fidfilter_model[i] == 0)
      {
        sprintf(spec_str, "LpBu%i/%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i]);
      }

      if(newsignalcomp->fidfilter_model[i] == 1)
      {
        sprintf(spec_str, "LpCh%i/%f/%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_ripple[i], newsignalcomp->fidfilter_freq[i]);
      }

      if(newsignalcomp->fidfilter_model[i] == 2)
      {
        sprintf(spec_str, "LpBe%i/%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i]);
      }
    }

    if(newsignalcomp->fidfilter_type[i] == 2)
    {
      if(newsignalcomp->fidfilter_model[i] == 0)
      {
        sprintf(spec_str, "BsRe/%i/%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i]);
      }
    }

    if(newsignalcomp->fidfilter_type[i] == 3)
    {
      if(newsignalcomp->fidfilter_model[i] == 0)
      {
        sprintf(spec_str, "BpBu%i/%f-%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i], newsignalcomp->fidfilter_freq2[i]);
      }

      if(newsignalcomp->fidfilter_model[i] == 1)
      {
        sprintf(spec_str, "BpCh%i/%f/%f-%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_ripple[i], newsignalcomp->fidfilter_freq[i], newsignalcomp->fidfilter_freq2[i]);
      }

      if(newsignalcomp->fidfilter_model[i] == 2)
      {
        sprintf(spec_str, "BpBe%i/%f-%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i], newsignalcomp->fidfilter_freq2[i]);
      }
    }

    if(newsignalcomp->fidfilter_type[i] == 4)
    {
      if(newsignalcomp->fidfilter_model[i] == 0)
      {
        sprintf(spec_str, "BsBu%i/%f-%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i], newsignalcomp->fidfilter_freq2[i]);
      }

      if(newsignalcomp->fidfilter_model[i] == 1)
      {
        sprintf(spec_str, "BsCh%i/%f/%f-%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_ripple[i], newsignalcomp->fidfilter_freq[i], newsignalcomp->fidfilter_freq2[i]);
      }

      if(newsignalcomp->fidfilter_model[i] == 2)
      {
        sprintf(spec_str, "BsBe%i/%f-%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i], newsignalcomp->fidfilter_freq2[i]);
      }
    }

    filter_spec = spec_str;

    err = fid_parse(((double)(newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[0]].smp_per_record)) / newsignalcomp->edfhdr->data_record_duration,
                    &filter_spec,
                    &newsignalcomp->fidfilter[i]);

    if(err != NULL)
    {
      printf(err);
      exit(2);
      free(err);
      free(signalcomp);
      return(NULL);
    }

    newsignalcomp->fid_run[i] = fid_run_new(newsignalcomp->fidfilter[i], &newsignalcomp->fidfuncp[i]);

    newsignalcomp->fidbuf[i] = fid_run_newbuf(newsignalcomp->fid_run[i]);
    newsignalcomp->fidbuf2[i] = fid_run_newbuf(newsignalcomp->fid_run[i]);

//    memcpy(newsignalcomp->fidbuf[i], original_signalcomp->fidbuf[i], fid_run_bufsize(original_signalcomp->fid_run[i]));
//    memcpy(newsignalcomp->fidbuf2[i], original_signalcomp->fidbuf2[i], fid_run_bufsize(original_signalcomp->fid_run[i]));
  }

 */

  signalcomp[signalcomps] = newsignalcomp;

  signalcomps++;

  return(newsignalcomp);
}


























