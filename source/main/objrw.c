#include <string.h>
#include "object.h"

typedef signed char sbyte;
typedef ubyte pint[4], puint[4], pfix[4];
typedef ubyte pshort[2], pushort[2], pfixang[2];
typedef struct pvms_vector { pfix x, y, z; } pvms_vector;
typedef struct pvms_angvec { pfixang p, b, h; } pvms_angvec;
typedef struct pvms_matrix { pvms_vector rvec, uvec, fvec; } pvms_matrix;

typedef struct physics_info_rw {
	pvms_vector  velocity;   // velocity vector of this object
	pvms_vector  thrust;     // constant force applied to this object
	pfix         mass;       // the mass of this object
	pfix         drag;       // how fast this slows down
	pfix         brakes;     // how much brakes applied
	pvms_vector  rotvel;     // rotational velecity (angles)
	pvms_vector  rotthrust;  // rotational acceleration
	pfixang      turnroll;   // rotation caused by turn banking
	pushort      flags;      // misc physics flags
} physics_info_rw;

typedef struct ai_static_rw {
	ubyte   behavior;               //
	sbyte   flags[MAX_AI_FLAGS];    // various flags, meaning defined by constants
	pshort   hide_segment;           // Segment to go to for hiding.
	pshort   hide_index;             // Index in Path_seg_points
	pshort   path_length;            // Length of hide path.
	sbyte   cur_path_index;         // Current index in path.
	sbyte   dying_sound_playing;    // !0 if this robot is playing its dying sound.
	pshort   danger_laser_num;
	pint     danger_laser_signature;
	pfix     dying_start_time;       // Time at which this robot started dying.
} ai_static_rw;

typedef struct laser_info_rw {
	pshort   parent_type;        // The type of the parent of this object
	pshort   parent_num;         // The object's parent's number
	pint     parent_signature;   // The object's parent's signature...
	pfix     creation_time;      // Absolute time of creation.
	pshort   last_hitobj;        // For persistent weapons (survive object collision), object it most recently hit.
	pshort   track_goal;         // Object this object is tracking.
	pfix     multiplier;         // Power if this is a fusion bolt (or other super weapon to be added).
} laser_info_rw;

typedef struct explosion_info_rw {
    pfix     spawn_time;         // when lifeleft is < this, spawn another
    pfix     delete_time;        // when to delete object
    pshort   delete_objnum;      // and what object to delete
    pshort   attach_parent;      // explosion is attached to this object
    pshort   prev_attach;        // previous explosion in attach list
    pshort   next_attach;        // next explosion in attach list
} explosion_info_rw;

typedef struct light_info_rw {
    pfix     intensity;          // how bright the light is
} light_info_rw;

typedef struct powerup_info_rw {
	pint     count;          // how many/much we pick up (vulcan cannon only?)
	pfix     creation_time;  // Absolute time of creation.
	pint     flags;          // spat by player?
} powerup_info_rw;

typedef struct vclip_info_rw {
	pint     vclip_num;
	pfix     frametime;
	sbyte   framenum;
} vclip_info_rw;

typedef struct polyobj_info_rw {
	pint     model_num;          // which polygon model
	pvms_angvec anim_angles[MAX_SUBMODELS]; // angles for each subobject
	pint     subobj_flags;       // specify which subobjs to draw
	pint     tmap_override;      // if this is not -1, map all face to this
	pint     alt_textures;       // if not -1, use these textures instead
} polyobj_info_rw;

