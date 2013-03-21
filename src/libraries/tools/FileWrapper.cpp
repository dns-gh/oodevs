// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "FileWrapper.h"
#include <tools/Path.h>

namespace tools
{

// -----------------------------------------------------------------------------
// XML
// -----------------------------------------------------------------------------
Xifstream::Xifstream( const tools::Path& path, const xml::grammar& grammar /* = xml::null_grammar() */ )
    : xml::xifstream( path.ToUTF8().c_str(), grammar )
{
    // NOTHING
}

Xifstream::Xifstream( const tools::Path& path, const xml::encoding& encoding, const xml::grammar& grammar /* = xml::null_grammar() */ )
    : xml::xifstream( path.ToUTF8().c_str(), encoding, grammar )
{
    // NOTHING
}

Xifstream::~Xifstream()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
Xofstream::Xofstream( const tools::Path& path, const xml::encoding& encoding /* = xml::encoding() */ )
    : xml::xofstream( path.ToUTF8().c_str(), encoding )
{
    // NOTHING
}

Xofstream::~Xofstream()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// XSL
// -----------------------------------------------------------------------------
//Xftransform::Xftransform( const Path& stylesheet, const Path& filename )
//    : xsl::xftransform( stylesheet.ToUTF8(), filename.ToUTF8() )
//{
//    // NOTHING
//}
//
//Xftransform::Xftransform( std::istream& stylesheet, const Path& filename )
//    : xsl::xftransform( stylesheet, filename.ToUTF8() )
//{
//    // NOTHING
//}
//
//Xftransform::~Xftransform()
//{
//    // NOTHING
//}
//
//// -----------------------------------------------------------------------------
//Xstringtransform::Xstringtransform( const Path& stylesheet )
//    : xsl::xstringtransform( stylesheet.ToUTF8() )
//{
//    // NOTHING
//}
//
//Xstringtransform::Xstringtransform( std::istream& stylesheet )
//    : xsl::xstringtransform( stylesheet )
//{
//    // NOTHING
//}
//
//Xstringtransform::~Xstringtransform()
//{
//    // NOTHING
//}

} //! namespace tools
