/**********************************************************
*														  *
*		The Code was created by the Go-Mod 3 Team		  *
*		 2009-2010 Ranger366 & FITAMOD (HLMODER)		  *
*														  *
**********************************************************/

#include "hud.h"
#include "cl_util.h"
#include "vgui_TeamFortressViewport.h"

CLinkMenu :: CLinkMenu(int iTrans, int iRemoveMe, int x, int y, int wide, int tall) : CMenuPanel(iTrans, iRemoveMe, x,y,wide,tall)
{
    // Start -
    CSchemeManager *pSchemes = gViewPort->GetSchemeManager();
    SchemeHandle_t hTitleScheme = pSchemes->getSchemeHandle( "Title Font" );
 
   
	int lado = 12;
    // End 
 
    m_pPanel = new CTransparentPanel( 0, ScreenWidth/2 - 260, 0, 520, 150);
    m_pPanel->setParent( this );
 
 
 






		///////////   START  PRIMERA FILA \\\\\\\\\\\\\\\\\\\\

	
	// Imagen Link Half Life Menu
    m_pMyPicture = new CImageLabel( "half-life_link", 1+lado, 1);
    m_pMyPicture->setVisible( true );
    m_pMyPicture->setParent( m_pPanel );
    // End - Link Half Life Menu


		//Imagen  Half Life Menu Weapon
		m_pHLWeaponImage = new CImageLabel( "half-life_weapon_menu_link", 1+lado, 60);
		m_pHLWeaponImage->setVisible( false );
		m_pHLWeaponImage->setParent( m_pPanel );
		// End - Link Half Life Menu Weapon


		
		// Boton -  Half Life Menu Weapon
		m_pHLWeapon = new CommandButton( "",4+lado, 65, 50, 25);
		m_pHLWeapon->setContentAlignment( vgui::Label::a_center );
		m_pHLWeapon->setParent( m_pPanel );
	    m_pHLWeapon->addActionSignal( new CMenuHandler_StringCommand( "vguimenu 45" ) );
		m_pHLWeapon->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );
		m_pHLWeapon->setVisible( false );
		// End -  Half Life Menu Weapon


		
		//Imagen  Half Life Menu NPCs
		m_pHLNpcsImage = new CImageLabel( "half-life_npc_menu_link", 60+lado, 60);
		m_pHLNpcsImage->setVisible( false );
		m_pHLNpcsImage->setParent( m_pPanel );
		// End - Link Half Life Menu NPCs


		
		// Boton -  Half Life Menu NPCs
		m_pHLNpcs = new CommandButton( "",63+lado, 65, 53, 25);
		m_pHLNpcs->setContentAlignment( vgui::Label::a_center );
		m_pHLNpcs->setParent( m_pPanel );
	    m_pHLNpcs->addActionSignal( new CMenuHandler_StringCommand( "vguimenu 30" ) );
		m_pHLNpcs->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );
		m_pHLNpcs->setVisible( false );
		// End -  Half Life Menu NPCs



		//Imagen  Half Life Menu Props
		m_pHLPropsImage = new CImageLabel( "half-life_prop_menu_link", 30+lado, 90);
		m_pHLPropsImage->setVisible( false );
		m_pHLPropsImage->setParent( m_pPanel );
		// End - Link Half Life Menu Props


		
		// Boton -  Half Life Menu Props
		m_pHLProps = new CommandButton( "",33+lado, 95, 53, 25);
		m_pHLProps->setContentAlignment( vgui::Label::a_center );
		m_pHLProps->setParent( m_pPanel );
	    m_pHLProps->addActionSignal( new CMenuHandler_StringCommand( "vguimenu 48" ) );
		m_pHLProps->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );
		m_pHLProps->setVisible( false );
		// End -  Half Life Menu Props







	// Boton - hgrunt Start
    m_pSpeak = new CommandButton( "",3+lado, 1, 123, 49);
    m_pSpeak->setContentAlignment( vgui::Label::a_center );
    m_pSpeak->setParent( m_pPanel );
   // m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "vguimenu 30" ) );
	m_pSpeak->addInputSignal(new CHandler_MenuButtonOver(this, 0));
	//m_pSpeak->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );
	//m_pSpeak->setBorder( new LineBorder( Color(50 * 0.7,50 * 0.7,200,0) ) );
    // End - hgrunt





	// Imagen Counter-strike_link
    m_pMyPicture = new CImageLabel( "counter-strike_link", 129+lado, 1 );
    m_pMyPicture->setVisible( true );
    m_pMyPicture->setParent( m_pPanel );
    // End - Counter-strike_link

	// Boton - Counter-strike_link
    m_pSpeak = new CommandButton( "",129+lado, 1, 123, 49);
    m_pSpeak->setContentAlignment( vgui::Label::a_center );
    m_pSpeak->setParent( m_pPanel );
	m_pSpeak->addInputSignal(new CHandler_MenuButtonOver(this, 1));
    m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "vguimenu 47" ) );
	m_pSpeak->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );
    // End - Counter-strike_link






	// Imagen Toolgun Menu
    m_pMyPicture = new CImageLabel( "toolgun_link", 254+lado, 1 );
    m_pMyPicture->setVisible( true );
    m_pMyPicture->setParent( m_pPanel );
    // End - Toolgun Menu

	// Boton - Toolgun Menu
    m_pSpeak = new CommandButton( "",257+lado, 1, 123, 49);
    m_pSpeak->setContentAlignment( vgui::Label::a_center );
    m_pSpeak->setParent( m_pPanel );
	m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "vguimenu 51" ) );
    m_pSpeak->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );
    // End - Toolgun Menu



		// Imagen Close Menu
    m_pMyPicture = new CImageLabel( "close_link_menu", 380+lado, 1 );
    m_pMyPicture->setVisible( true );
    m_pMyPicture->setParent( m_pPanel );
    // End - Close Menu

	// Boton - Close Menu
    m_pSpeak = new CommandButton( "",383+lado, 1, 123, 49);
    m_pSpeak->setContentAlignment( vgui::Label::a_center );
    m_pSpeak->setParent( m_pPanel );
	m_pSpeak->addInputSignal(new CHandler_MenuButtonOver(this, 1));
	m_pSpeak->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );
    // End - Close Menu


   
}




void CLinkMenu :: SetActiveInfo( int iShowText )
{
   
	if(iShowText == 0)
	{
		m_pHLWeaponImage->setVisible( true );
		m_pHLWeapon->setVisible( true );

		m_pHLNpcsImage->setVisible( true );
		m_pHLNpcs->setVisible( true );

		m_pHLPropsImage->setVisible( true );
		m_pHLProps->setVisible( true );
	}

	if(iShowText == 1)
	{
		m_pHLWeaponImage->setVisible( false );
		m_pHLWeapon->setVisible( false );

		m_pHLNpcsImage->setVisible( false );
		m_pHLNpcs->setVisible( false );

		m_pHLPropsImage->setVisible( false );
		m_pHLProps->setVisible( false );
	}
}



void CLinkMenu :: setVisible( bool visible )
{
    if (visible)
	{
	
	}
    else
	{
       	m_pHLWeaponImage->setVisible( false );
		m_pHLWeapon->setVisible( false );

		m_pHLNpcsImage->setVisible( false );
		m_pHLNpcs->setVisible( false );

		m_pHLPropsImage->setVisible( false );
		m_pHLProps->setVisible( false );
	}

    CMenuPanel::setVisible(visible);
}