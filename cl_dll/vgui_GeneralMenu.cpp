/**********************************************************
*														  *
*		The Code was created by the Go-Mod 3 Team		  *
*		 2009-2010 Ranger366 & FITAMOD (HLMODER)		  *
*														  *
**********************************************************/

#include "hud.h"
#include "cl_util.h"
#include "vgui_TeamFortressViewport.h"

CGeneralMenu :: CGeneralMenu(int iTrans, int iRemoveMe, int x, int y, int wide, int tall) : CMenuPanel(iTrans, iRemoveMe, x,y,wide,tall)
{
   
    CSchemeManager *pSchemes = gViewPort->GetSchemeManager();
    SchemeHandle_t hTitleScheme = pSchemes->getSchemeHandle( "Title Font" );
 
	int	lado = 10;
   
 
     m_pPanel = new CTransparentPanel( 0, ScreenWidth/2 - 320, 50, 650, 200);
     m_pPanel->setParent( this );
 




	 	// Imagen DuplicateMode
    m_pMyPicture = new CImageLabel( "duplicate_select", 1+lado, 1);
    m_pMyPicture->setVisible( true );
    m_pMyPicture->setParent( m_pPanel );
    // End - DuplicateMode


	// Boton - DuplicateMode 
    m_pSpeak = new CommandButton( "",3+lado, 1, 123, 49);
    m_pSpeak->setContentAlignment( vgui::Label::a_center );
    m_pSpeak->setParent( m_pPanel );
    m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "duplicatemode"));
	m_pSpeak->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );
	m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "weapon_removetool"));
    // End - DuplicateMode 




		// Imagen RenderMode
    m_pMyPicture = new CImageLabel( "render_select", 129+lado, 1);
    m_pMyPicture->setVisible( true );
    m_pMyPicture->setParent( m_pPanel );
    // End - RenderMode



	// Boton - RenderMode 
    m_pSpeak = new CommandButton( "",129+lado, 1, 123, 49);
    m_pSpeak->setContentAlignment( vgui::Label::a_center );
    m_pSpeak->setParent( m_pPanel );
    m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "rendermode"));
	m_pSpeak->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );
	m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "vguimenu 49"));
	m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "weapon_removetool"));
    // End - RenderMode 


	// Imagen RemoveMode
    m_pMyPicture = new CImageLabel( "remove_select", 257+lado, 1);
    m_pMyPicture->setVisible( true );
    m_pMyPicture->setParent( m_pPanel );
    // End - RemoveMode
	


	// Boton - RemoveMode 
    m_pSpeak = new CommandButton( "",257+lado, 1, 123, 49);
    m_pSpeak->setContentAlignment( vgui::Label::a_center );
    m_pSpeak->setParent( m_pPanel );
    m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "removemode"));
	m_pSpeak->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );
	m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "weapon_removetool"));
    // End - RemoveMode 




	// Imagen CameraMode
    m_pMyPicture = new CImageLabel( "camera_select", 385+lado, 1);
    m_pMyPicture->setVisible( true );
    m_pMyPicture->setParent( m_pPanel );
    // End - CameraMode
	


	// Boton - CameraMode 
    m_pSpeak = new CommandButton( "",385+lado, 1, 123, 49);
    m_pSpeak->setContentAlignment( vgui::Label::a_center );
    m_pSpeak->setParent( m_pPanel );
    m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "cameramode"));
	m_pSpeak->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );
	m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "weapon_removetool"));
    // End - CameraMode 



	// Imagen CameraMode
    m_pMyPicture = new CImageLabel( "camera_select", 385+lado, 1);
    m_pMyPicture->setVisible( true );
    m_pMyPicture->setParent( m_pPanel );
    // End - CameraMode
	


	// Boton - CameraMode 
    m_pSpeak = new CommandButton( "",385+lado, 1, 123, 49);
    m_pSpeak->setContentAlignment( vgui::Label::a_center );
    m_pSpeak->setParent( m_pPanel );
    m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "cameramode"));
	m_pSpeak->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );
	m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "weapon_removetool"));
    // End - CameraMode 





	// Imagen GibMode
    m_pMyPicture = new CImageLabel( "gib_select", 510+lado, 1);
    m_pMyPicture->setVisible( true );
    m_pMyPicture->setParent( m_pPanel );
    // End - GibMode
	


	// Boton - GibMode 
    m_pSpeak = new CommandButton( "",514+lado, 1, 120, 49);
    m_pSpeak->setContentAlignment( vgui::Label::a_center );
    m_pSpeak->setParent( m_pPanel );
    m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "gibmode"));
	m_pSpeak->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );
	m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "weapon_removetool"));
    // End - GibMode 







	// Imagen PoserMode
    m_pMyPicture = new CImageLabel( "poser_select", 1+lado, 50);
    m_pMyPicture->setVisible( true );
    m_pMyPicture->setParent( m_pPanel );
    // End - PoserMode


	// Boton - PoserMode 
    m_pSpeak = new CommandButton( "",3+lado, 50, 123, 49);
    m_pSpeak->setContentAlignment( vgui::Label::a_center );
    m_pSpeak->setParent( m_pPanel );
    m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "posermode"));
	m_pSpeak->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );
	m_pSpeak->addActionSignal( new CMenuHandler_StringCommand( "weapon_removetool"));
    // End - PoserMode 



}




void CGeneralMenu :: SetActiveInfo( int iShowText )
{
   
	iShowText = iShowText + 1;
}