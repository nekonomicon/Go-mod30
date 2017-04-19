//***************************************************
//	Opposing Forces Weapon Barnacle.
//
//			Edited by Jack-Hang
//		Based on code Grapping hook,
//	by Phantasiere (nrich@iinet.net.au)
//
//		Separate thanks for BUzer because 
//  without his councils, the code would be a 
//		 nightmare of the programmer.
//
//FGD:
//weapon_grapple
//
//NOTE:
//		1) grapple monsters
//		2) grapple textures (xeno_...)
//
//***************************************************

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

#define WEAPON_GRAPPLE 18
#define GRAPPLE_WEIGHT 25
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
	CBasePlayer *myowner;

	unsigned short m_usTongue;
};
/*
class CGrapple : public CBasePlayerWeapon
{
public:

	void Spawn( void );
	void Precache( void );
	int iItemSlot( ) { return 3; }
	int GetItemInfo(ItemInfo *p);
	int AddToPlayer( CBasePlayer *pPlayer );
	void Holster( int skiplocal = 0 );

	void PrimaryAttack( void );
	BOOL Deploy( );
	void WeaponIdle( void );
	
	BOOL IsUseable( void );
	void FlyThink( void );
	void PullThink( void );

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usTongue;
	unsigned short m_usTongue2;
};
*/

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
	pev->nextthink = gpGlobals->time + 0.13; 
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

	CBaseEntity *pList[1];
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
	Vector vecEnd = vecSrc + gpGlobals->v_forward * 8;

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
		WRITE_BYTE( 15 );	//width 
		WRITE_BYTE( 0);		//noise
		WRITE_BYTE( 255 );	//r
		WRITE_BYTE( 255 );	//g
		WRITE_BYTE( 255 );	//b
		WRITE_BYTE( 255 );	//brightnes
		WRITE_BYTE( 0);		//scrollspeed
		MESSAGE_END( );

	pev->nextthink = gpGlobals->time + 0.1;
}



//****************************
//
//		Barnacle Weapon
//
//****************************
enum grapple_e {
	GRAPPLE_BREATHE = 0,
	GRAPPLE_LONGIDLE,
	GRAPPLE_SHORTIDLE,
	GRAPPLE_COUGH,
	GRAPPLE_DOWN,
	GRAPPLE_UP,
	GRAPPLE_FIRE,
	GRAPPLE_FIREWAITING,
	GRAPPLE_FIREREACHED,
	GRAPPLE_FIRETRAVEL,
	GRAPPLE_FIRERELEASE
};

LINK_ENTITY_TO_CLASS( weapon_grapple, CGrapple );

BOOL CGrapple::IsUseable( void )
{
	return TRUE;
}

void CGrapple :: Spawn( void ) 
{ 
	pev->classname = MAKE_STRING( "weapon_grapple" ); 

	Precache( ); 

	SET_MODEL(ENT(pev), "models/w_physgun.mdl");
	m_iId = WEAPON_GRAPPLE; 
	m_iDefaultAmmo = 1; 
		FallInit( ); 
}

void CGrapple :: Precache( void ) 
{ 
	PRECACHE_MODEL("models/v_physgun.mdl");
	PRECACHE_MODEL("models/p_physgun.mdl");
	PRECACHE_MODEL("models/w_physgun.mdl");
	PRECACHE_MODEL( "sprites/laserbeam.spr" );
	PRECACHE_MODEL( "models/v_physgun.mdl" ); 

	/*
	PRECACHE_SOUND("weapons/alienweap_draw.wav");	//pick up
	PRECACHE_SOUND("weapons/bgrapple_pull.wav");	//tongue pull player
	PRECACHE_SOUND("weapons/bgrapple_fire.wav");	//fire
	PRECACHE_SOUND("weapons/bgrapple_impact.wav");	//hit tongue
	PRECACHE_SOUND("weapons/bgrapple_release.wav");	//remove tongue
	PRECACHE_SOUND("weapons/bgrapple_wait.wav");	//idle
	PRECACHE_SOUND("weapons/bgrapple_cough.wav");	//idle
	*/

	UTIL_PrecacheOther("proj_hook");

	m_usTongue = PRECACHE_EVENT ( 1, "events/grapple.sc" );
	m_usTongue2 = PRECACHE_EVENT ( 1, "events/grapple.sc" );

}

int CGrapple::AddToPlayer( CBasePlayer *pPlayer )
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


int CGrapple :: GetItemInfo( ItemInfo* Info ) 
{ 
	Info->pszName = STRING( pev->classname ); 
	Info->pszAmmo1 = NULL; 
	Info->iMaxAmmo1 = -1; 
	Info->pszAmmo2 = NULL; 
	Info->iMaxAmmo2 = -1; 
	Info->iMaxClip = 1; 
	Info->iSlot = 0; 
	Info->iPosition = 2; 
	Info->iFlags = 0;
	Info->iId = WEAPON_GRAPPLE; 
	Info->iWeight = GRAPPLE_WEIGHT;
	return 1; 
}

