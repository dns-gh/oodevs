// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __WikiXmlConverter_h_
#define __WikiXmlConverter_h_

#include <boost/noncopyable.hpp>
#include <xeumeuleu/xml.hpp>
#include <string>

void FromWikiToXml( xml::xostream& xos, const std::string& text );
void FromXmlToWiki( xml::xistream& xis, std::string& text );

#endif // __WikiXmlConverter_h_
