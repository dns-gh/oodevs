// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Path.h"
#include "PathPoint.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Point.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;
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
Path::Path( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Common::MsgLocation& message )
    : Parameter< QString >( parameter )
    , converter_( converter )
{
    AddPoints( message );
}

// -----------------------------------------------------------------------------
// Name: Path constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
Path::Path( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : Parameter< QString >( parameter )
    , converter_( converter )
{
    xis >> xml::list( "parameter", *this, &Path::ReadPoint );
}

// -----------------------------------------------------------------------------
// Name: Path constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
Path::Path( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
: Parameter< QString >( OrderParameter( xis.attribute< std::string >( "name" ), "path", false ) )
    , converter_( converter )
{
    xis >> xml::list( "parameter", *this, &Path::ReadPoint );
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
void Path::AddPoints( const Common::MsgLocation& message )
{
    const unsigned int count = message.coordinates().elem_size();
    for( unsigned int i = 0; i < count; ++i )
        AddPoint( converter_.ConvertToXY( message.coordinates().elem( i ) ), i, count );
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
void Path::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        CommitTo( *message.mutable_value()->Add()->mutable_path()->mutable_location() );
}
// -----------------------------------------------------------------------------
// Name: Path::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Path::CommitTo( Common::MsgMissionParameter_Value& message ) const
{
    if( IsSet() )
        CommitTo( *message.mutable_path()->mutable_location() );
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( Common::MsgLocation& message ) : message_( &message ) {}
        virtual void Visit( const PathPoint& param )
        {
            param.CommitTo( *message_->mutable_coordinates()->add_elem() );
        }
        Common::MsgLocation* message_;
    };
}

// -----------------------------------------------------------------------------
// Name: Path::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Path::CommitTo( Common::MsgLocation& message ) const
{
    message.set_type( Common::MsgLocation::line );
    AsnSerializer serializer( message );
    Accept( serializer );
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
