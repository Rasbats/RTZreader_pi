//////////////////////////////////////////////////////////////////////////////
// Name:        SVGFEBlendElement.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_FE_BLEND_ELEMENT_H
#define WX_SVG_FE_BLEND_ELEMENT_H

#include "SVGElement.h"
#include "SVGFilterPrimitiveStandardAttributes.h"
#include "SVGAnimatedString.h"
#include "SVGAnimatedEnumeration.h"
#include "String_wxsvg.h"
#include "Element.h"
#include "SVGAnimatedType.h"

enum wxSVG_FEBLEND_MODE {
  wxSVG_FEBLEND_MODE_UNKNOWN = 0,
  wxSVG_FEBLEND_MODE_NORMAL = 1,
  wxSVG_FEBLEND_MODE_MULTIPLY = 2,
  wxSVG_FEBLEND_MODE_SCREEN = 3,
  wxSVG_FEBLEND_MODE_DARKEN = 4,
  wxSVG_FEBLEND_MODE_LIGHTEN = 5
};

class wxSVGFEBlendElement : public wxSVGElement,
                            public wxSVGFilterPrimitiveStandardAttributes {
protected:
  wxSVGAnimatedString m_in1;
  wxSVGAnimatedString m_in2;
  wxSVGAnimatedEnumeration m_mode;

public:
  inline const wxSVGAnimatedString& GetIn1() const { return m_in1; }
  inline void SetIn1(const wxSVGAnimatedString& n) { m_in1 = n; }
  inline void SetIn1(const wxString& n) { m_in1.SetBaseVal(n); }

  inline const wxSVGAnimatedString& GetIn2() const { return m_in2; }
  inline void SetIn2(const wxSVGAnimatedString& n) { m_in2 = n; }
  inline void SetIn2(const wxString& n) { m_in2.SetBaseVal(n); }

  inline const wxSVGAnimatedEnumeration& GetMode() const { return m_mode; }
  inline void SetMode(const wxSVGAnimatedEnumeration& n) { m_mode = n; }
  inline void SetMode(unsigned char n) { m_mode.SetBaseVal(n); }

public:
  wxSVGFEBlendElement(wxString tagName = wxT("feBlend"))
      : wxSVGElement(tagName) {}
  virtual ~wxSVGFEBlendElement() {}
  wxSvgXmlNode* CloneNode(bool deep = true) {
    return new wxSVGFEBlendElement(*this);
  }
  bool HasAttribute(const wxString& name) const;
  wxString GetAttribute(const wxString& name) const;
  bool SetAttribute(const wxString& name, const wxString& value);
  wxSvgXmlAttrHash GetAttributes() const;
  bool SetAnimatedValue(const wxString& name, const wxSVGAnimatedType& value);
  virtual wxSVGDTD GetDtd() const { return wxSVG_FEBLEND_ELEMENT; }
};

#endif  // WX_SVG_FE_BLEND_ELEMENT_H
