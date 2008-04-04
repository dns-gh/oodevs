// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterPath.h"
#include "ActionParameterPathPoint.h"
#include "ActionParameterVisitor_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Point.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : ActionParameter< QString >( parameter )
    , converter_( converter )
{
    location.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_Location& asn )
    : ActionParameter< QString >( parameter )
    , converter_( converter )
{
    AddPoints( asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
    , converter_( converter )
{
    xis >> list( "parameter", *this, &ActionParameterPath::ReadPoint );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
: ActionParameter< QString >( OrderParameter( attribute< std::string >( xis, "name" ), "path", false ) )
    , converter_( converter )
{
    xis >> list( "parameter", *this, &ActionParameterPath::ReadPoint );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath destructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::~ActionParameterPath()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::AddPoints
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionParameterPath::AddPoints( const ASN1T_Location& asn )
{
    for( unsigned int i = 0; i < asn.coordinates.n; ++i )
        AddPoint( converter_.ConvertToXY( asn.coordinates.elem[i] ), i, asn.coordinates.n );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::VisitLines
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionParameterPath::VisitLines( const T_PointVector& points )
{
    unsigned int i = 0;
    for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
        AddPoint( *it, i++, points.size() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::AddPoint
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void ActionParameterPath::AddPoint( const geometry::Point2f& p, unsigned i, unsigned count )
{
    Point pt; pt.AddPoint( p );
    QString label;
    if( i + 1 == count )
        label = tools::translate( "ActionParameter", "Destination" );
    else
        label = tools::translate( "ActionParameter", "Way point %1" ).arg( i + 1 );
    AddParameter( *new ActionParameterPathPoint( OrderParameter( label.ascii(), "pathpoint", false ), converter_, pt ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::ReadPoint
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionParameterPath::ReadPoint( xml::xistream& xis )
{
    AddParameter( *new ActionParameterPathPoint( xis, converter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterPath::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_path;
    ASN1T_Path*& path = asn.value.u.path = new ASN1T_Path();
    if( IsSet() )
        CommitTo( *path );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterPath::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.path )
        Clean( *asn.value.u.path );
    delete asn.value.u.path;
}

namespace
{
    struct AsnSerializer : public ActionParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_Path& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterPathPoint& param )
        {
            param.CommitTo( asn_->coordinates.elem[current_++] );
        }

        ASN1T_Path* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPath::CommitTo( ASN1T_Path& asn ) const
{
    asn.type = EnumLocationType::line;
    asn.coordinates.n = Count();
    asn.coordinates.elem = new ASN1T_CoordLatLong[asn.coordinates.n];
    AsnSerializer serializer( asn );
    Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPath::Clean( ASN1T_Path& asn ) const
{
    if( asn.coordinates.n )
        delete[] asn.coordinates.elem;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPath::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    ActionParameter< QString >::Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void ActionParameterPath::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    ActionParameter< QString >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool ActionParameterPath::IsSet() const
{
    return !elements_.empty();
}