typedef struct object_rw {
	pint     signature;      // Every object ever has a unique signature...
	ubyte   type;           // what type of object this is... robot, weapon, hostage, powerup, fireball
	ubyte   id;             // which form of object...which powerup, robot, etc.
	pshort   next,prev;      // id of next and previous connected object in Objects, -1 = no connection
	ubyte   control_type;   // how this object is controlled
	ubyte   movement_type;  // how this object moves
	ubyte   render_type;    // how this object renders
	ubyte   flags;          // misc flags
	pshort   segnum;         // segment number containing object
	pshort   attached_obj;   // number of attached fireball object
	pvms_vector pos;         // absolute x,y,z coordinate of center of object
	pvms_matrix orient;      // orientation of object in world
	pfix     size;           // 3d size of object - for collision detection
	pfix     shields;        // Starts at maximum, when <0, object dies..
	pvms_vector last_pos;    // where object was last frame
	sbyte   contains_type;  // Type of object this object contains (eg, spider contains powerup)
	sbyte   contains_id;    // ID of object this object contains (eg, id = blue type = key)
	sbyte   contains_count; // number of objects of type:id this object contains
	sbyte   matcen_creator; // Materialization center that created this object, high bit set if matcen-created
	pfix     lifeleft;       // how long until goes away, or 7fff if immortal
	// -- Removed, MK, 10/16/95, using lifeleft instead: int     lightlevel;

	// movement info, determined by MOVEMENT_TYPE
	union {
		physics_info_rw phys_info; // a physics object
		pvms_vector   spin_rate; // for spinning objects
	} mtype ;

	// control info, determined by CONTROL_TYPE
	union {
		laser_info_rw   laser_info;
		explosion_info_rw  expl_info;      // NOTE: debris uses this also
		ai_static_rw    ai_info;
		struct light_info_rw      light_info;     // why put this here?  Didn't know what else to do with it.
		powerup_info_rw powerup_info;
	} ctype ;

	// render info, determined by RENDER_TYPE
	union {
		polyobj_info_rw    pobj_info;      // polygon model
		struct vclip_info_rw      vclip_info;     // vclip
	} rtype;
} object_rw;

#define COPY_B(a, b) a = b
#define COPY_S(a, b) memcpy(&a, &b, sizeof(a))
#define COPY_I(a, b) memcpy(&a, &b, sizeof(a))

