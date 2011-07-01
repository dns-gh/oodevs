// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "TacticalLine_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::messenger;

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: SBO 2006-11-15
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( unsigned int id, const sword::TacticalLine& asn )
    : id_       ( id )
    , strName_  ( asn.name() )
    , diffusion_( asn.diffusion())
{
    UpdateGeometry( asn.geometry() );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: RDS 2008-04-03
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( unsigned int id, xml::xistream& xis, const sword::Diffusion& diffusion, const kernel::CoordinateConverter_ABC& converter)
    : id_       ( id ),
      diffusion_( diffusion )
{
    xis >> xml::attribute( "name", strName_ )
        >> xml::list( "point", *this, &TacticalLine_ABC::ReadPoint, geometry_, converter );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC destructor
// Created: SBO 2006-11-15
// -----------------------------------------------------------------------------
TacticalLine_ABC::~TacticalLine_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::ReadPoint
// Created: RDS 2008-04-08
// -----------------------------------------------------------------------------
void TacticalLine_ABC::ReadPoint( xml::xistream& xis, T_PositionVector& points, const kernel::CoordinateConverter_ABC& converter ) const
{
    std::string mgrs;
    xis >> mgrs;
    sword::CoordLatLong geo ;
    geometry::Point2d pos = converter.ConvertToGeo( converter.ConvertToXY( mgrs ) );
    geo.set_latitude( pos.Y() );
    geo.set_longitude( pos.X() );
    points.push_back( geo );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Update
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Update( const sword::TacticalLine& asn )
{
    strName_ = asn.name();
    diffusion_ = asn.diffusion();
    UpdateGeometry( asn.geometry() );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Send
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Send( sword::Location& asn ) const
{
    asn.set_type( sword::Location::line );
    for( T_PositionVector::const_iterator it = geometry_.begin(); it != geometry_.end(); ++it )
        *asn.mutable_coordinates()->add_elem() = *it;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Send
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Send( sword::TacticalLine& asn ) const
{
    asn.set_name ( strName_.c_str() );
    *asn.mutable_diffusion() = diffusion_;
    Send( *asn.mutable_geometry() );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::GetID
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
unsigned int TacticalLine_ABC::GetID() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::UpdateGeometry
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
void TacticalLine_ABC::UpdateGeometry( const sword::Location& asn )
{
    geometry_.resize( 0 );
    std::copy( asn.coordinates().elem().begin(), asn.coordinates().elem().end(), std::back_inserter( geometry_ ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Write
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Write( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter ) const
{
    xos << xml::attribute( "name", strName_ );
    for( CIT_PositionVector it = geometry_.begin(); it != geometry_.end(); ++it )
        WritePoint( xos, converter, *it );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::WritePoint
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void TacticalLine_ABC::WritePoint( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter, const sword::CoordLatLong& point ) const
{
    geometry::Point2d pos( point.longitude(), point.latitude() );
    xos << xml::start( "point" )
            << converter.ConvertToMgrs( converter.ConvertFromGeo( pos ) )
        << xml::end;
}
