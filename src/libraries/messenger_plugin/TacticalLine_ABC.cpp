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
#include "ASN_Messages.h"
#include "clients_kernel/CoordinateConverter.h"

#include <xeumeuleu/xml.h>

using namespace messenger;

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: SBO 2006-11-15
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( unsigned int id, const ASN1T_TacticalLine& asn ) :
      id_       ( id )
    , strName_  ( asn.name )
    , diffusion_( asn.diffusion)
{
    UpdateGeometry(asn.geometry);
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: RDS 2008-04-03
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( unsigned int id, xml::xistream& xis, const ASN1T_TacticalLinesDiffusion& diffusion, const kernel::CoordinateConverter_ABC& converter) :
      id_       ( id ),
      diffusion_( diffusion)
{
    xis >> xml::attribute( "name", strName_ )
        >> xml::list( "point", *this, &TacticalLine_ABC::ReadPoint, geometry_, converter);
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
void TacticalLine_ABC::ReadPoint( xml::xistream& xis, T_PositionVector& points, const kernel::CoordinateConverter_ABC& converter) const
{
    std::string mgrs;
    xis >> mgrs;
    ASN1T_CoordLatLong geo ;
    geometry::Point2d pos = converter.ConvertToGeo(converter.ConvertToXY(mgrs));
    geo.latitude = pos.Y();
    geo.longitude = pos.X();
    points.push_back( geo );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Update
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Update( const ASN1T_TacticalLine& asn )
{
    strName_ = asn.name;
    diffusion_ = asn.diffusion;
    UpdateGeometry( asn.geometry );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Send
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Send( ASN1T_Line& asn) const
{
    asn.type             = EnumLocationType::line ;
    asn.coordinates.n    = geometry_.size();
    asn.coordinates.elem = geometry_.size() ? const_cast< ASN1T_CoordLatLong* >( & geometry_.front() ) : 0;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Send
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Send( ASN1T_TacticalLine& asn ) const
{
    asn.name = strName_.c_str();
    asn.diffusion = diffusion_ ;
    Send( asn.geometry );
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
void TacticalLine_ABC::UpdateGeometry(const ASN1T_Location& asn )
{
    geometry_.resize( 0 );
    std::copy( asn.coordinates.elem, asn.coordinates.elem + asn.coordinates.n, std::back_inserter( geometry_ ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Write
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Write( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter ) const
{
    xos << xml::attribute( "id",   id_ )
        << xml::attribute( "name", strName_ );
    for( CIT_PositionVector it = geometry_.begin(); it != geometry_.end(); ++it )
        WritePoint( xos, converter, *it );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::WritePoint
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void TacticalLine_ABC::WritePoint( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter, const ASN1T_CoordLatLong& point ) const
{
    geometry::Point2d pos( point.longitude, point.latitude );
    xos << xml::start( "point" )
            << converter.ConvertToMgrs( converter.ConvertFromGeo( pos ) )
        << xml::end();
}
