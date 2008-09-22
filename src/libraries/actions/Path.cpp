// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Path.h"
#include "PathPoint.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Point.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Path constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
Path::Path( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : Parameter< QString >( parameter )
    , converter_( converter )
{
    location.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: Path constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
Path::Path( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_Location& asn )
    : Parameter< QString >( parameter )
    , converter_( converter )
{
    AddPoints( asn );
}

// -----------------------------------------------------------------------------
// Name: Path constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
Path::Path( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : Parameter< QString >( parameter )
    , converter_( converter )
{
    xis >> list( "parameter", *this, &Path::ReadPoint );
}

// -----------------------------------------------------------------------------
// Name: Path constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
Path::Path( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
: Parameter< QString >( OrderParameter( attribute< std::string >( xis, "name" ), "path", false ) )
    , converter_( converter )
{
    xis >> list( "parameter", *this, &Path::ReadPoint );
}

// -----------------------------------------------------------------------------
// Name: Path destructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
Path::~Path()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Path::AddPoints
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void Path::AddPoints( const ASN1T_Location& asn )
{
    for( unsigned int i = 0; i < asn.coordinates.n; ++i )
        AddPoint( converter_.ConvertToXY( asn.coordinates.elem[i] ), i, asn.coordinates.n );
}

// -----------------------------------------------------------------------------
// Name: Path::VisitLines
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void Path::VisitLines( const T_PointVector& points )
{
    unsigned int i = 0;
    for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
        AddPoint( *it, i++, points.size() );
}

// -----------------------------------------------------------------------------
// Name: Path::VisitPath
// Created: AGE 2008-06-26
// -----------------------------------------------------------------------------
void Path::VisitPath( const geometry::Point2f& , const T_PointVector& points )
{
    VisitLines( points );
}

// -----------------------------------------------------------------------------
// Name: Path::AddPoint
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void Path::AddPoint( const geometry::Point2f& p, unsigned i, unsigned count )
{
    Point pt; pt.AddPoint( p );
    QString label;
    if( i + 1 == count )
        label = tools::translate( "Parameter", "Destination" );
    else
        label = tools::translate( "Parameter", "Way point %1" ).arg( i + 1 );
    AddParameter( *new PathPoint( OrderParameter( label.ascii(), "pathpoint", false ), converter_, pt ) );
}

// -----------------------------------------------------------------------------
// Name: Path::ReadPoint
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void Path::ReadPoint( xml::xistream& xis )
{
    AddParameter( *new PathPoint( xis, converter_ ) );
}

// -----------------------------------------------------------------------------
// Name: Path::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Path::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_path;
    ASN1T_Path*& path = asn.value.u.path = new ASN1T_Path();
    if( IsSet() )
        CommitTo( *path );
}

// -----------------------------------------------------------------------------
// Name: Path::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Path::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.path )
        Clean( *asn.value.u.path );
    delete asn.value.u.path;
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_Path& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const PathPoint& param )
        {
            param.CommitTo( asn_->coordinates.elem[current_++] );
        }

        ASN1T_Path* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: Path::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Path::CommitTo( ASN1T_Path& asn ) const
{
    asn.type = EnumLocationType::line;
    asn.coordinates.n = Count();
    asn.coordinates.elem = new ASN1T_CoordLatLong[asn.coordinates.n];
    AsnSerializer serializer( asn );
    Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: Path::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Path::Clean( ASN1T_Path& asn ) const
{
    if( asn.coordinates.n )
        delete[] asn.coordinates.elem;
}

// -----------------------------------------------------------------------------
// Name: Path::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Path::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    Parameter< QString >::Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: Path::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void Path::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    Parameter< QString >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Path::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool Path::IsSet() const
{
    return !elements_.empty();
}
