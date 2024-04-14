/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  RTZreader Plugin
 * Author:   Mike Rossiter
 *
 ***************************************************************************
 *   Copyright (C) 2024 by Mike Rossiter                                   *
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

#include "RTZreadergui_impl.h"
#include "RTZreadergui.h"
#include "RTZreader_pi.h"
#include "icons.h"

#include <wx/progdlg.h>
#include <wx/wx.h>
#include "wx/dir.h"
#include <list>
#include <cmath>
#include "pugixml.hpp"

class waypoint;
class route;
class rtept;
class rte;

using namespace pugi;

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(Plugin_WaypointExList);

#define FAIL(X)  \
  do {           \
    error = X;   \
    goto failed; \
  } while (0)

Dlg::Dlg(wxWindow* parent, RTZreader_pi* ppi) : m_Dialog(parent) {
  this->Fit();
  dbg = false;  // for debug output set to true

  pPlugIn = ppi;
  pParent = parent;

  m_choiceSchema->SetSelection(0);

}

Dlg::~Dlg() {}

void Dlg::Addpoint(xml_node Route, wxString ptlat, wxString ptlon,
                   wxString ptname, wxString ptsym, wxString pttype,
                   wxString ptviz) {
  xml_node RoutePoint = Route.append_child("rtept");
  RoutePoint.append_attribute("lat") = ptlat.mb_str();
  RoutePoint.append_attribute("lon") = ptlon.mb_str();

  xml_node Name = RoutePoint.append_child("name");
  Name.set_value(ptname.mb_str());

  xml_node Symbol = RoutePoint.append_child("sym");
  Symbol.set_value(ptsym.mb_str());

  xml_node Type = RoutePoint.append_child("type");
  Type.set_value(pttype.mb_str());
}

bool Dlg::ReadGPX(wxFileName my_file) {
  int response = wxID_CANCEL;
  int my_count = 0;

  wxArrayString file_array;

  wxString m_gpx_path;
  wxFileDialog openDialog(this, _("Import GPX Route file"), m_gpx_path, wxT(""),
                          wxT("GPX files (*.gpx)|*.gpx|All files (*.*)|*.*"),
                          wxFD_OPEN);
  response = openDialog.ShowModal();
  if (response == wxID_OK) {
    openDialog.GetPaths(file_array);

    //    Record the currently selected directory for later use
    if (file_array.GetCount()) {
      wxFileName fn(file_array[0]);
      filename = file_array[0];
      m_gpx_path = fn.GetPath();
      return true;
    }
  } else if (response == wxID_CANCEL) {
    return false;
  }

  return false;
}

