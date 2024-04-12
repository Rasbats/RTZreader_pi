//////////////////////////////////////////////////////////////////////////////
// Name:        SVGPathSegLinetoVerticalRel.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_PATH_SEG_LINETO_VERTICAL_REL_H
#define WX_SVG_PATH_SEG_LINETO_VERTICAL_REL_H

#include "SVGPathSeg.h"

class wxSVGPathSegLinetoVerticalRel : public wxSVGPathSeg {
protected:
  double m_y;

public:
  inline double GetY() const { return m_y; }
  inline void SetY(double n) { m_y = n; }

public:
  wxSVGPathSegLinetoVerticalRel()
      : wxSVGPathSeg(wxPATHSEG_LINETO_VERTICAL_REL), m_y(0) {}
  virtual ~wxSVGPathSegLinetoVerticalRel() {}
};

#endif  // WX_SVG_PATH_SEG_LINETO_VERTICAL_REL_H
