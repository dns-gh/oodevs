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
#include "Path.h"

namespace tools
{

// -----------------------------------------------------------------------------
// STD
// -----------------------------------------------------------------------------
Fstream::Fstream()
{
    // NOTHING
}

Fstream::Fstream( const tools::Path& path, std::ios_base::openmode mode /*= std::ios_base::in | std::ios_base::out*/ )
    : std::fstream( path.ToUnicode().c_str(), mode )
{
    // NOTHING
}

Fstream::~Fstream()
{
    // NOTHING
}

void Fstream::open( const tools::Path& path, std::ios_base::openmode mode /*= std::ios_base::in | std::ios_base::out*/ )
{
    std::fstream::open( path.ToUnicode().c_str(), mode );
}

// -----------------------------------------------------------------------------
Ifstream::Ifstream()
    : std::ifstream()
{
    // NOTHING
}

Ifstream::Ifstream( const tools::Path& path, std::ios_base::openmode mode /* = std::ios_base::in */ )
    : std::ifstream( path.ToUnicode().c_str(), mode )
{
    // NOTHING
}

Ifstream::~Ifstream()
{
    // NOTHING
}

void Ifstream::open( const tools::Path& path, std::ios_base::openmode mode /*= std::ios_base::in*/ )
{
    std::ifstream::open( path.ToUnicode().c_str(), mode );
}

// -----------------------------------------------------------------------------
Ofstream::Ofstream()
    : std::ofstream()
{
    // NOTHING
}

Ofstream::Ofstream( const tools::Path& path, std::ios_base::openmode mode /* = std::ios_base::out */ )
    : std::ofstream( path.ToUnicode().c_str(), mode )
{
    // NOTHING
}

Ofstream::~Ofstream()
{
    // NOTHING
}

void Ofstream::open( const tools::Path& path, std::ios_base::openmode mode /*= std::ios_base::out*/ )
{
    std::ofstream::open( path.ToUnicode().c_str(), mode );
}


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