void object_to_object_rw(object *obj, object_rw *obj_rw)
{
	COPY_I(obj_rw->signature     , obj->signature);
	COPY_B(obj_rw->type          , obj->type);
	COPY_B(obj_rw->id            , obj->id);
	COPY_S(obj_rw->next          , obj->next);
	COPY_S(obj_rw->prev          , obj->prev);
	COPY_B(obj_rw->control_type  , obj->control_type);
	COPY_B(obj_rw->movement_type , obj->movement_type);
	COPY_B(obj_rw->render_type   , obj->render_type);
	COPY_B(obj_rw->flags         , obj->flags);
	COPY_S(obj_rw->segnum        , obj->segnum);
	COPY_S(obj_rw->attached_obj  , obj->attached_obj);
	COPY_I(obj_rw->pos.x         , obj->pos.x);
	COPY_I(obj_rw->pos.y         , obj->pos.y);
	COPY_I(obj_rw->pos.z         , obj->pos.z);
	COPY_I(obj_rw->orient.rvec.x , obj->orient.rvec.x);
	COPY_I(obj_rw->orient.rvec.y , obj->orient.rvec.y);
	COPY_I(obj_rw->orient.rvec.z , obj->orient.rvec.z);
	COPY_I(obj_rw->orient.fvec.x , obj->orient.fvec.x);
	COPY_I(obj_rw->orient.fvec.y , obj->orient.fvec.y);
	COPY_I(obj_rw->orient.fvec.z , obj->orient.fvec.z);
	COPY_I(obj_rw->orient.uvec.x , obj->orient.uvec.x);
	COPY_I(obj_rw->orient.uvec.y , obj->orient.uvec.y);
	COPY_I(obj_rw->orient.uvec.z , obj->orient.uvec.z);
	COPY_I(obj_rw->size          , obj->size);
	COPY_I(obj_rw->shields       , obj->shields);
	COPY_I(obj_rw->last_pos.x    , obj->last_pos.x);
	COPY_I(obj_rw->last_pos.y    , obj->last_pos.y);
	COPY_I(obj_rw->last_pos.z    , obj->last_pos.z);
	COPY_B(obj_rw->contains_type , obj->contains_type);
	COPY_B(obj_rw->contains_id   , obj->contains_id);
	COPY_B(obj_rw->contains_count, obj->contains_count);
	COPY_B(obj_rw->matcen_creator, obj->matcen_creator);
	COPY_I(obj_rw->lifeleft      , obj->lifeleft);

	switch (obj->movement_type)
	{
		case MT_PHYSICS:
			COPY_I(obj_rw->mtype.phys_info.velocity.x  , obj->mtype.phys_info.velocity.x);
			COPY_I(obj_rw->mtype.phys_info.velocity.y  , obj->mtype.phys_info.velocity.y);
			COPY_I(obj_rw->mtype.phys_info.velocity.z  , obj->mtype.phys_info.velocity.z);
			COPY_I(obj_rw->mtype.phys_info.thrust.x    , obj->mtype.phys_info.thrust.x);
			COPY_I(obj_rw->mtype.phys_info.thrust.y    , obj->mtype.phys_info.thrust.y);
			COPY_I(obj_rw->mtype.phys_info.thrust.z    , obj->mtype.phys_info.thrust.z);
			COPY_I(obj_rw->mtype.phys_info.mass        , obj->mtype.phys_info.mass);
			COPY_I(obj_rw->mtype.phys_info.drag        , obj->mtype.phys_info.drag);
			COPY_I(obj_rw->mtype.phys_info.brakes      , obj->mtype.phys_info.brakes);
			COPY_I(obj_rw->mtype.phys_info.rotvel.x    , obj->mtype.phys_info.rotvel.x);
			COPY_I(obj_rw->mtype.phys_info.rotvel.y    , obj->mtype.phys_info.rotvel.y);
			COPY_I(obj_rw->mtype.phys_info.rotvel.z    , obj->mtype.phys_info.rotvel.z);
			COPY_I(obj_rw->mtype.phys_info.rotthrust.x , obj->mtype.phys_info.rotthrust.x);
			COPY_I(obj_rw->mtype.phys_info.rotthrust.y , obj->mtype.phys_info.rotthrust.y);
			COPY_I(obj_rw->mtype.phys_info.rotthrust.z , obj->mtype.phys_info.rotthrust.z);
			COPY_S(obj_rw->mtype.phys_info.turnroll    , obj->mtype.phys_info.turnroll);
			COPY_S(obj_rw->mtype.phys_info.flags       , obj->mtype.phys_info.flags);
			break;

		case MT_SPINNING:
			COPY_I(obj_rw->mtype.spin_rate.x , obj->mtype.spin_rate.x);
			COPY_I(obj_rw->mtype.spin_rate.y , obj->mtype.spin_rate.y);
			COPY_I(obj_rw->mtype.spin_rate.z , obj->mtype.spin_rate.z);
			break;
	}

	switch (obj->control_type)
	{
		case CT_WEAPON:
			COPY_S(obj_rw->ctype.laser_info.parent_type      , obj->ctype.laser_info.parent_type);
			COPY_S(obj_rw->ctype.laser_info.parent_num       , obj->ctype.laser_info.parent_num);
			COPY_I(obj_rw->ctype.laser_info.parent_signature , obj->ctype.laser_info.parent_signature);
			COPY_I(obj_rw->ctype.laser_info.creation_time    , obj->ctype.laser_info.creation_time);
			COPY_S(obj_rw->ctype.laser_info.last_hitobj      , obj->ctype.laser_info.last_hitobj);
			COPY_S(obj_rw->ctype.laser_info.track_goal       , obj->ctype.laser_info.track_goal);
			COPY_I(obj_rw->ctype.laser_info.multiplier       , obj->ctype.laser_info.multiplier);
			break;

		case CT_EXPLOSION:
			COPY_I(obj_rw->ctype.expl_info.spawn_time    , obj->ctype.expl_info.spawn_time);
			COPY_I(obj_rw->ctype.expl_info.delete_time   , obj->ctype.expl_info.delete_time);
			COPY_S(obj_rw->ctype.expl_info.delete_objnum , obj->ctype.expl_info.delete_objnum);
			COPY_S(obj_rw->ctype.expl_info.attach_parent , obj->ctype.expl_info.attach_parent);
			COPY_S(obj_rw->ctype.expl_info.prev_attach   , obj->ctype.expl_info.prev_attach);
			COPY_S(obj_rw->ctype.expl_info.next_attach   , obj->ctype.expl_info.next_attach);
			break;

		case CT_AI:
		{
			int i;
			COPY_B(obj_rw->ctype.ai_info.behavior               , obj->ctype.ai_info.behavior);
			for (i = 0; i < MAX_AI_FLAGS; i++)
				COPY_B(obj_rw->ctype.ai_info.flags[i]       , obj->ctype.ai_info.flags[i]);
			COPY_S(obj_rw->ctype.ai_info.hide_segment           , obj->ctype.ai_info.hide_segment);
			COPY_S(obj_rw->ctype.ai_info.hide_index             , obj->ctype.ai_info.hide_index);
			COPY_S(obj_rw->ctype.ai_info.path_length            , obj->ctype.ai_info.path_length);
			COPY_B(obj_rw->ctype.ai_info.cur_path_index         , obj->ctype.ai_info.cur_path_index);
			COPY_B(obj_rw->ctype.ai_info.dying_sound_playing    , obj->ctype.ai_info.dying_sound_playing);
			COPY_S(obj_rw->ctype.ai_info.danger_laser_num       , obj->ctype.ai_info.danger_laser_num);
			COPY_I(obj_rw->ctype.ai_info.danger_laser_signature , obj->ctype.ai_info.danger_laser_signature);
			COPY_I(obj_rw->ctype.ai_info.dying_start_time       , obj->ctype.ai_info.dying_start_time);
			break;
		}

		case CT_LIGHT:
			COPY_I(obj_rw->ctype.light_info.intensity , obj->ctype.light_info.intensity);
			break;

		case CT_POWERUP:
			COPY_I(obj_rw->ctype.powerup_info.count         , obj->ctype.powerup_info.count);
			COPY_I(obj_rw->ctype.powerup_info.creation_time , obj->ctype.powerup_info.creation_time);
			COPY_I(obj_rw->ctype.powerup_info.flags         , obj->ctype.powerup_info.flags);
			break;
	}

	switch (obj->render_type)
	{
		case RT_MORPH:
		case RT_POLYOBJ:
		case RT_NONE: // HACK below
		{
			int i;
			if (obj->render_type == RT_NONE && obj->type != OBJ_GHOST) // HACK: when a player is dead or not connected yet, clients still expect to get polyobj data - even if render_type == RT_NONE at this time.
				break;
			COPY_I(obj_rw->rtype.pobj_info.model_num                , obj->rtype.pobj_info.model_num);
			for (i=0;i<MAX_SUBMODELS;i++)
			{
				COPY_S(obj_rw->rtype.pobj_info.anim_angles[i].p , obj->rtype.pobj_info.anim_angles[i].p);
				COPY_S(obj_rw->rtype.pobj_info.anim_angles[i].b , obj->rtype.pobj_info.anim_angles[i].b);
				COPY_S(obj_rw->rtype.pobj_info.anim_angles[i].h , obj->rtype.pobj_info.anim_angles[i].h);
			}
			COPY_I(obj_rw->rtype.pobj_info.subobj_flags             , obj->rtype.pobj_info.subobj_flags);
			COPY_I(obj_rw->rtype.pobj_info.tmap_override            , obj->rtype.pobj_info.tmap_override);
			COPY_I(obj_rw->rtype.pobj_info.alt_textures             , obj->rtype.pobj_info.alt_textures);
			break;
		}

		case RT_WEAPON_VCLIP:
		case RT_HOSTAGE:
		case RT_POWERUP:
		case RT_FIREBALL:
			COPY_I(obj_rw->rtype.vclip_info.vclip_num , obj->rtype.vclip_info.vclip_num);
			COPY_I(obj_rw->rtype.vclip_info.frametime , obj->rtype.vclip_info.frametime);
			COPY_B(obj_rw->rtype.vclip_info.framenum  , obj->rtype.vclip_info.framenum);
			break;

		case RT_LASER:
			break;

	}
}