int Dlg::exWriteGPX() {
  // temporary place holders
  route myRoute;
  waypoint myWaypoint;
  vector<waypoint> myVectorWaypoints;
  // end of placeholders

  wxString isExtension = "";

  string readFile = "dom_out.txt";  // intermediate file - make temporary?
  vector<string> myVector;

  ifstream ips;
  ips.open(readFile);
  string line;
  while (getline(ips, line)) {
    std::string::iterator end_pos = std::remove(line.begin(), line.end(), ' ');
    line.erase(end_pos, line.end());

    myVector.push_back(line);  // hold lines of file, without white space
  }

  int c = myVector.size();

  for (int z = 0; z < c; z++) {
    if (myVector[z] == "tag:extensions") {
      break;
    }

    wxStringTokenizer tokenizer(myVector[z], ":");
    while (tokenizer.HasMoreTokens()) {
      wxString token = tokenizer.GetNextToken();
      // process token here

      if (token == "routeName") {
        myRoute.routeName = tokenizer.GetNextToken();
      }

      if (token == "tag") {
        isExtension = tokenizer.GetNextToken();
        if (isExtension == "extension") {
          break;  // this eliminates RTZ refpoints, which have lat/lon and could
                  // be misinterpreted as waypoints.
        }
      }

      if (token == "id") {
        myWaypoint.id = tokenizer.GetNextToken();
      }

      if (token == "name") {
        myWaypoint.name = tokenizer.GetNextToken();
      }

      if (token == "lat") {
        myWaypoint.lat = tokenizer.GetNextToken();
      }

      if (token == "lon") {
        myWaypoint.lon = tokenizer.GetNextToken();
        //
        myRoute.waypoints.push_back(myWaypoint);
        //
      }
    }
  }

  int waypointCount = myRoute.waypoints.size();

  // Start GPX
  pugi::xml_document doc;

  // add a custom declaration node
  auto decl = doc.prepend_child();
  decl.append_attribute("version") = "1.0";
  decl.append_attribute("encoding") = "UTF-8";
  decl.append_attribute("standalone") = "no";

  // doc.linkendchild(decl);

  xml_node root = doc.append_child("gpx");
  xml_node Route = root.append_child("rte");
  xml_node RouteName = Route.append_child("name");
  // xml_node text4 = new TiXmlText(this->m_Route->GetValue().ToUTF8());

  root.append_attribute("value") = "version, 1.1";
  root.append_attribute("value") = "creator, RTZreader_pi by Rasbats";
  root.append_attribute("value") =
      "xmlns:xsi, http://www.w3.org/2001/XMLSchema-instance";
  root.append_attribute("value") = "xmlns, http://www.topografix.com/GPX/1/1";
  root.append_attribute("value") =
      "xmlns:gpxx, http://www.garmin.com/xmlschemas/GpxExtensions/v3";
  root.append_attribute("value") =
      "xsi:schemaLocation, http://www.topografix.com/GPX/1/1/gpx.xsd";

  root.append_attribute("value") = "xmlns:opencpn, http://www.opencpn.org";
  RouteName.set_value(m_Route->GetValue());

  xml_node Extensions = Route.append_child("extensions");

  xml_node StartN = Extensions.append_child("opencpn:start");

  xml_node EndN = Extensions.append_child("opencpn:end");

  xml_node Speed = Extensions.append_child("opencpn:planned_speed");

  if (dbg) cout << "RTZreader Calculation\n";
  double speed = 5;
  int interval = 1;

  interval += 1;

  speed = speed * (double)interval;

  int n = 0;

  double lati, loni;
  double latN[100], lonN[100], rad[100];  // allow for up to 100 waypoints
  wxString wpName[100];                   // allow for up to 100 waypoints
  double latF, lonF;

  Position my_point;
  wpt_num = 1;
  double value = 1.0;
  double value1 = 1.0;

  for (std::vector<Position>::iterator it = my_positions.begin();
       it != my_positions.end(); it++) {
    if (!(*it).lat.ToDouble(&value)) { /* error! */
    }
    lati = value;
    if (!(*it).lon.ToDouble(&value1)) { /* error! */
    }
    loni = value1;

    latN[n] = lati;
    lonN[n] = loni;

    wpName[n] = (*it).wpName;

    n++;  // 0,1,2,3
  }

  my_points.clear();

  n--;  // n = 2,  0,1,2

  int i;
  bool skip = false;
  bool inloop = false;
  bool setF = false;

  wxRealPoint rp, rn, rtt, rtpivot, pb, pa;
  wxString portstbd;

  latF = latN[0];
  lonF = lonN[0];

  // Start of new logic
  for (i = 0; i < n; i++) {  // n is number of routepoints

    if (i == 0) {  // First F is a routepoint
                   // save the routepoint

      my_point.lat = wxString::Format(wxT("%f"), latN[i]);
      my_point.lon = wxString::Format(wxT("%f"), lonN[i]);
      my_point.routepoint = 1;
      my_point.viz = "1";
      my_point.wpName = wpName[i];

      my_points.push_back(my_point);
      wpt_num += 1;

    } else {
      if (rad[i] > 0.0) {
        rtt.x = latN[i];
        rtt.y = lonN[i];

        rp.x = latN[i - 1];
        rp.y = lonN[i - 1];

        rn.x = latN[i + 1];
        rn.y = lonN[i + 1];

        // wxString r = wxString::Format("%f", rad[i]);
        // wxMessageBox(r);

      } else {
        my_point.lat = wxString::Format(wxT("%f"), latN[i]);
        my_point.lon = wxString::Format(wxT("%f"), lonN[i]);
        my_point.routepoint = 1;
        my_point.viz = "1";
        my_point.wpName = wpName[i];

        my_points.push_back(my_point);
        wpt_num += 1;
      }
    }
  }
  // print the last routepoint
  my_point.lat = wxString::Format(wxT("%f"), latN[i]);
  my_point.lon = wxString::Format(wxT("%f"), lonN[i]);
  my_point.routepoint = 1;
  my_point.viz = "1";
  my_point.wpName = wpName[i];

  my_points.push_back(my_point);

  for (std::vector<Position>::iterator itOut = my_points.begin();
       itOut != my_points.end(); itOut++) {
    // wxMessageBox((*it).lat, _T("*it.lat"));

    double value, value1;
    if (!(*itOut).lat.ToDouble(&value)) { /* error! */
    }
    lati = value;
    if (!(*itOut).lon.ToDouble(&value1)) { /* error! */
    }
    loni = value1;

    // wxMessageBox(s_radius);

    if ((*itOut).routepoint == 1) {
      Addpoint(Route, wxString::Format(wxT("%f"), lati),
               wxString::Format(wxT("%f"), loni), (*itOut).wpName,
               _T("diamond"), _T("WPT"), (*itOut).viz);
    } else {
      if ((*itOut).routepoint == 0) {
        Addpoint(Route, wxString::Format(wxT("%f"), lati),
                 wxString::Format(wxT("%f"), loni), (*itOut).wpName,
                 _T("square"), _T("WPT"), (*itOut).viz);
      }
    }
  }
  my_points.clear();

  wxString s = "value";
  // check if string ends with .gpx or .GPX
  if (!s.EndsWith(_T(".gpx"))) {
    s = s + _T(".gpx");
  }

  // wxLogMessage("RTZreader: " + s);
  wxCharBuffer buffer = s.ToUTF8();
  if (dbg) std::cout << buffer.data() << std::endl;
  doc.save_file(buffer.data());

  // prevent user making a second export of
  // the same waypoints ?????
  my_positions.clear();
  return 1;
}

