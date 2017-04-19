/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
//=========================================================
// Zombie
//=========================================================

// UNDONE: Don't flinch every time you get hit

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"schedule.h"
#include	"player.h"
#include	"weapons.h"
extern CBasePlayer *camera_owner;


class CCamera : public CBaseMonster
{
public:
	void Spawn( void );
	void Precache( void );
	int  Classify ( void );
	int IgnoreConditions ( void );
	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	int TakeDamage( entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType );

	static	TYPEDESCRIPTION m_SaveData[];
	
	

	int player_use;
	EHANDLE m_hPlayer;

	float m_flNextFlinch;



};

LINK_ENTITY_TO_CLASS( monster_camera, CCamera );


TYPEDESCRIPTION	CCamera::m_SaveData[] = 
{
	DEFINE_FIELD( CCamera, m_hPlayer, FIELD_EHANDLE ),

};

IMPLEMENT_SAVERESTORE(CCamera,CBaseMonster);
//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
int	CCamera :: Classify ( void )
{
	return	CLASS_NONE;
}




int CCamera :: TakeDamage( entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType )
{

	return CBaseMonster::TakeDamage( pevInflictor, pevAttacker, flDamage, bitsDamageType );
}

//=========================================================
// Spawn
//=========================================================
void CCamera :: Spawn()
{
	Precache( );

	SET_MODEL(ENT(pev), "models/gomod/camera.mdl");
	UTIL_SetSize( pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX-15 );

	pev->solid			= SOLID_BBOX;
	pev->movetype		= MOVETYPE_NOCLIP;
//	m_bloodColor		= BLOOD_COLOR_GREEN;
	pev->health			= 999999;
	m_MonsterState		= MONSTERSTATE_NONE;

	pev->takedamage		= DAMAGE_NO;
	pev->flags			|= FL_MONSTER;

	m_hPlayer			= camera_owner;
	MonsterInit();



	pev->origin.z += 25;
	pev->origin.y -= 25;
	player_use = 0;

	pev->nextthink = gpGlobals->time + 0.1;

}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CCamera :: Precache()
{
	
}	

//=========================================================
// AI Schedules Specific to this monster
//=========================================================



int CCamera::IgnoreConditions ( void )
{
	

	int iIgnore = CBaseMonster::IgnoreConditions();
	

	if(m_hPlayer->pev->button & (IN_CAMERA))
	{
		if(player_use == 0)
		{
			player_use = 1;
			pev->angles.x *= -1;
			SET_VIEW( m_hPlayer->edict(), edict() );
		}
		else
		{
			player_use = 0;
			pev->angles.x *= -1;
			SET_VIEW( m_hPlayer->edict(), m_hPlayer->edict() );
		}
	}

	if(m_hPlayer->pev->health <= 0)
	{
		SET_VIEW( m_hPlayer->edict(), m_hPlayer->edict() );
	}
		
	
	
	if(player_use == 1)
	{
	//	SET_VIEW( m_hPlayer->edict(), edict() );
		pev->renderamt = 0;								// The engine won't draw this model if this is set to 0 and blending is on
		pev->rendermode = kRenderTransTexture;
	}
	else
	{
	//	SET_VIEW( m_hPlayer->edict(), m_hPlayer->edict() );
		pev->renderamt = 255;								// The engine won't draw this model if this is set to 0 and blending is on
		pev->rendermode = kRenderNormal;
	}

	
	pev->nextthink = gpGlobals->time + 0.1;
	return iIgnore;
	
}