// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "LocationList.h"
#include "Location.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: LocationList constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
LocationList::LocationList( const OrderParameter& parameter )
    : Parameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationList constructor
// Created: FHD 2009-11-09
// -----------------------------------------------------------------------------
LocationList::LocationList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const Common::MsgLocationList& message )
    : Parameter< QString >( parameter )
{
    for( int i = 0; i < message.elem_size(); ++i )
        AddParameter( *new Location( kernel::OrderParameter( tools::translate( "Parameter", "%1 (item %2)" ).arg( parameter.GetName().c_str() ).arg( i + 1 ).ascii(), "location", false ), converter, message.elem( i ) ) );
}

// -----------------------------------------------------------------------------
// Name: LocationList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
LocationList::LocationList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : Parameter< QString >( parameter )
{
    xis >> xml::list( "parameter", *this, &LocationList::ReadLocation, converter );
}

// -----------------------------------------------------------------------------
// Name: LocationList destructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
LocationList::~LocationList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationList::ReadLocation
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void LocationList::ReadLocation( xml::xistream& xis, const CoordinateConverter_ABC& converter )
{
    AddParameter( *new Location( OrderParameter( xis.attribute< std::string >( "name" ), "location", false ), converter, xis ) );
}

namespace
{
    struct MessageSerializer : public ParameterVisitor_ABC
    {
        explicit MessageSerializer( Common::MsgLocationList& message ) : message_( &message ) {}
        virtual void Visit( const Location& param )
        {
            param.CommitTo( *message_->add_elem() );
        }
        Common::MsgLocationList* message_;
    };
}

// -----------------------------------------------------------------------------
// Name: LocationList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void LocationList::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
    {
        ::MessageSerializer serializer( *message.mutable_value()->mutable_locationlist() );
        Accept( serializer );
    }
}

// -----------------------------------------------------------------------------
// Name: LocationList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void LocationList::Clean( Common::MsgMissionParameter& message ) const
{
    message.mutable_value()->clear_locationlist();
}

// -----------------------------------------------------------------------------
// Name: LocationList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void LocationList::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    Parameter< QString >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: LocationList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool LocationList::IsSet() const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        if( !it->second->IsSet() )
            return false;
    return Parameter< QString >::IsSet();
}
