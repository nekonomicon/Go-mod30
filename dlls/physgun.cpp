/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#include "extdll.h" 
#include "util.h" 
#include "cbase.h" 
#include "monsters.h" 
#include "weapons.h" 
#include "nodes.h" 
#include "player.h" 
#include "soundent.h" 
#include "shake.h" 
#include "gamerules.h"
#include "vector.h"


//****************************
//
//		Barnacle Tongue
//
//****************************

class CGrappleHook : public CBaseEntity
{ 
public: 

	void Spawn( void ); 
	void Precache( void ); 
	void EXPORT Move( void ); 
	void EXPORT Hit( CBaseEntity* ); 
	void Killed(entvars_t *pev, int gib); //Removes grapple
	static CGrappleHook* Create( Vector, Vector, CBasePlayer*);
	int m_Chain; 
	int m_iIsMoving; 
	int m_iTrail_Length;
	int colorR;
	int colorG;
	int colorB;
	CBasePlayer *myowner;

	unsigned short m_usTongue;
};

LINK_ENTITY_TO_CLASS( proj_hook, CGrappleHook );


void CGrappleHook :: Spawn( void ) 
{ 
	Precache( ); 

	SET_MODEL( ENT(pev), "models/v_physgun.mdl" );
	pev->movetype = MOVETYPE_FLY; 
	pev->solid = SOLID_BBOX; 
	pev->rendermode = kRenderNormal;
	pev->renderamt = 0;
	pev->effects = EF_NODRAW;

	UTIL_SetSize( pev, Vector(0,0,0), Vector(0,0,0) ); 
	UTIL_SetOrigin( pev, pev->origin ); 

	pev->classname = MAKE_STRING( "proj_hook" );

	SetThink( Move ); 
	SetTouch( Hit );

	UTIL_MakeVectors(pev->angles);
	pev->velocity = gpGlobals->v_forward * 700; 
	pev->gravity = 0; 
	pev->nextthink = gpGlobals->time + 0.01; 
	pev->dmg = 0;
}

void CGrappleHook :: Precache( void ) 
{ 
	m_Chain = PRECACHE_MODEL( "sprites/laserbeam.spr" ); 
	PRECACHE_MODEL( "models/v_physgun.mdl" ); 

	//PRECACHE_SOUND("weapons/bgrapple_impact.wav");	//hit tongue
}

void CGrappleHook :: Hit( CBaseEntity* Target ) 
{ 
	TraceResult TResult; 
	Vector StartPosition;

//	EMIT_SOUND(ENT(pev), CHAN_WEAPON, "weapons/bgrapple_impact.wav", 1, ATTN_NORM);

	Vector delta = Vector( 8, 8, 0 );
	Vector mins = pev->origin - delta;
	Vector maxs = pev->origin + delta;
	maxs.z = pev->origin.z;

#ifndef CLIENT_DLL

		colorR = 200;
		colorG = 200;
		colorB = 255;

	CBaseEntity *pList[1];

		Vector vecAiming = myowner->GetAutoaimVector( AUTOAIM_5DEGREES );
	int count = UTIL_EntitiesInBox( pList, 1, mins, maxs, (FL_CLIENT|FL_MONSTER) );

	if ( count )
	{
		pev->velocity = pev->velocity.Normalize( );
		myowner->m_afPhysicsFlags |= PFLAG_ON_GRAPPLE; //Set physics flag to "on grapple"
		myowner->pev->movetype = MOVETYPE_BOUNCE; //Remove gravity effect on player
	

	}
	else
	{

	TraceResult tr;
	float rgfl1[3];
	float rgfl2[3];
	const char *pTextureName;
	Vector vecSrc = pev->origin;
	Vector vecEnd = vecSrc + gpGlobals->v_forward * 160;

	UTIL_MakeVectors (pev->v_angle);
	UTIL_TraceLine( vecSrc, vecEnd, dont_ignore_monsters, ENT( pev ), &tr );

	CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);
		vecSrc.CopyToArray(rgfl1);
		vecEnd.CopyToArray(rgfl2);
	

		if (pEntity)
			pTextureName = TRACE_TEXTURE( ENT(pEntity->pev), rgfl1, rgfl2 );
		else
			pTextureName = TRACE_TEXTURE( ENT(0), rgfl1, rgfl2 );

		//if (pTextureName && myowner && (strnicmp (pTextureName, "xeno_", 5) == 0))
		//{
			pev->velocity = pev->velocity.Normalize( );
			myowner->m_afPhysicsFlags |= PFLAG_ON_GRAPPLE; //Set physics flag to "on grapple"
			myowner->pev->movetype = MOVETYPE_BOUNCE; //Remove gravity effect on player
		
	//	}
		//	else
		//	Killed(pev, 0);
	}