void Dlg::OnImport(wxCommandEvent& event) {
  ReadRTZ();
}

void Dlg::OnExport(wxCommandEvent& event) {

   WriteRTZ();
          
}

void Dlg::OnValidate(wxCommandEvent& event) {
  wxFileDialog dlg(this, "Select RTZ file for validation", wxEmptyString,
                   wxEmptyString, "RTZ 1.0 files(*.rtz) | *.rtz;*RTZ",
                   wxFD_OPEN);
  if (dlg.ShowModal() == wxID_OK) {
    if (dlg.GetPath() != wxEmptyString) {
      // wxString show = dlg.GetPath();
      // wxMessageBox(show);

      if (dlg.GetPath().Right(3) == "rtz" || dlg.GetPath().Right(3) == "RTZ") {
        wxString schemaSelection = m_choiceSchema->GetStringSelection();
        if (schemaSelection == wxEmptyString) {
          wxMessageBox("Please select a schema");
          return;
        }

        // wxMessageBox(schemaSelection);

        string xschema, xpath;
        xschema = schemaSelection.mb_str();
        xpath = dlg.GetPath().mb_str();

        ValidateRTZ(xschema, xpath);
      }
    }
  }
  // wxMessageBox("Not yet implemented");
}

void Dlg::OnClose(wxCloseEvent& event) { pPlugIn->OnRTZreaderDialogClose(); }

