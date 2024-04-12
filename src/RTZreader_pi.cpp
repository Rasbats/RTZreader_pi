/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  RTZreader Plugin
 * Author:   Mike Rossiter
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Mike Rossiter                                *
 *   $EMAIL$                                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "RTZreader_pi.h"
#include "RTZreadergui_impl.h"
#include "RTZreadergui.h"
#include <wx/filename.h>

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new RTZreader_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//    RTZreader PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

#include "icons.h"

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

RTZreader_pi::RTZreader_pi(void *ppimgr)
      :opencpn_plugin_118 (ppimgr)
{
      // Create the PlugIn icons
      initialize_images();

      wxFileName fn;

      auto path = GetPluginDataDir("RTZreader_pi");
      fn.SetPath(path);
      fn.AppendDir("data");
      fn.SetFullName("RTZreader_panel_icon.png");

      path = fn.GetFullPath();

      wxInitAllImageHandlers();

      wxLogDebug(wxString("Using icon path: ") + path);
      if (!wxImage::CanRead(path)) {
      wxLogDebug("Initiating image handlers.");
      wxInitAllImageHandlers();
      }
      wxImage panelIcon(path);
      if (panelIcon.IsOk())
      m_panelBitmap = wxBitmap(panelIcon);
      else
      wxLogWarning("aisRX panel icon has NOT been loaded");

	   
	  m_bShowRTZreader = false;
}

RTZreader_pi::~RTZreader_pi(void)
{
     delete _img_RTZreader_pi;
     delete _img_RTZreader;
     
}

int RTZreader_pi::Init(void)
{
      AddLocaleCatalog( _T("opencpn-RTZreader_pi") );

      // Set some default private member parameters
      m_route_dialog_x = 0;
      m_route_dialog_y = 0;
      ::wxDisplaySize(&m_display_width, &m_display_height);

      //    Get a pointer to the opencpn display canvas, to use as a parent for the POI Manager dialog
      m_parent_window = GetOCPNCanvasWindow();

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();

      //    And load the configuration items
      LoadConfig();

      //    This PlugIn needs a toolbar icon, so request its insertion
	if(m_bRTZreaderShowIcon)

#ifdef ocpnUSE_SVG
		m_leftclick_tool_id = InsertPlugInToolSVG(_T("RTZreader"), _svg_RTZreader, _svg_RTZreader, _svg_RTZreader_toggled,
			wxITEM_CHECK, _("RTZreader"), _T(""), NULL, CALCULATOR_TOOL_POSITION, 0, this);
#else
		m_leftclick_tool_id = InsertPlugInTool(_T(""), _img_RTZreader, _img_RTZreader, wxITEM_CHECK,
			_("RTZreader"), _T(""), NULL,
			CALCULATOR_TOOL_POSITION, 0, this);
#endif
    

      m_pDialog = NULL;

      return (WANTS_OVERLAY_CALLBACK |
              WANTS_OPENGL_OVERLAY_CALLBACK |
		  
		      WANTS_CURSOR_LATLON      |
              WANTS_TOOLBAR_CALLBACK    |
              INSTALLS_TOOLBAR_TOOL     |
              WANTS_CONFIG             |
			  WANTS_PLUGIN_MESSAGING

           );
}

bool RTZreader_pi::DeInit(void)
{
      //    Record the dialog position
      if (NULL != m_pDialog)
      {
            //Capture dialog position
            wxPoint p = m_pDialog->GetPosition();
            SetCalculatorDialogX(p.x);
            SetCalculatorDialogY(p.y);
            m_pDialog->Close();
            delete m_pDialog;
            m_pDialog = NULL;

			m_bShowRTZreader = false;
			SetToolbarItemState( m_leftclick_tool_id, m_bShowRTZreader );

      }	
    
    SaveConfig();

    RequestRefresh(m_parent_window); // refresh main window

    return true;
}

int RTZreader_pi::GetAPIVersionMajor()
{ 
  return atoi(API_VERSION); 

}

int RTZreader_pi::GetAPIVersionMinor()
{
    std::string v(API_VERSION);
    size_t dotpos = v.find('.');
    return atoi(v.substr(dotpos + 1).c_str());
}

int RTZreader_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int RTZreader_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

int GetPlugInVersionPatch() { return PLUGIN_VERSION_PATCH; }

int GetPlugInVersionPost() { return PLUGIN_VERSION_TWEAK; };

const char *GetPlugInVersionPre() { return PKG_PRERELEASE; }

const char *GetPlugInVersionBuild() { return PKG_BUILD_INFO; }

wxBitmap *RTZreader_pi::GetPlugInBitmap()
{
      return &m_panelBitmap;
}

wxString RTZreader_pi::GetCommonName()
{
      return _("RTZreader");
}


wxString RTZreader_pi::GetShortDescription()
{
      return _("RTZreader Positions using GPX files");
}

wxString RTZreader_pi::GetLongDescription()
{
      return _("Creates GPX files with\n\
RTZreader Positions");
}

int RTZreader_pi::GetToolbarToolCount(void)
{
      return 1;
}

void RTZreader_pi::SetColorScheme(PI_ColorScheme cs)
{
      if (NULL == m_pDialog)
            return;

      DimeWindow(m_pDialog);
}

void RTZreader_pi::OnToolbarToolCallback(int id)
{
    
	if(NULL == m_pDialog)
      {
            m_pDialog = new Dlg(m_parent_window, this);
            m_pDialog->Move(wxPoint(m_route_dialog_x, m_route_dialog_y));

            
      }

	  m_pDialog->Fit();
	  //Toggle 
	  m_bShowRTZreader = !m_bShowRTZreader;	  

      //    Toggle dialog? 
      if(m_bShowRTZreader) {
          m_pDialog->Show();         
      } else
          m_pDialog->Hide();
     
      // Toggle is handled by the toolbar but we must keep plugin manager b_toggle updated
      // to actual status to ensure correct status upon toolbar rebuild
      SetToolbarItemState( m_leftclick_tool_id, m_bShowRTZreader );

      RequestRefresh(m_parent_window); // refresh main window
}

bool RTZreader_pi::LoadConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T( "/Settings/RTZreader_pi" ) );
			 pConf->Read ( _T( "ShowRTZreaderIcon" ), &m_bRTZreaderShowIcon, 1 );
           
            m_route_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
            m_route_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 20L );
         
            if((m_route_dialog_x < 0) || (m_route_dialog_x > m_display_width))
                  m_route_dialog_x = 5;
            if((m_route_dialog_y < 0) || (m_route_dialog_y > m_display_height))
                  m_route_dialog_y = 5;
            return true;
      }
      else
            return false;
}

bool RTZreader_pi::SaveConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T ( "/Settings/RTZreader_pi" ) );
			pConf->Write ( _T ( "ShowRTZreaderIcon" ), m_bRTZreaderShowIcon );
          
            pConf->Write ( _T ( "DialogPosX" ),   m_route_dialog_x );
            pConf->Write ( _T ( "DialogPosY" ),   m_route_dialog_y );
            
            return true;
      }
      else
            return false;
}

void RTZreader_pi::OnRTZreaderDialogClose()
{
    m_bShowRTZreader = false;
    SetToolbarItemState( m_leftclick_tool_id, m_bShowRTZreader );
    m_pDialog->Hide();
    SaveConfig();

    RequestRefresh(m_parent_window); // refresh main window

}


