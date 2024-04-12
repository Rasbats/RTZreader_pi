//////////////////////////////////////////////////////////////////////////////
// Name:        SVGAltGlyphItemElement.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_ALT_GLYPH_ITEM_ELEMENT_H
#define WX_SVG_ALT_GLYPH_ITEM_ELEMENT_H

#include "SVGElement.h"
#include "String_wxsvg.h"
#include "Element.h"

class wxSVGAltGlyphItemElement : public wxSVGElement {
public:
  wxSVGAltGlyphItemElement(wxString tagName = wxT("altGlyphItem"))
      : wxSVGElement(tagName) {}
  virtual ~wxSVGAltGlyphItemElement() {}
  wxSvgXmlNode* CloneNode(bool deep = true) {
    return new wxSVGAltGlyphItemElement(*this);
  }
  bool HasAttribute(const wxString& name) const;
  wxString GetAttribute(const wxString& name) const;
  bool SetAttribute(const wxString& name, const wxString& value);
  wxSvgXmlAttrHash GetAttributes() const;
  virtual wxSVGDTD GetDtd() const { return wxSVG_ALTGLYPHITEM_ELEMENT; }
};

#endif  // WX_SVG_ALT_GLYPH_ITEM_ELEMENT_H
