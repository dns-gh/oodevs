// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __FileWrapper_h_
#define __FileWrapper_h_

#include <tools/StdFileWrapper.h>
#include <xeumeuleu/xml.hpp>

// $$$$ ABR 2013-03-13: Multiple warning in xutilily if include xeuseuleu here
//#pragma warning( push, 1 )
//#include <xeuseuleu/xsl.h>
//#pragma warning( pop )

namespace tools
{
    class Path;

    // -----------------------------------------------------------------------------
    // XML
    // -----------------------------------------------------------------------------
    class Xifstream : public xml::xifstream
    {
    public:
        explicit Xifstream( const Path& path, const xml::grammar& grammar = xml::null_grammar() );
        Xifstream( const Path& path, const xml::encoding& encoding, const xml::grammar& grammar = xml::null_grammar() );
        virtual ~Xifstream();
    };

    // -----------------------------------------------------------------------------
    class Xofstream : public xml::xofstream
    {
    public:
        explicit Xofstream( const Path& path, const xml::encoding& encoding = xml::encoding() );
        virtual ~Xofstream();
    };

    // -----------------------------------------------------------------------------
    // XSL
    // -----------------------------------------------------------------------------
    //class Xftransform : public xsl::xftransform
    //{
    //public:
    //    Xftransform( const Path& stylesheet, const Path& filename );
    //    Xftransform( std::istream& stylesheet, const Path& filename );
    //    virtual ~Xftransform();
    //};

    // -----------------------------------------------------------------------------
    //class Xstringtransform : public xsl::xstringtransform
    //{
    //public:
    //    Xstringtransform( const Path& stylesheet );
    //    Xstringtransform( std::istream& stylesheet );
    //    virtual ~Xstringtransform();
    //};
}

#endif // __FileWrapper_h_
