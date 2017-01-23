/*
** Xtank
**
** Copyright 1993 by Pix Technologies Corp.
**
** lowlib.h
*/

/*
$Author: lidl $
$Id: lowlib.h,v 1.1.1.1 1995/02/01 00:25:40 lidl Exp $
*/

#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* lowlib.c */
#ifdef __bsdi__
#define Angle double
#define ID int
#endif
extern void get_location P_((Location *loc));
extern FLOAT max_speed P_((void));
extern FLOAT speed P_((void));
extern Angle heading P_((void));
extern FLOAT acc P_((void));
extern FLOAT engine_acc P_((void));
extern FLOAT tread_acc P_((void));
extern FLOAT get_abs_drive P_((void));
extern void vehicle_size P_((int *width, int *height));
extern void turn_vehicle P_((Angle desired_heading));
extern void turn_vehicle_human P_((Angle desired_heading));
extern Angle turn_rate P_((double abs_speed));
extern void set_abs_drive P_((double abs_speed));
extern void set_rel_drive P_((double rel_drive));
extern void set_safety P_((int status));
extern void set_teleport P_((int status));
extern void turret_position P_((TurretNum turret, int *xp, int *yp));
extern int num_turrets P_((void));
extern Angle turret_angle P_((TurretNum num));
extern Angle turret_turn_rate P_((TurretNum num));
extern void turn_turret P_((TurretNum num, Angle angle));
extern Angle aim_turret P_((TurretNum num, int dx, int dy));
extern Boolean weapon_on P_((WeaponNum num));
extern int turn_on_weapon P_((WeaponNum num));
extern int turn_off_weapon P_((WeaponNum num));
extern int toggle_weapon P_((WeaponNum num));
extern WeaponStatus fire_weapon P_((WeaponNum num));
extern int num_weapons P_((void));
extern int get_weapon P_((WeaponNum num, Weapon_info *winfo));
extern int weapon_time P_((WeaponNum num));
extern int weapon_ammo P_((WeaponNum num));
extern int armor P_((Side num));
extern int max_armor P_((Side num));
extern int protection P_((void));
extern Box (*map_get())[GRID_HEIGHT];
extern WallType wall P_((WallSide dir, int x, int y));
extern LandmarkType landmark P_((int x, int y));
extern int get_landmarks P_((int *num_landmark_infos, Landmark_info landmark_info[]));
extern void get_outpost_loc P_((int x, int y, int frame_num, int *xret, int *yret));
extern int get_blips P_((int *num_blip_infos, Blip_info blip_info[]));
extern void get_vehicles P_((int *num_vehicle_infos, Vehicle_info vehicle_info[]));
extern void get_self P_((Vehicle_info *v_info));
extern void get_bullets P_((int *num_bullet_infos, Bullet_info bullet_info[]));
extern Team team P_((ID vid));
extern int number_vehicles P_((void));
extern void get_settings P_((Settings_info *si));
extern int get_money P_((void));
extern int get_fuel_cost P_((void));
extern int get_armor_cost P_((void));
extern int get_ammo_cost P_((WeaponNum wn));
extern void throw_discs P_((double dspeed, Boolean delay));
extern void spin_discs P_((Spin dir));
extern int num_discs P_((void));
extern void get_discs P_((int *num_disc_infos, Disc_info disc_info[]));
extern int messages P_((void));
#ifndef __bsdi__
extern void send_msg P_((Byte recipient, Opcode opcode, Byte * data));
#endif
extern Boolean receive_msg P_((Message *m));
extern FLOAT fuel P_((void));
extern FLOAT max_fuel P_((void));
extern int heat P_((void));
extern int heat_sinks P_((void));
extern Boolean has_special P_((SpecialType st));
extern int frame_number P_((void));
extern int num_kills P_((void));
extern int score P_((void));
extern void done P_((void));
extern void set_cleanup_func P_((void (*funcp) (), void *argp));
#ifdef __bsdi__
#undef Angle
#endif

/* highlib.c */
extern void turn_all_turrets P_((Angle angle));
extern void aim_all_turrets P_((int dx, int dy));
extern int fire_all_weapons P_((void));
extern Boolean clear_path P_((Location *start, Location *finish));
extern Boolean get_closest_enemy P_((Vehicle_info *enemy));

#undef P_