BOOL CGrapple :: Deploy( void ) 
{ 
	return DefaultDeploy( "models/v_physgun.mdl", "models/p_physgun.mdl", GRAPPLE_UP, "hive", 0 );
	m_flTimeWeaponIdle = gpGlobals->time;
	SetThink (WeaponIdle);
}

void CGrapple::Holster( int skiplocal )
{
	//STOP_SOUND( ENT(pev), CHAN_WEAPON, "weapons/bgrapple_wait.wav" );
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
}

void CGrapple :: PrimaryAttack( ) 
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


	SendWeaponAnim( GRAPPLE_FIREWAITING, 1 );
//	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/bgrapple_fire.wav", 1, ATTN_NORM);
	m_pPlayer->m_iGrappleExists = 1;
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 ); 

	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle ); 
	Vector AimingDir = gpGlobals->v_forward; 
	Vector GunPosition = m_pPlayer->GetGunPosition( ); 

#ifndef CLIENT_DLL

		// single player spread
		GunPosition = GunPosition + gpGlobals->v_up * -4 + gpGlobals->v_right * 3 + gpGlobals->v_forward * 16;
		m_pPlayer->m_MyGrapple = CGrappleHook :: Create( GunPosition, m_pPlayer->pev->v_angle, m_pPlayer );

#endif
	//GunPosition = GunPosition + gpGlobals->v_up * -4 + gpGlobals->v_right * 3 + gpGlobals->v_forward * 16;
//	m_pPlayer->m_MyGrapple = CGrappleHook :: Create( GunPosition, m_pPlayer->pev->v_angle, m_pPlayer );


//	EMIT_SOUND( ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/bgrapple_pull.wav", 1, ATTN_NORM);
//		ALERT( at_console, "Think launched!!!\n" );

	SetThink(FlyThink);
	pev->nextthink = gpGlobals->time + 0.1;

}

void CGrapple :: FlyThink( void ) 
{
	
	if( m_pPlayer->m_afPhysicsFlags & PFLAG_ON_GRAPPLE ) //If we are on a grapple
	{
//		ALERT( at_console, "Splat, flying!!!\n" );

	SendWeaponAnim( GRAPPLE_FIRETRAVEL, 1 );
		
	SetThink(PullThink);
	}

	if ( !m_pPlayer->m_iGrappleExists )
	{
		SendWeaponAnim( GRAPPLE_FIRERELEASE, 1 );
		//EMIT_SOUND( ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/bgrapple_release.wav", 1, ATTN_NORM);

		m_flNextPrimaryAttack = m_flNextPrimaryAttack + 0.1;

		if (m_flNextPrimaryAttack < UTIL_WeaponTimeBase() )
		{
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.25;
		}

		SetThink (WeaponIdle);
		pev->nextthink = gpGlobals->time + 0.1;
		m_flTimeWeaponIdle = gpGlobals->time + 0.5;
	}

	pev->nextthink = gpGlobals->time + 0.1;
}

void CGrapple :: PullThink( void ) 
{
	if( m_pPlayer->m_afPhysicsFlags & PFLAG_ON_GRAPPLE ) //If we are on a grapple
	{
	TraceResult tr;

	UTIL_MakeVectors (m_pPlayer->pev->v_angle);
	Vector vecSrc = m_pPlayer->GetGunPosition();
	Vector vecEnd = vecSrc + gpGlobals->v_forward * 48;

#ifndef CLIENT_DLL
	UTIL_TraceLine( vecSrc, vecEnd, dont_ignore_monsters, ENT( m_pPlayer->pev ), &tr );

	//gSkillData.plrDmgGrapple = 20

	ClearMultiDamage();
	CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);
	pEntity->TraceAttack(m_pPlayer->pev, 20, gpGlobals->v_forward, &tr, DMG_CLUB || DMG_ALWAYSGIB ); 
	ApplyMultiDamage( m_pPlayer->pev, m_pPlayer->pev );
#endif
		pev->nextthink = gpGlobals->time + 0.1;
	}
	else 
	{

	SendWeaponAnim( GRAPPLE_FIRERELEASE, 1 );
//	EMIT_SOUND( ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/bgrapple_release.wav", 1, ATTN_NORM);

	m_flNextPrimaryAttack = m_flNextPrimaryAttack + 0.1;

//	if (m_flNextPrimaryAttack < UTIL_WeaponTimeBase() )
//	{
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 1;
//	}

	SetThink (WeaponIdle);
	pev->nextthink = gpGlobals->time + 0.1;
	m_flTimeWeaponIdle = gpGlobals->time + 0.1;
	}
}

void CGrapple :: WeaponIdle( void ) 
{
	ResetEmptySound( );

	m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );

	if ( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	int iAnim;
	switch ( RANDOM_LONG( 0, 1 ) )
	{
	case 0:	
		iAnim = GRAPPLE_SHORTIDLE;	
		break;
	
	default:
	case 1:
		iAnim = GRAPPLE_SHORTIDLE;
		break;
	}

	SendWeaponAnim( iAnim );
	
	m_flTimeWeaponIdle = UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 ); // how long till we do this again.
}