#endif

}

void CGrappleHook :: Killed(entvars_t *pev, int gib)
{ 
		myowner->pev->movetype = MOVETYPE_WALK; //Re-apply gravity
		myowner->m_afPhysicsFlags &= ~PFLAG_ON_GRAPPLE; //Remove "on grapple" flag
		myowner->m_iGrappleExists = 0;
		colorR = 0;
		colorG = 0;
		colorB = 255;
		SetThink (NULL);
		SetTouch (NULL);

		SUB_Remove( ); 
}

CGrappleHook* CGrappleHook :: Create( Vector Pos, Vector Aim, CBasePlayer* Owner ) 
{ 
	CGrappleHook* Hook = GetClassPtr( (CGrappleHook*)NULL ); 
	UTIL_SetOrigin( Hook->pev, Pos ); 
	Hook->pev->angles = Aim; 
	Hook->Spawn( ); 
	Hook->SetTouch( CGrappleHook :: Hit ); 
	Hook->pev->owner = Owner->edict( );
	Hook->myowner = Owner;
	Hook->colorR = 0;
	Hook->colorG = 0;
	Hook->colorB = 255;
	return Hook; 
}

void CGrappleHook :: Move( void ) 
{ 
	if( !myowner->IsAlive( ) ) //if owner is dead is
	{
		Killed(pev, 0); //Remove grapple
		return;
	}


	if(!(myowner->pev->button & (IN_ATTACK|IN_ATTACK2) )) //If owner is not pushing attack buttons
	{
		Killed(pev, 0); //Remove grapple
	return;
	}

/*	if( myowner->m_afPhysicsFlags & PFLAG_ON_GRAPPLE) //If we are on a grapple
	{
	myowner->pev->velocity * 10;
	}
*/
	
	MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY ); //Draw 'chain'
		WRITE_BYTE( TE_BEAMENTPOINT ); 
		WRITE_SHORT( myowner->entindex() + 0x1000 );
		WRITE_COORD( pev->origin.x );
		WRITE_COORD( pev->origin.y );
		WRITE_COORD( pev->origin.z );
		WRITE_SHORT( m_Chain ); 
		WRITE_BYTE( 1 );	//start frame
		WRITE_BYTE( 0 );	//rate
		WRITE_BYTE( 1 );	//life
		WRITE_BYTE( 5 );	//width 
		WRITE_BYTE( 0);		//noise
		WRITE_BYTE( colorR );	//r
		WRITE_BYTE( colorG );	//g
		WRITE_BYTE( colorB );	//b
		WRITE_BYTE( 255 );	//brightnes
		WRITE_BYTE( 0);		//scrollspeed
	MESSAGE_END( );
		

	pev->nextthink = gpGlobals->time + 0.01;
}


enum physgun_e
{

	PHYSGUN_IDLE,
	PHYSGUN_SHOOT,

	
};



LINK_ENTITY_TO_CLASS( weapon_physgun, CPHYSGUN);

BOOL CPHYSGUN::IsUseable( void )
{
	return TRUE;
}

void CPHYSGUN::Spawn( )
{
	pev->classname = MAKE_STRING("weapon_physgun"); // hack to allow for old names
	Precache( );
	SET_MODEL(ENT(pev), "models/w_physgun.mdl");
	m_iId = WEAPON_PHYSGUN;

	m_iDefaultAmmo = 1;

	FallInit();// get ready to fall down.
}