#undef COPY_S
#undef COPY_I
#define COPY_S(a, b) memcpy(&a, &b, sizeof(a))
#define COPY_I(a, b) memcpy(&a, &b, sizeof(a))

void object_rw_to_object(object_rw *obj_rw, object *obj)
{
	COPY_I(obj->signature     , obj_rw->signature);
	COPY_B(obj->type          , obj_rw->type);
	COPY_B(obj->id            , obj_rw->id);
	COPY_S(obj->next          , obj_rw->next);
	COPY_S(obj->prev          , obj_rw->prev);
	COPY_B(obj->control_type  , obj_rw->control_type);
	COPY_B(obj->movement_type , obj_rw->movement_type);
	COPY_B(obj->render_type   , obj_rw->render_type);
	COPY_B(obj->flags         , obj_rw->flags);
	COPY_S(obj->segnum        , obj_rw->segnum);
	COPY_S(obj->attached_obj  , obj_rw->attached_obj);
	COPY_I(obj->pos.x         , obj_rw->pos.x);
	COPY_I(obj->pos.y         , obj_rw->pos.y);
	COPY_I(obj->pos.z         , obj_rw->pos.z);
	COPY_I(obj->orient.rvec.x , obj_rw->orient.rvec.x);
	COPY_I(obj->orient.rvec.y , obj_rw->orient.rvec.y);
	COPY_I(obj->orient.rvec.z , obj_rw->orient.rvec.z);
	COPY_I(obj->orient.fvec.x , obj_rw->orient.fvec.x);
	COPY_I(obj->orient.fvec.y , obj_rw->orient.fvec.y);
	COPY_I(obj->orient.fvec.z , obj_rw->orient.fvec.z);
	COPY_I(obj->orient.uvec.x , obj_rw->orient.uvec.x);
	COPY_I(obj->orient.uvec.y , obj_rw->orient.uvec.y);
	COPY_I(obj->orient.uvec.z , obj_rw->orient.uvec.z);
	COPY_I(obj->size          , obj_rw->size);
	COPY_I(obj->shields       , obj_rw->shields);
	COPY_I(obj->last_pos.x    , obj_rw->last_pos.x);
	COPY_I(obj->last_pos.y    , obj_rw->last_pos.y);
	COPY_I(obj->last_pos.z    , obj_rw->last_pos.z);
	COPY_B(obj->contains_type , obj_rw->contains_type);
	COPY_B(obj->contains_id   , obj_rw->contains_id);
	COPY_B(obj->contains_count, obj_rw->contains_count);
	COPY_B(obj->matcen_creator, obj_rw->matcen_creator);
	COPY_I(obj->lifeleft      , obj_rw->lifeleft);

	switch (obj->movement_type)
	{
		case MT_PHYSICS:
			COPY_I(obj->mtype.phys_info.velocity.x  , obj_rw->mtype.phys_info.velocity.x);
			COPY_I(obj->mtype.phys_info.velocity.y  , obj_rw->mtype.phys_info.velocity.y);
			COPY_I(obj->mtype.phys_info.velocity.z  , obj_rw->mtype.phys_info.velocity.z);
			COPY_I(obj->mtype.phys_info.thrust.x    , obj_rw->mtype.phys_info.thrust.x);
			COPY_I(obj->mtype.phys_info.thrust.y    , obj_rw->mtype.phys_info.thrust.y);
			COPY_I(obj->mtype.phys_info.thrust.z    , obj_rw->mtype.phys_info.thrust.z);
			COPY_I(obj->mtype.phys_info.mass        , obj_rw->mtype.phys_info.mass);
			COPY_I(obj->mtype.phys_info.drag        , obj_rw->mtype.phys_info.drag);
			COPY_I(obj->mtype.phys_info.brakes      , obj_rw->mtype.phys_info.brakes);
			COPY_I(obj->mtype.phys_info.rotvel.x    , obj_rw->mtype.phys_info.rotvel.x);
			COPY_I(obj->mtype.phys_info.rotvel.y    , obj_rw->mtype.phys_info.rotvel.y);
			COPY_I(obj->mtype.phys_info.rotvel.z    , obj_rw->mtype.phys_info.rotvel.z);
			COPY_I(obj->mtype.phys_info.rotthrust.x , obj_rw->mtype.phys_info.rotthrust.x);
			COPY_I(obj->mtype.phys_info.rotthrust.y , obj_rw->mtype.phys_info.rotthrust.y);
			COPY_I(obj->mtype.phys_info.rotthrust.z , obj_rw->mtype.phys_info.rotthrust.z);
			COPY_S(obj->mtype.phys_info.turnroll    , obj_rw->mtype.phys_info.turnroll);
			COPY_S(obj->mtype.phys_info.flags       , obj_rw->mtype.phys_info.flags);
			break;

		case MT_SPINNING:
			COPY_I(obj->mtype.spin_rate.x , obj_rw->mtype.spin_rate.x);
			COPY_I(obj->mtype.spin_rate.y , obj_rw->mtype.spin_rate.y);
			COPY_I(obj->mtype.spin_rate.z , obj_rw->mtype.spin_rate.z);
			break;
	}

	switch (obj->control_type)
	{
		case CT_WEAPON:
			COPY_S(obj->ctype.laser_info.parent_type      , obj_rw->ctype.laser_info.parent_type);
			COPY_S(obj->ctype.laser_info.parent_num       , obj_rw->ctype.laser_info.parent_num);
			COPY_I(obj->ctype.laser_info.parent_signature , obj_rw->ctype.laser_info.parent_signature);
			COPY_I(obj->ctype.laser_info.creation_time    , obj_rw->ctype.laser_info.creation_time);
			COPY_S(obj->ctype.laser_info.last_hitobj      , obj_rw->ctype.laser_info.last_hitobj);
			COPY_S(obj->ctype.laser_info.track_goal       , obj_rw->ctype.laser_info.track_goal);
			COPY_I(obj->ctype.laser_info.multiplier       , obj_rw->ctype.laser_info.multiplier);
			break;

		case CT_EXPLOSION:
			COPY_I(obj->ctype.expl_info.spawn_time    , obj_rw->ctype.expl_info.spawn_time);
			COPY_I(obj->ctype.expl_info.delete_time   , obj_rw->ctype.expl_info.delete_time);
			COPY_S(obj->ctype.expl_info.delete_objnum , obj_rw->ctype.expl_info.delete_objnum);
			COPY_S(obj->ctype.expl_info.attach_parent , obj_rw->ctype.expl_info.attach_parent);
			COPY_S(obj->ctype.expl_info.prev_attach   , obj_rw->ctype.expl_info.prev_attach);
			COPY_S(obj->ctype.expl_info.next_attach   , obj_rw->ctype.expl_info.next_attach);
			break;

		case CT_AI:
		{
			int i;
			COPY_B(obj->ctype.ai_info.behavior               , obj_rw->ctype.ai_info.behavior);
			for (i = 0; i < MAX_AI_FLAGS; i++)
				COPY_B(obj->ctype.ai_info.flags[i]       , obj_rw->ctype.ai_info.flags[i]);
			COPY_S(obj->ctype.ai_info.hide_segment           , obj_rw->ctype.ai_info.hide_segment);
			COPY_S(obj->ctype.ai_info.hide_index             , obj_rw->ctype.ai_info.hide_index);
			COPY_S(obj->ctype.ai_info.path_length            , obj_rw->ctype.ai_info.path_length);
			COPY_B(obj->ctype.ai_info.cur_path_index         , obj_rw->ctype.ai_info.cur_path_index);
			COPY_B(obj->ctype.ai_info.dying_sound_playing    , obj_rw->ctype.ai_info.dying_sound_playing);
			COPY_S(obj->ctype.ai_info.danger_laser_num       , obj_rw->ctype.ai_info.danger_laser_num);
			COPY_I(obj->ctype.ai_info.danger_laser_signature , obj_rw->ctype.ai_info.danger_laser_signature);
			COPY_I(obj->ctype.ai_info.dying_start_time       , obj_rw->ctype.ai_info.dying_start_time);
			break;
		}

		case CT_LIGHT:
			COPY_I(obj->ctype.light_info.intensity , obj_rw->ctype.light_info.intensity);
			break;

		case CT_POWERUP:
			COPY_I(obj->ctype.powerup_info.count         , obj_rw->ctype.powerup_info.count);
			COPY_I(obj->ctype.powerup_info.creation_time , obj_rw->ctype.powerup_info.creation_time);
			COPY_I(obj->ctype.powerup_info.flags         , obj_rw->ctype.powerup_info.flags);
			break;
	}

	switch (obj->render_type)
	{
		case RT_MORPH:
		case RT_POLYOBJ:
		case RT_NONE: // HACK below
		{
			int i;
			if (obj->render_type == RT_NONE && obj->type != OBJ_GHOST) // HACK: when a player is dead or not connected yet, clients still expect to get polyobj data - even if render_type == RT_NONE at this time.
				break;
			COPY_I(obj->rtype.pobj_info.model_num                , obj_rw->rtype.pobj_info.model_num);
			for (i=0;i<MAX_SUBMODELS;i++)
			{
				COPY_S(obj->rtype.pobj_info.anim_angles[i].p , obj_rw->rtype.pobj_info.anim_angles[i].p);
				COPY_S(obj->rtype.pobj_info.anim_angles[i].b , obj_rw->rtype.pobj_info.anim_angles[i].b);
				COPY_S(obj->rtype.pobj_info.anim_angles[i].h , obj_rw->rtype.pobj_info.anim_angles[i].h);
			}
			COPY_I(obj->rtype.pobj_info.subobj_flags             , obj_rw->rtype.pobj_info.subobj_flags);
			COPY_I(obj->rtype.pobj_info.tmap_override            , obj_rw->rtype.pobj_info.tmap_override);
			COPY_I(obj->rtype.pobj_info.alt_textures             , obj_rw->rtype.pobj_info.alt_textures);
			break;
		}

		case RT_WEAPON_VCLIP:
		case RT_HOSTAGE:
		case RT_POWERUP:
		case RT_FIREBALL:
			COPY_I(obj->rtype.vclip_info.vclip_num , obj_rw->rtype.vclip_info.vclip_num);
			COPY_I(obj->rtype.vclip_info.frametime , obj_rw->rtype.vclip_info.frametime);
			COPY_B(obj->rtype.vclip_info.framenum  , obj_rw->rtype.vclip_info.framenum);
			break;

		case RT_LASER:
			break;

	}
}
