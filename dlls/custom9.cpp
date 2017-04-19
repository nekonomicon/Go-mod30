#include	"extdll.h"
#include	"plane.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"schedule.h"
#include	"animation.h"
#include	"squadmonster.h"
#include	"weapons.h"
#include	"talkmonster.h"
#include	"soundent.h"
#include	"effects.h"
#include	"customentity.h"

class Ccustom9 : public CBaseMonster
{
public:
        void Spawn( void );
        void Precache( void );
        int  Classify ( void );
        void SetActivity(int activity);                                 
        int  GetActivity() { return m_Activity; }
        void EXPORT IdleThink();
        int BloodColor( void ) { return DONT_BLEED; }
        int m_Activity;                                                                 //What entity is doing (animation)//
        
};
LINK_ENTITY_TO_CLASS(custom9, Ccustom9);

void Ccustom9::Spawn(void)
{
        Precache();
        
        pev->movetype   = MOVETYPE_STEP;
        pev->solid              = SOLID_BBOX;
        pev->takedamage = DAMAGE_NO;
      //  pev->flags              |= FL_MONSTER;
        pev->health             = 10;
        pev->gravity    = 1.0;
        
        SET_MODEL(ENT(pev), "models/gomod/custom9.mdl");

        SetActivity(ACT_IDLE);
        SetSequenceBox();
        SetThink(IdleThink);
        pev->nextthink = gpGlobals->time + 0.1;           

}

void Ccustom9::Precache(void)
{
        PRECACHE_MODEL("models/gomod/custom9.mdl");
}

int  Ccustom9::Classify(void)
{
        
        return  CLASS_NONE;

}

void Ccustom9::SetActivity(int act)
{       
        int sequence = LookupActivity( act ); 
        if ( sequence != ACTIVITY_NOT_AVAILABLE )
        {
                pev->sequence = sequence;
                m_Activity = act; 
                pev->frame = 0;
                ResetSequenceInfo( );
                //m_flFrameRate = 1.0;
        }

}

void Ccustom9::IdleThink()
{
        float flInterval = StudioFrameAdvance();

        pev->nextthink = gpGlobals->time + 1;
        DispatchAnimEvents(flInterval);

        if(!IsInWorld())
        {
                SetTouch(NULL);
                UTIL_Remove(this);
                return;
        }

}

//=========================================================
//=========================================================
