//////////////////////////////////////////////////////////////////////////////
// Name:        SVGAnimateMotionElement.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_ANIMATE_MOTION_ELEMENT_H
#define WX_SVG_ANIMATE_MOTION_ELEMENT_H

#include "SVGAnimationElement.h"
#include "String_wxsvg.h"
#include "Element.h"

class wxSVGAnimateMotionElement : public wxSVGAnimationElement {
public:
  wxSVGAnimateMotionElement(wxString tagName = wxT("animateMotion"))
      : wxSVGAnimationElement(tagName) {}
  virtual ~wxSVGAnimateMotionElement() {}
  wxSvgXmlNode* CloneNode(bool deep = true) {
    return new wxSVGAnimateMotionElement(*this);
  }
  virtual void ApplyAnimation();
  bool HasAttribute(const wxString& name) const;
  wxString GetAttribute(const wxString& name) const;
  bool SetAttribute(const wxString& name, const wxString& value);
  wxSvgXmlAttrHash GetAttributes() const;
  virtual wxSVGDTD GetDtd() const { return wxSVG_ANIMATEMOTION_ELEMENT; }
};

#endif  // WX_SVG_ANIMATE_MOTION_ELEMENT_H