void CPHYSGUN::Precache( void )
{
	PRECACHE_MODEL("models/v_physgun.mdl");
	PRECACHE_MODEL("models/w_physgun.mdl");
	PRECACHE_MODEL("models/p_physgun.mdl");

	m_iShell = PRECACHE_MODEL ("models/shell.mdl");// brass shellTE_MODEL

	PRECACHE_MODEL("models/grenade.mdl");	// grenade

	PRECACHE_MODEL("models/w_9mmARclip.mdl");
	PRECACHE_SOUND("items/9mmclip1.wav");              

	PRECACHE_SOUND("items/clipinsert1.wav");
	PRECACHE_SOUND("items/cliprelease1.wav");

	PRECACHE_SOUND ("weapons/tg_shot1.wav");// H to the K
	PRECACHE_SOUND ("weapons/tg_shot2.wav");// H to the K
	PRECACHE_SOUND ("weapons/hks3.wav");// H to the K

	PRECACHE_SOUND( "weapons/glauncher.wav" );
	PRECACHE_SOUND( "weapons/glauncher2.wav" );

	PRECACHE_SOUND ("weapons/357_cock1.wav");

	m_physgun = PRECACHE_EVENT( 1, "events/physgun.sc" );

	UTIL_PrecacheOther("proj_hook");

	m_usTongue = PRECACHE_EVENT ( 1, "events/grapple.sc" );
	m_usTongue2 = PRECACHE_EVENT ( 1, "events/grapple.sc" );

}

int CPHYSGUN::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "9mm";
	p->iMaxAmmo1 = 1;
	p->pszAmmo2 = "NULL";
	p->iMaxAmmo2 = -1;
	p->iMaxClip = 1;
	p->iSlot = 0;
	p->iPosition = 4;
	p->iFlags = 2;
	p->iId = m_iId = WEAPON_PHYSGUN;
	p->iWeight = 15;

	return 1;
}

int CPHYSGUN::AddToPlayer( CBasePlayer *pPlayer )
{
	if ( CBasePlayerWeapon::AddToPlayer( pPlayer ) )
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev );
			WRITE_BYTE( m_iId );
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}

BOOL CPHYSGUN::Deploy( )
{
	return DefaultDeploy( "models/v_physgun.mdl", "models/p_physgun.mdl", PHYSGUN_IDLE, "physgun" );
}


void CPHYSGUN::PrimaryAttack()
{

	if( m_pPlayer->m_iGrappleExists ) //if player already has a grapple
	{
		return;
	}

	if(m_flNextPrimaryAttack > gpGlobals->time)
	{
		return;
	}

	int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif


	//SendWeaponAnim( GRAPPLE_FIREWAITING, 1 );
//	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/bgrapple_fire.wav", 1, ATTN_NORM);
	m_pPlayer->m_iGrappleExists = 1;
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 ); 

	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle ); 
	Vector AimingDir = gpGlobals->v_forward; 
	Vector GunPosition = m_pPlayer->GetGunPosition( ); 

#ifndef CLIENT_DLL
	GunPosition = GunPosition + gpGlobals->v_up * 4 + gpGlobals->v_right * 3 + gpGlobals->v_forward * 16;
	//if( !m_pPlayer->m_iGrappleExists )
		m_pPlayer->m_MyGrapple = CGrappleHook :: Create( GunPosition, m_pPlayer->pev->v_angle, m_pPlayer );
#endif


//	EMIT_SOUND( ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/bgrapple_pull.wav", 1, ATTN_NORM);
//		ALERT( at_console, "Think launched!!!\n" );

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.1;


	SetThink(FlyThink);
	pev->nextthink = gpGlobals->time + 0.1;
}




