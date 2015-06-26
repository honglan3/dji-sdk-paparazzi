/*
 * Copyright (C) 2014 Freek van Tienen
 *
 * This file is part of paparazzi.
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * @file gps_datalink.c
 * @brief GPS system based on datalink
 *
 * This GPS parses the datalink REMOTE_GPS packet and sets the
 * GPS structure to the values received.
 */

#include "subsystems/gps.h"

#ifdef GPS_RC
#include "state.h"
#include "subsystems/radio_control/spektrum_arch.h"
#endif

bool_t gps_available;   ///< Is set to TRUE when a new REMOTE_GPS packet is received and parsed

/** GPS initialization */
void gps_impl_init(void)
{
  gps.fix = GPS_FIX_NONE;
  gps_available = FALSE;
  gps.gspeed = 700; // To enable course setting
  gps.cacc = 0; // To enable course setting
}

/** Parse the REMOTE_GPS datalink packet */
void parse_gps_datalink(uint8_t numsv, int32_t ecef_x, int32_t ecef_y, int32_t ecef_z, int32_t lat, int32_t lon,
                        int32_t alt,
                        int32_t hmsl, int32_t ecef_xd, int32_t ecef_yd, int32_t ecef_zd, uint32_t tow, int32_t course)
{

  gps.lla_pos.lat = lat;
  gps.lla_pos.lon = lon;
  gps.lla_pos.alt = alt;
  gps.hmsl        = hmsl;

  gps.ecef_pos.x = ecef_x;
  gps.ecef_pos.y = ecef_y;
  gps.ecef_pos.z = ecef_z;

  gps.ecef_vel.x = ecef_xd;
  gps.ecef_vel.y = ecef_yd;
  gps.ecef_vel.z = ecef_zd;

  gps.course = course;
  gps.num_sv = numsv;
  gps.tow = tow;
  gps.fix = GPS_FIX_3D;
  gps_available = TRUE;

#if GPS_USE_LATLONG
  // Computes from (lat, long) in the referenced UTM zone
  struct LlaCoor_f lla_f;
  LLA_FLOAT_OF_BFP(lla_f, gps.lla_pos);
  struct UtmCoor_f utm_f;
  utm_f.zone = nav_utm_zone0;
  // convert to utm
  utm_of_lla_f(&utm_f, &lla_f);
  // copy results of utm conversion
  gps.utm_pos.east = utm_f.east * 100;
  gps.utm_pos.north = utm_f.north * 100;
  gps.utm_pos.alt = gps.lla_pos.alt;
  gps.utm_pos.zone = nav_utm_zone0;
#endif
}

/** Parse the GPS RC channels */
void parse_gps_rc(int16_t channels[])
{
  uint16_t gps_data[7];
  uint8_t i, chan_idx;

  for(i = 0; i < 7; i++)
    gps_data[i] = 0;

  for(i = 0, chan_idx = 0; i < 8; i++) {
    uint8_t chan_num = (channels[chan_idx] >> 10) & 0x0F;

    /* Skip radio mode RADIO_MODE */
    if(chan_num == RADIO_MODE)
      chan_idx++;

    // Set the GPS data
    gps_data[i] = ((channels[chan_idx]&0x3FF) << 5) + ((channels[++chan_idx]&0x3E0) >> 5);
    if(i < 6) gps_data[++i] = ((channels[chan_idx]&0x1F) << 10) + (channels[++chan_idx]&0x3FF);
  }

  /* Convert the position */
  struct EnuCoor_i enu_coord;
  enu_coord.x = gps_data[0] >> 3;
  enu_coord.y = gps_data[1] >> 3;
  enu_coord.z = gps_data[2] >> 3;
  ecef_of_enu_pos_i(&gps.ecef_pos, &state.ned_origin_i, &enu_coord);
  lla_of_ecef_i(&gps.lla_pos, &gps.ecef_pos);

  /* Maybe should convert this? */
  gps.ecef_vel.x = gps_data[3] >> 3;
  gps.ecef_vel.y = gps_data[4] >> 3;
  gps.ecef_vel.z = gps_data[5] >> 3;

  /* Set course */
  gps.course = gps_data[6];

  /* Fake some values */
  gps.num_sv = 0;
  gps.tow = 0;
  gps.fix = GPS_FIX_3D;
  gps.hmsl = 0; // TODO??
  gps_available = TRUE;


#if GPS_USE_LATLONG
  // Computes from (lat, long) in the referenced UTM zone
  struct LlaCoor_f lla_f;
  lla_f.lat = ((float) gps.lla_pos.lat) / 1e7;
  lla_f.lon = ((float) gps.lla_pos.lon) / 1e7;
  struct UtmCoor_f utm_f;
  utm_f.zone = nav_utm_zone0;
  // convert to utm
  utm_of_lla_f(&utm_f, &lla_f);
  // copy results of utm conversion
  gps.utm_pos.east = utm_f.east*100;
  gps.utm_pos.north = utm_f.north*100;
  gps.utm_pos.alt = gps.lla_pos.alt;
  gps.utm_pos.zone = nav_utm_zone0;
#endif
}
