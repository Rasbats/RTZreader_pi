//////////////////////////////////////////////////////////////////////////////
// Name:        SVGFontElement.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_FONT_ELEMENT_H
#define WX_SVG_FONT_ELEMENT_H

#include "SVGElement.h"
#include "SVGExternalResourcesRequired.h"
#include "SVGStylable.h"
#include "String_wxsvg.h"
#include "Element.h"
#include "SVGAnimatedType.h"

class wxSVGFontElement : public wxSVGElement,
                         public wxSVGExternalResourcesRequired,
                         public wxSVGStylable {
public:
  wxSVGFontElement(wxString tagName = wxT("font")) : wxSVGElement(tagName) {}
  virtual ~wxSVGFontElement() {}
  wxSvgXmlNode* CloneNode(bool deep = true) {
    return new wxSVGFontElement(*this);
  }
  bool HasAttribute(const wxString& name) const;
  wxString GetAttribute(const wxString& name) const;
  bool SetAttribute(const wxString& name, const wxString& value);
  wxSvgXmlAttrHash GetAttributes() const;
  bool SetAnimatedValue(const wxString& name, const wxSVGAnimatedType& value);
  virtual wxSVGDTD GetDtd() const { return wxSVG_FONT_ELEMENT; }
};

#endif  // WX_SVG_FONT_ELEMENT_H