void CPHYSGUN::SecondaryAttack()
{

	m_pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;

	//m_iClip--;


	m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;

	// player "shoot" animation
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	Vector vecSrc	 = m_pPlayer->GetGunPosition( );
//	Vector vecAiming = m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );
	Vector vecDir;
	Vector vecAiming;
	

	vecAiming = gpGlobals->v_forward;
#ifdef CLIENT_DLL
	if ( !bIsMultiplayer() )
#else
	if ( !g_pGameRules->IsMultiplayer() )
#endif
	{
		// optimized multiplayer. Widened to make it easier to hit a moving player
		
		
		vecDir = m_pPlayer->FireBulletsPhysGun( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_NONE, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
	}
	else
	{
		// single player spread

		vecDir = m_pPlayer->FireBulletsPhysGun( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_NONE, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
	}

  int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif
	
	//pev->rendermode = kRenderGlow;
	//pev->renderamt = 192;
	//pev->renderfx = kRenderFxExplode;

	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_physgun, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, 0, 0, 0, 0 );

	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);


//	m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.0;

//	if ( m_flNextSecondaryAttack < UTIL_WeaponTimeBase() )
//		m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.0;

//	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
	

	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle ); 
	Vector AimingDir = gpGlobals->v_forward; 
	Vector GunPosition = m_pPlayer->GetGunPosition( );
}



void CPHYSGUN :: FlyThink( void ) 
{
	
	if( m_pPlayer->m_afPhysicsFlags & PFLAG_ON_GRAPPLE ) //If we are on a grapple
	{
//		ALERT( at_console, "Splat, flying!!!\n" );

//	SendWeaponAnim( GRAPPLE_FIRETRAVEL, 1 );
		
		SetThink(PullThink);
	}

	if ( !m_pPlayer->m_iGrappleExists )
	{
		//SendWeaponAnim( GRAPPLE_FIRERELEASE, 1 );
		//EMIT_SOUND( ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/bgrapple_release.wav", 1, ATTN_NORM);

		m_flNextSecondaryAttack = m_flNextSecondaryAttack + 0.1;

		if (m_flNextSecondaryAttack < UTIL_WeaponTimeBase() )
		{
			m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.25;
		}

		SetThink (WeaponIdle);
		pev->nextthink = gpGlobals->time + 0.1;
		m_flTimeWeaponIdle = gpGlobals->time + 0.1;
	}

	pev->nextthink = gpGlobals->time + 0.1;
}

void CPHYSGUN :: PullThink( void ) 
{
	if( m_pPlayer->m_afPhysicsFlags & PFLAG_ON_GRAPPLE ) //If we are on a grapple
	{
	TraceResult tr;

	UTIL_MakeVectors (m_pPlayer->pev->v_angle);
	Vector vecSrc = m_pPlayer->GetGunPosition();
	Vector vecEnd = vecSrc + gpGlobals->v_forward * 9999;

#ifndef CLIENT_DLL
	UTIL_TraceLine( vecSrc, vecEnd, dont_ignore_monsters, ENT( m_pPlayer->pev ), &tr );

	//gSkillData.plrDmgGrapple = 20

	ClearMultiDamage();
	CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);
	//pEntity->TraceAttack(m_pPlayer->pev, 20, gpGlobals->v_forward, &tr, DMG_CLUB || DMG_ALWAYSGIB ); 
	ApplyMultiDamage( m_pPlayer->pev, m_pPlayer->pev );
#endif
//		pev->nextthink = gpGlobals->time + 0.1;
	}
	else 
	{

//	SendWeaponAnim( GRAPPLE_FIRERELEASE, 1 );
//	EMIT_SOUND( ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/bgrapple_release.wav", 1, ATTN_NORM);

	m_flNextPrimaryAttack = m_flNextPrimaryAttack + 0.1;

//	if (m_flNextPrimaryAttack < UTIL_WeaponTimeBase() )
//	{
	//	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 1;
//	}

	SetThink (WeaponIdle);
//	pev->nextthink = gpGlobals->time + 0.1;
//	m_flTimeWeaponIdle = gpGlobals->time + 0.1;
	}
}


void CPHYSGUN::WeaponIdle( void )
{

	ResetEmptySound( );

//	m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );

	if ( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	int iAnim;
	switch ( RANDOM_LONG( 0, 1 ) )
	{
	case 0:	
		iAnim = PHYSGUN_IDLE;	
		break;
	
	default:
	case 1:
		iAnim = PHYSGUN_IDLE;
		break;
	}

	SendWeaponAnim( iAnim );
	
	m_flTimeWeaponIdle = UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 ); // how long till we do this again.
}







