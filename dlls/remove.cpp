/**********************************************************
*														  *
*		The Code was created by the Go-Mod 3 Team		  *
*		 2009-2010 Ranger366 & FITAMOD (HLMODER)		  *
*														  *
**********************************************************/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "gamerules.h"

#ifdef CLIENT_DLL
#include "render.h"
#endif
/*
extern float r_duplicatemode;
extern float r_rendermode;
extern float r_removemode;
extern float r_cameramode;
extern float r_gibmode;
extern float r_posermode;
*/
enum removetool_e
{

	REMOVETOOL_IDLE,
	REMOVETOOL_IDLE2,
	//REMOVETOOL_LAUNCH,
	REMOVETOOL_RELOAD,
	REMOVETOOL_SHOOT,
	REMOVETOOL_SHOOT2,
	REMOVETOOL_DRAW,
	
};



LINK_ENTITY_TO_CLASS( weapon_removetool, CRemoveTool);


#ifndef CLIENT_DLL

LINK_ENTITY_TO_CLASS( laser_spot_new, CLaserSpotNew );

//=========================================================
//=========================================================
CLaserSpotNew *CLaserSpotNew::CreateSpotNew( void )
{
	CLaserSpotNew *pSpot = GetClassPtr( (CLaserSpotNew *)NULL );
	pSpot->Spawn();

	pSpot->pev->classname = MAKE_STRING("laser_spot_new");

	return pSpot;
}

//=========================================================
//=========================================================
void CLaserSpotNew::Spawn( void )
{
	Precache( );
	pev->movetype = MOVETYPE_NONE;
	pev->solid = SOLID_NOT;

	pev->rendermode = kRenderGlow;
	pev->renderfx = kRenderFxNoDissipation;
	pev->renderamt = 255;

	SET_MODEL(ENT(pev), "sprites/laserdot2.spr");
	UTIL_SetOrigin( pev, pev->origin );
};

//=========================================================
// Suspend- make the laser sight invisible. 
//=========================================================
void CLaserSpotNew::Suspend( float flSuspendTime )
{
	pev->effects |= EF_NODRAW;
	
	SetThink( Revive );
	pev->nextthink = gpGlobals->time + flSuspendTime;
}

//=========================================================
// Revive - bring a suspended laser sight back.
//=========================================================
void CLaserSpotNew::Revive( void )
{
	pev->effects &= ~EF_NODRAW;

	SetThink( NULL );
}

void CLaserSpotNew::Precache( void )
{
	PRECACHE_MODEL("sprites/laserdot2.spr");
};
#endif

//=========================================================
//=========================================================


void CRemoveTool::Spawn( )
{
	pev->classname = MAKE_STRING("weapon_removetool"); // hack to allow for old names
	Precache( );
	SET_MODEL(ENT(pev), "models/w_removetool.mdl");
	m_iId = WEAPON_REMOVETOOL;

	m_fSpotActive = 1;

	m_iDefaultAmmo = 1;

	FallInit();// get ready to fall down.
}



void CRemoveTool::Precache( void )
{
	PRECACHE_MODEL("models/v_removetool.mdl");
	PRECACHE_MODEL("models/w_removetool.mdl");
	PRECACHE_MODEL("models/p_removetool.mdl");

	m_iShell = PRECACHE_MODEL ("models/shell.mdl");// brass shellTE_MODEL


	PRECACHE_SOUND ("weapons/tg_shot1.wav");// H to the K
	PRECACHE_SOUND ("weapons/tg_shot2.wav");// H to the K
	PRECACHE_SOUND ("weapons/hks3.wav");// H to the K



	UTIL_PrecacheOther( "laser_spot_new" );

	m_Chain = PRECACHE_MODEL( "sprites/laserbeam.spr" ); 
	PRECACHE_SOUND ("weapons/357_cock1.wav");

	m_removetool = PRECACHE_EVENT( 1, "events/removetool.sc" );

}

int CRemoveTool::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "9mm";
	p->iMaxAmmo1 = 1;
	p->pszAmmo2 = "NULL";
	p->iMaxAmmo2 = -1;
	p->iMaxClip = 1;
	p->iSlot = 0;
	p->iPosition = 3;
	p->iFlags = 2;
	p->iId = m_iId = WEAPON_REMOVETOOL;
	p->iWeight = MP5_WEIGHT;

	return 1;
}

int CRemoveTool::AddToPlayer( CBasePlayer *pPlayer )
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

BOOL CRemoveTool::Deploy( )
{
	return DefaultDeploy( "models/v_removetool.mdl", "models/p_removetool.mdl", REMOVETOOL_DRAW, "mp5" );
}


