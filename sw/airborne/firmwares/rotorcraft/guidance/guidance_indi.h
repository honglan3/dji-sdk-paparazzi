/*
 * Copyright (C) 2015 Ewoud Smeur <ewoud.smeur@gmail.com>
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
 * @file firmwares/rotorcraft/guidance/guidance_indi.h
 *
 * A guidance mode based on Incremental Nonlinear Dynamic Inversion
 * Come to ICRA2016 to learn more!
 *
 */

#ifndef GUIDANCE_INDI_H
#define GUIDANCE_INDI_H

#include "std.h"
#include "math/pprz_algebra_int.h"
#include "math/pprz_algebra_float.h"

extern float guidance_indi_pos_gain;
extern float guidance_indi_speed_gain;
extern float filter_omega;

extern void guidance_indi_enter(void);
extern void guidance_indi_run(bool in_flight, int32_t heading);
extern void guidance_indi_filter_attitude(void);
extern void guidance_indi_calcG(struct FloatMat33 *Gmat);
extern void guidance_indi_filter_accel(void);
extern void stabilization_attitude_set_setpoint_rp_quat_f(bool in_flight, int32_t heading);

#endif /* GUIDANCE_INDI_H */