void Dlg::ReadRTZ() {
  rtz_version = "";
  Position my_position;
  my_positions.clear();

  wxString filename;
  wxFileDialog dlg(this, "Select file", wxEmptyString, wxEmptyString,
                   "RTZ files(*.rtz) | *.rtz;*.RTZ", wxFD_OPEN);
  if (dlg.ShowModal() == wxID_OK) {
    if (dlg.GetPath() != wxEmptyString) {
      filename = dlg.GetPath();
      // wxMessageBox(filename);
    }

  } else
    wxMessageBox(_("No file entered"));

  pugi::xml_document xmlDoc;
  pugi::xml_parse_result result =
      xmlDoc.load_file(filename.mb_str(), parse_default | parse_declaration);

  string rtz_version = xmlDoc.child("route").attribute("version").value();

  if (rtz_version == "1.0") {
    m_choiceSchema->SetSelection(0);
  } else if (rtz_version == "1.1") {
    m_choiceSchema->SetSelection(1);
  } else if (rtz_version == "1.2") {
    m_choiceSchema->SetSelection(2);
  }

  pugi::xml_node pRoot = xmlDoc.child("route").child("routeInfo");
  if (pRoot == nullptr) return;

  wxString error;
  wxProgressDialog* progressdialog = NULL;

  my_position.viz = "1";  // dummy value to make sure there is an entry
  my_position.planned_speed =
      "5";  // dummy value to make sure there is an entry
  my_position.routeName = "na";
  my_position.wpSym = "diamond";

  xml_node pRouteNameElement = xmlDoc.child("route").child("routeInfo");

  if (pRouteNameElement == nullptr) return;

  string route_name = pRouteNameElement.attribute("routeName").value();
  my_position.routeName = route_name;
  bool exists = false;
  // wxMessageBox(s);

  xml_node pWaypointsElement = xmlDoc.child("route").child("waypoints");
  if (pWaypointsElement == nullptr) return;

  xml_node pListWaypointsElement = pWaypointsElement.child("waypoint");
  if (pListWaypointsElement == nullptr) return;

  while (pListWaypointsElement != nullptr) {
    string value = "nullptr";
    value = pListWaypointsElement.attribute("id").value();
    if (value == "nullptr") return;  // must have id
    my_position.wpId = value;
    // wxMessageBox(sti);

    value = pListWaypointsElement.attribute("name").value();
    if (value != "nullptr") {
      my_position.wpName = value;
    } else {
      my_position.wpName = my_position.wpId;  // make name same as id if missing
    }

    xml_node pListPositionElement = pListWaypointsElement.child("position");
    if (pListPositionElement == nullptr) return;
    while (pListPositionElement != nullptr) {
      string stp = pListPositionElement.attribute("lat").value();
      my_position.lat = stp;
      // wxMessageBox(stp);

      string stpl = pListPositionElement.attribute("lon").value();
      my_position.lon = stpl;
      // wxMessageBox(stpl);

      pListPositionElement = pListPositionElement.next_sibling(
          "position");  // stop the loop when position empty
    }

    pListWaypointsElement = pListWaypointsElement.next_sibling(
        "waypoint");  // stop the loop when waypoints empty

    my_positions.push_back(my_position);
  }

  int count = my_positions.size();
  wxString mycount = wxString::Format("%i", count);
  // wxMessageBox(mycount);
  // SetRTZversion(rtz_version);

  if (m_Route->GetValue() != wxEmptyString) route_name = m_Route->GetValue();

  ChartTheRoute(route_name);
}

