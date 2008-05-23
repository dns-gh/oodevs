// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "PointList.h"
#include "Point.h"
#include <xeumeuleu/xml.h>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: PointList constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
PointList::PointList( xml::xistream& xis )
{
    xis >> xml::start( "jc3iedm:PointList" )
            >> xml::list( "jc3iedm:Point", *this, &PointList::ReadPoint )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: PointList destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
PointList::~PointList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointList::ReadPoint
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void PointList::ReadPoint( xml::xistream& xis )
{
    unsigned int index = 0;
    xis >> xml::content( "jc3iedm:SequenceOrdinal", index );
    if( index == 0 )
        throw std::runtime_error( __FUNCTION__ ": jc3iedm:SequenceOrdinal out of range [1 +inf)" );
    if( index + 1 > points_.size() )
        points_.resize( index );
    points_[--index] = Point( xis );
}

// -----------------------------------------------------------------------------
// Name: PointList::Serialize
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void PointList::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "jc3iedm:PointList" );
    unsigned int i = 1;
    for( T_Points::const_iterator it = points_.begin(); it != points_.end(); ++it, ++i )
    {
        xos << xml::content( "jc3iedm:SequenceOrdinal", i );
        it->Serialize( xos );
    }
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: PointList::Serialize
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void PointList::Serialize( ASN1T_CoordLatLongList& asn ) const
{
    asn.n = points_.size();
    if( asn.n > 0 )
        asn.elem = new ASN1T_CoordLatLong[asn.n];
    unsigned int i = 0;
    for( T_Points::const_iterator it = points_.begin(); it != points_.end(); ++it, ++i )
        it->Serialize( asn.elem[i] );
}

// -----------------------------------------------------------------------------
// Name: PointList::Clean
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void PointList::Clean( ASN1T_CoordLatLongList& asn ) const
{
    delete[] asn.elem;
}