void CRemoveTool::PrimaryAttack()
{

	// don't fire underwater
	if (m_pPlayer->pev->waterlevel == 3)
	{
		PlayEmptySound( );
		m_flNextPrimaryAttack = 0.15;
		return;
	}
	UpdateSpot( );
	m_pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;



	m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;

	// player "shoot" animation
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	Vector vecSrc	 = m_pPlayer->GetGunPosition( );
//	Vector vecAiming = m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );
	Vector vecDir;
	Vector vecEnd;
	Vector vecAiming;
	

	vecAiming = gpGlobals->v_forward;
	
	TraceResult tr;
	vecEnd = vecSrc + vecAiming * 8192;
	UTIL_TraceLine(vecSrc, vecEnd, dont_ignore_monsters, ENT(pev)/*pentIgnore*/, &tr);


#ifdef CLIENT_DLL
	if ( !bIsMultiplayer() )
#else
	if ( !g_pGameRules->IsMultiplayer() )
#endif
	{
		// optimized multiplayer. Widened to make it easier to hit a moving player
		
		if ( m_pPlayer->r_duplicatemode == 1)
		{
			vecDir = m_pPlayer->FireBulletsDuplicator( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );

		}

		if (  m_pPlayer->r_rendermode == 1)
		{
			vecDir = m_pPlayer->FireBulletsRemoveToolRender( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
		}

		if (  m_pPlayer->r_removemode == 1)
		{
			vecDir = m_pPlayer->FireBulletsRemoveTool( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
		}

		if (  m_pPlayer->r_cameramode == 1)
		{
		//	vecDir = m_pPlayer->FireBulletsRemoveTool( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
			UTIL_MakeVectors( Vector( pev->v_angle.x*-1, pev->v_angle.y, pev->v_angle.z ) );
			CBaseEntity::Create("monster_camera", pev->origin, pev->angles,pev->owner,m_pPlayer);
		}

		if (  m_pPlayer->r_gibmode == 1)
		{
			#ifndef CLIENT_DLL
				CGib::SpawnRandomGibs( pev, 1, 1 );
			#endif
		}

		
		if ( m_pPlayer-> r_posermode == 1)
		{
			vecDir = m_pPlayer->FireBulletsPoserTool( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
		}
	}
	else
	{
		// single player spread

		//vecDir = m_pPlayer->FireBulletsRemoveTool( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
			
		if (  m_pPlayer->r_duplicatemode == 1)
		{
			vecDir = m_pPlayer->FireBulletsDuplicator( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );

		}

		if (  m_pPlayer->r_rendermode == 1)
		{
			vecDir = m_pPlayer->FireBulletsRemoveToolRender( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
		}

		if (  m_pPlayer->r_removemode == 1)
		{
			vecDir = m_pPlayer->FireBulletsRemoveTool( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
		}

		if (  m_pPlayer->r_cameramode == 1)
		{
		//	vecDir = m_pPlayer->FireBulletsRemoveTool( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
			UTIL_MakeVectors( Vector( pev->v_angle.x, pev->v_angle.y, pev->v_angle.z ) );
			CBaseEntity::Create("monster_camera", pev->origin, pev->angles,pev->owner,m_pPlayer);
		}

		if (  m_pPlayer->r_gibmode == 1)
		{
			#ifndef CLIENT_DLL
				CGib::SpawnRandomGibs( pev, 1, 1 );
			#endif
		}

		if (  m_pPlayer->r_posermode == 1)
		{
			vecDir = m_pPlayer->FireBulletsPoserTool( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
		}

	}

  int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif
	

	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_removetool, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, 0, 0, 0, 0 );

	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);


	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.1;

	if ( m_flNextPrimaryAttack < UTIL_WeaponTimeBase() )
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.1;

	

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );

}





void CRemoveTool::Holster( int skiplocal /* = 0 */ )
{

	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
	

#ifndef CLIENT_DLL
	if (m_pSpot)
	{
		m_pSpot->Killed( NULL, GIB_NEVER );
		m_pSpot = NULL;
	}
#endif

}



void CRemoveTool::SecondaryAttack()
{

	// don't fire underwater
	if (m_pPlayer->pev->waterlevel == 3)
	{
		PlayEmptySound( );
		m_flNextPrimaryAttack = 0.15;
		return;
	}
/*
	if (m_iClip <= 0)
	{
		PlayEmptySound();
		m_flNextPrimaryAttack = 0.15;
		return;
	}
*/
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
		
		if (  m_pPlayer->r_duplicatemode == 1)
		{
			vecDir = m_pPlayer->FireBulletsDuplicatorSelect( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );

		}



		if (  m_pPlayer->r_posermode == 1)
		{
			vecDir = m_pPlayer->FireBulletsPoserToolSelect( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
		}

	//	vecDir = m_pPlayer->FireBulletsRemoveToolRender( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
	}
	else
	{
		// single player spread

			
		if (  m_pPlayer->r_duplicatemode == 1)
		{
			vecDir = m_pPlayer->FireBulletsDuplicatorSelect( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );

		}

		if (  m_pPlayer->r_posermode == 1)
		{
			vecDir = m_pPlayer->FireBulletsPoserToolSelect( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
		}

	//	vecDir = m_pPlayer->FireBulletsRemoveToolRender( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
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

	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_removetool, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, 0, 0, 0, 0 );

	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);


	m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.1;

	if ( m_flNextSecondaryAttack < UTIL_WeaponTimeBase() )
		m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.1;

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );

	UpdateSpot( );
}



void CRemoveTool::WeaponIdle( void )
{

	UpdateSpot();

	ResetEmptySound( );

//	m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );

	if ( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	int iAnim;
	switch ( RANDOM_LONG( 0, 1 ) )
	{
	case 0:	
		iAnim = REMOVETOOL_IDLE;	
		break;
	
	default:
	case 1:
		iAnim = REMOVETOOL_IDLE2;
		break;
	}

	SendWeaponAnim( iAnim );



	//LONG APRT INFO END
	
	m_flTimeWeaponIdle = UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 ); // how long till we do this again.

	UpdateSpot( );
	
}



void CRemoveTool::UpdateSpot( void )
{
#ifndef CLIENT_DLL
	if (m_fSpotActive)
	{
		if (!m_pSpot)
		{
			m_pSpot = CLaserSpotNew::CreateSpotNew();
		}

		UTIL_MakeVectors( m_pPlayer->pev->v_angle );
		Vector vecSrc = m_pPlayer->GetGunPosition( );;
		Vector vecAiming = gpGlobals->v_forward;

		TraceResult tr;
		UTIL_TraceLine ( vecSrc, vecSrc + vecAiming * 8192, dont_ignore_monsters, ENT(m_pPlayer->pev), &tr );
		
		UTIL_SetOrigin( m_pSpot->pev, tr.vecEndPos );

	}
#endif


}