void Dlg::WriteRTZ() {

  // Select the route from the route table
  wxString route_name = GetRoute();

  // Possible to choose a different default name for the file
  if (m_Route->GetValue() != wxEmptyString) route_name = m_Route->GetValue();

  // put the required RTZ version in the RTZ file
  wxString versn;
  int selection = m_choiceSchema->GetSelection();

  // Create Main level XML container
  xml_document xmlDoc;

  auto declarationNode = xmlDoc.append_child(node_declaration);

  declarationNode.append_attribute("version") = "1.0";

  declarationNode.append_attribute("encoding") = "UTF-8";

  if (selection == 0) {
    declarationNode.append_attribute("standalone") = "no";
  }

  const char* value = "";
  if (selection == 0) {
    value = "http://www.cirm.org/RTZ/1/0";
  } else if (selection == 1) {
    value = "http://www.cirm.org/RTZ/1/1";
  } else if (selection == 2) {
    value = "http://www.cirm.org/RTZ/1/2";
  }

  // Create XML root node called animals
  xml_node pRoot = xmlDoc.append_child("route");

  pRoot.append_attribute("xmlns").set_value(value);
  pRoot.append_attribute("xmlns:xsi")
      .set_value("http://www.w3.org/2001/XMLSchema-instance");

  if (selection == 0) {
    pRoot.append_attribute("xmlns:xsd")
        .set_value("http://www.w3.org/2001/XMLSchema");
    pRoot.append_attribute("version").set_value("1.0");
  } else if (selection == 1) {
    pRoot.append_attribute("version").set_value("1.1");
    pRoot.append_attribute("xmlns:stm")
        .set_value("http://stmvalidation.eu/STM/1/0/0");
  } else if (selection == 2) {
    pRoot.append_attribute("version").set_value("1.2");
  }

  // ************* Add routeInfo to root node *******

  xml_node routeInfo = pRoot.append_child("routeInfo");
  routeInfo.append_attribute("routeName").set_value(route_name.mb_str());

  // Insert cat's name as first child of animal

  // ************* Add waypoints *******
  xml_node waypoints = pRoot.append_child("waypoints");

  int idn = 0;

  for (std::vector<Position>::iterator itOut = my_positions.begin();
       itOut != my_positions.end(); itOut++) {
    xml_node m_waypoint = waypoints.append_child("waypoint");
    wxString myIdn = wxString::Format(wxT("%i"), idn);
    m_waypoint.append_attribute("id").set_value(myIdn.mb_str());
    m_waypoint.append_attribute("name").set_value((*itOut).wpName.mb_str());
    if (selection == 2) m_waypoint.append_attribute("revision").set_value("0");

    xml_node position = m_waypoint.append_child("position");

    position.append_attribute("lat").set_value((*itOut).lat.mb_str());
    position.append_attribute("lon").set_value((*itOut).lon.mb_str());

    idn++;
  }
  // done adding waypoints
  // Write xmlDoc into a file

  wxFileDialog dlg(
      this, _("Save in RTZ format"), wxEmptyString, route_name,
      " RTZ files(*.rtz) | *.rtz;*RTZ | Transas",
      wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  if (dlg.ShowModal() == wxID_OK) {
    if (dlg.GetPath() != wxEmptyString) {
      wxString file_name = dlg.GetPath();

      // Route name must be the same as the file name, without file extension

      int fl = file_name.length();
      wxString rtz_name = file_name.SubString(0, (fl - 5));

      if (route_name != rtz_name) {
        wxMessageBox(_("RTZ file name must be the same as route name"),
                     "Error");
        return;
      }

      xmlDoc.save_file(file_name.mb_str());
      return;
    } else
      return;
  }
}

int Dlg::WriteGPX(wxFileName my_file) {
  if (my_positions.size() == 0) {
    wxMessageBox("No waypoints available for export");
    return -1;
  }

  wxString defaultFileName;
  defaultFileName = this->m_Route->GetValue();

  wxString s = "";

  wxFileDialog dlg(this, _("Export RTZreader Positions in GPX file as"),
                   wxEmptyString, defaultFileName,
                   _T("GPX files (*.gpx)|*.gpx|All files (*.*)|*.*"),
                   wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (dlg.ShowModal() == wxID_CANCEL) {
    // the user changed idea...
    return -1;
  }

  // dlg.ShowModal();
  s = dlg.GetPath();
  if (dlg.GetPath() == wxEmptyString) {
    if (dbg) printf("RTZreader: Empty Path\n");
  }

  // Start GPX
  pugi::xml_document doc;

  // add a custom declaration node
  auto decl = doc.prepend_child();
  decl.append_attribute("version") = "1.0";
  decl.append_attribute("encoding") = "UTF-8";
  decl.append_attribute("standalone") = "no";

  // doc.linkendchild(decl);

  xml_node root = doc.append_child("gpx");
  xml_node Route = root.append_child("rte");
  xml_node RouteName = Route.append_child("name");
  // xml_node text4 = new TiXmlText(this->m_Route->GetValue().ToUTF8());

  root.append_attribute("value") = "version, 1.1";
  root.append_attribute("value") = "creator, RTZreader_pi by Rasbats";
  root.append_attribute("value") =
      "xmlns:xsi, http://www.w3.org/2001/XMLSchema-instance";
  root.append_attribute("value") = "xmlns, http://www.topografix.com/GPX/1/1";
  root.append_attribute("value") =
      "xmlns:gpxx, http://www.garmin.com/xmlschemas/GpxExtensions/v3";
  root.append_attribute("value") =
      "xsi:schemaLocation, http://www.topografix.com/GPX/1/1/gpx.xsd";

  root.append_attribute("value") = "xmlns:opencpn, http://www.opencpn.org";
  RouteName.set_value(m_Route->GetValue());

  xml_node Extensions = Route.append_child("extensions");

  xml_node StartN = Extensions.append_child("opencpn:start");

  xml_node EndN = Extensions.append_child("opencpn:end");

  xml_node Speed = Extensions.append_child("opencpn:planned_speed");

  if (dbg) cout << "RTZreader Calculation\n";
  double speed = 5;
  int interval = 1;

  interval += 1;

  speed = speed * (double)interval;

  int n = 0;

  double lati, loni;
  double latN[100], lonN[100], rad[100];  // allow for up to 100 waypoints
  wxString wpName[100];                   // allow for up to 100 waypoints
  double latF, lonF;

  Position my_point;
  wpt_num = 1;
  double value = 1.0;
  double value1 = 1.0;

  for (std::vector<Position>::iterator it = my_positions.begin();
       it != my_positions.end(); it++) {
    if (!(*it).lat.ToDouble(&value)) { /* error! */
    }
    lati = value;
    if (!(*it).lon.ToDouble(&value1)) { /* error! */
    }
    loni = value1;

    latN[n] = lati;
    lonN[n] = loni;

    wpName[n] = (*it).wpName;

    n++;  // 0,1,2,3
  }

  my_points.clear();

  n--;  // n = 2,  0,1,2

  int i;
  bool skip = false;
  bool inloop = false;
  bool setF = false;

  wxRealPoint rp, rn, rtt, rtpivot, pb, pa;
  wxString portstbd;

  latF = latN[0];
  lonF = lonN[0];

  // Start of new logic
  for (i = 0; i < n; i++) {  // n is number of routepoints

    if (i == 0) {  // First F is a routepoint
                   // save the routepoint

      my_point.lat = wxString::Format(wxT("%f"), latN[i]);
      my_point.lon = wxString::Format(wxT("%f"), lonN[i]);
      my_point.routepoint = 1;
      my_point.viz = "1";
      my_point.wpName = wpName[i];

      my_points.push_back(my_point);
      wpt_num += 1;

    } else {
      if (rad[i] > 0.0) {
        rtt.x = latN[i];
        rtt.y = lonN[i];

        rp.x = latN[i - 1];
        rp.y = lonN[i - 1];

        rn.x = latN[i + 1];
        rn.y = lonN[i + 1];

        // wxString r = wxString::Format("%f", rad[i]);
        // wxMessageBox(r);

      } else {
        my_point.lat = wxString::Format(wxT("%f"), latN[i]);
        my_point.lon = wxString::Format(wxT("%f"), lonN[i]);
        my_point.routepoint = 1;
        my_point.viz = "1";
        my_point.wpName = wpName[i];

        my_points.push_back(my_point);
        wpt_num += 1;
      }
    }
  }
  // print the last routepoint
  my_point.lat = wxString::Format(wxT("%f"), latN[i]);
  my_point.lon = wxString::Format(wxT("%f"), lonN[i]);
  my_point.routepoint = 1;
  my_point.viz = "1";
  my_point.wpName = wpName[i];

  my_points.push_back(my_point);

  for (std::vector<Position>::iterator itOut = my_points.begin();
       itOut != my_points.end(); itOut++) {
    // wxMessageBox((*it).lat, _T("*it.lat"));

    double value, value1;
    if (!(*itOut).lat.ToDouble(&value)) { /* error! */
    }
    lati = value;
    if (!(*itOut).lon.ToDouble(&value1)) { /* error! */
    }
    loni = value1;

    if ((*itOut).routepoint == 1) {
      Addpoint(Route, wxString::Format(wxT("%f"), lati),
               wxString::Format(wxT("%f"), loni), (*itOut).wpName,
               _T("diamond"), _T("WPT"), (*itOut).viz);
    } else {
      if ((*itOut).routepoint == 0) {
        Addpoint(Route, wxString::Format(wxT("%f"), lati),
                 wxString::Format(wxT("%f"), loni), (*itOut).wpName,
                 _T("square"), _T("WPT"), (*itOut).viz);
      }
    }
  }
  my_points.clear();

  // check if string ends with .gpx or .GPX
  if (!s.EndsWith(_T(".gpx"))) {
    s = s + _T(".gpx");
  }

  // wxLogMessage("RTZreader: " + s);
  wxCharBuffer buffer = s.ToUTF8();
  if (dbg) std::cout << buffer.data() << std::endl;
  doc.save_file(buffer.data());

  // prevent user making a second export of
  // the same waypoints ?????
  my_positions.clear();
  return 1;
}

void Dlg::ValidateRTZ(string schema, string rtz) {
  wxString mySchema(schema);
  mySchema += ".xsd";

  wxString tmp_path;
  wxFileName fn;
  tmp_path = GetPluginDataDir("RTZreader_pi");
  fn.SetPath(tmp_path);
  fn.AppendDir("data");
  fn.SetFullName("schema");
  wxString fname = fn.GetFullPath();

  wxString schemaPlus = fname + wxFileName::GetPathSeparator() + mySchema;

  string s_schema = schemaPlus.mb_str();

  int argc = 2;
  const char* args[] = {s_schema.c_str(), rtz.c_str()};
  int error = 0;
  int r = 0;

  wxString startURI = "file:///";

  r = mainValidator(argc, args);

  wxString line = "";

  // wxString r_string = wxString::Format("%i", r);

  if (r != 0) {
    // wxMessageBox("Validation Errors", "myMessage");

    m_textCtrlShowResult->Clear();

    wxString line = "VALIDATION ERRORS FOUND";
    m_textCtrlShowResult->SetValue(line);
  } else {
    line = "No Validation Errors";
    m_textCtrlShowResult->SetValue(line);
  }
}

void Dlg::SetRTZversion(wxString version) {
  if (version == "1.0") {
    m_choiceSchema->SetSelection(0);
  } else if (version == "1.1") {
    m_choiceSchema->SetSelection(1);
  } else if (version == "1.2") {
    m_choiceSchema->SetSelection(2);
  } else
    wxMessageBox("Error finding the schema", "Schema Settings");
}

wxString Dlg::GetRoute() {
  std::vector<std::unique_ptr<PlugIn_Route_Ex>> routes;
  auto uids = GetRouteGUIDArray();
  for (size_t i = 0; i < uids.size(); i++) {
    routes.push_back(std::move(GetRouteEx_Plugin(uids[i])));
  }

  GetRouteDialog RouteDialog(this, -1, _("Route Selection"), wxPoint(200, 200),
                             wxSize(300, 200), wxCAPTION | wxRESIZE_BORDER);

  RouteDialog.dialogText->InsertColumn(0, "", 0, wxLIST_AUTOSIZE);
  RouteDialog.dialogText->SetColumnWidth(0, 290);
  RouteDialog.dialogText->InsertColumn(1, "", 0, wxLIST_AUTOSIZE);
  RouteDialog.dialogText->SetColumnWidth(1, 0);
  RouteDialog.dialogText->DeleteAllItems();

  int in = 0;
  std::vector<std::string> names;
  for (const auto& r : routes) names.push_back(r->m_NameString.ToStdString());

  for (size_t n = 0; n < names.size(); n++) {
    wxString routeName = names[in];

    RouteDialog.dialogText->InsertItem(in, "", -1);
    RouteDialog.dialogText->SetItem(in, 0, routeName);
    in++;
  }

  // ReadNavobj();
  long si = -1;
  long itemIndex = -1;
  // int f = 0;

  wxListItem row_info;
  wxString cell_contents_string = wxEmptyString;
  bool foundRoute = false;

  if (RouteDialog.ShowModal() == wxID_OK) {
    for (;;) {
      itemIndex = RouteDialog.dialogText->GetNextItem(
          itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

      if (itemIndex == -1) break;

      // Got the selected item index
      if (RouteDialog.dialogText->IsSelected(itemIndex)) {
        si = itemIndex;
        foundRoute = true;
        break;
      }
    }

    if (foundRoute) {
      // Set what row it is (m_itemId is a member of the regular
      // wxListCtrl class)
      row_info.m_itemId = si;
      // Set what column of that row we want to query for information.
      row_info.m_col = 0;
      // Set text mask
      row_info.m_mask = wxLIST_MASK_TEXT;

      // Get the info and store it in row_info variable.
      RouteDialog.dialogText->GetItem(row_info);
      // Extract the text out that cell
      cell_contents_string = row_info.m_text;
      rtept initPoint;
      int nextRoutePointIndex = 0;
      bool foundRoute = false;

      for (size_t i = 0; i < uids.size(); i++) {
        thisRoute = GetRouteEx_Plugin(uids[i]);
        // wxMessageBox(thisRoute->m_NameString);
        if (thisRoute->m_NameString == cell_contents_string) {
          foundRoute = true;
          break;
        }
      }
      if (foundRoute) {
        m_bFoundRoute = true;
        // countRoutePoints = thisRoute->pWaypointList->size();
        myList = thisRoute->pWaypointList;

        PlugIn_Waypoint_Ex* myWaypoint;
        theWaypoints.clear();
        my_positions.clear();
        Position my_pos;

        wxPlugin_WaypointExListNode* pwpnode = myList->GetFirst();
        while (pwpnode) {
          myWaypoint = pwpnode->GetData();
          theWaypoints.push_back(myWaypoint);
          pwpnode = pwpnode->GetNext();
        }

        for (size_t n = 0; n < theWaypoints.size(); n++) {
          my_pos.lat = wxString::Format("%f", theWaypoints[n]->m_lat);
          my_pos.lon = wxString::Format("%f", theWaypoints[n]->m_lon);
          my_pos.wpId = theWaypoints[n]->m_GUID;
          my_pos.wpName = theWaypoints[n]->m_MarkName;
          my_positions.push_back(my_pos);
        }
        return (thisRoute->m_NameString);
      } else {
        wxMessageBox("Route not found");
        return (wxEmptyString);
      }
    }
  }
  return (wxEmptyString);
}

void Dlg::ChartTheRoute(wxString myRoute) {
  PlugIn_Route_Ex* newRoute =
      new PlugIn_Route_Ex;  // for adding a route on OpenCPN chart display

  newRoute->m_NameString = myRoute;
  newRoute->m_isVisible = true;

  double lati, loni, value, value1;
  bool m_bNameVisible = true;

  for (std::vector<Position>::iterator itp = my_positions.begin();
       itp != my_positions.end(); itp++) {
    PlugIn_Waypoint_Ex* wayPoint = new PlugIn_Waypoint_Ex;

    wayPoint->m_MarkName = (*itp).wpName;

    if (!(*itp).lat.ToDouble(&value)) { /* error! */
    }
    lati = value;
    if (!(*itp).lon.ToDouble(&value1)) { /* error! */
    }
    loni = value1;

    m_bNameVisible = (*itp).name_visible;

    wayPoint->m_lat = lati;
    wayPoint->m_lon = loni;
    wayPoint->IsVisible = true;
    wayPoint->IsNameVisible = m_bNameVisible;

    wayPoint->IconName = "diamond";

    newRoute->pWaypointList->Append(wayPoint);
  }

  AddPlugInRouteEx(newRoute, true);

  wxMessageBox("Route & Mark Manager will show the imported route",
               "Imported Route");

  GetParent()->Refresh();
}

GetRouteDialog::GetRouteDialog(wxWindow* parent, wxWindowID id,
                               const wxString& title, const wxPoint& position,
                               const wxSize& size, long style)
    : wxDialog(parent, id, title, position, size, style) {
  wxPoint p;
  wxSize sz;

  sz.SetWidth(size.GetWidth() - 20);
  sz.SetHeight(size.GetHeight() - 70);

  p.x = 6;
  p.y = 2;

  dialogText = new wxListView(this, wxID_ANY, p, sz,
                              wxLC_NO_HEADER | wxLC_REPORT | wxLC_SINGLE_SEL,
                              wxDefaultValidator, wxT(""));
  wxFont pVLFont(wxFontInfo(12).FaceName("Arial"));
  dialogText->SetFont(pVLFont);

  auto sizerlist = new wxBoxSizer(wxVERTICAL);
  sizerlist->Add(-1, -1, 100, wxEXPAND);
  sizerlist->Add(dialogText);

  auto sizer = new wxBoxSizer(wxHORIZONTAL);
  auto flags = wxSizerFlags().Bottom().Border();
  sizer->Add(1, 1, 100, wxEXPAND);  // Expanding spacer
  auto cancel = new wxButton(this, wxID_CANCEL, _("Cancel"));
  sizer->Add(cancel, flags);
  auto m_ok = new wxButton(this, wxID_OK, _("OK"));
  m_ok->Enable(true);
  sizer->Add(m_ok, flags);
  sizerlist->Add(sizer);
  SetSizer(sizerlist);
  Fit();
  SetFocus();
};
