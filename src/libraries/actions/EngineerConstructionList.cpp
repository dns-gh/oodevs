// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "EngineerConstructionList.h"
#include "EngineerConstruction.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: EngineerConstructionList constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
EngineerConstructionList::EngineerConstructionList( const OrderParameter& parameter )
    : Parameter< std::string >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EngineerConstructionList constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
EngineerConstructionList::EngineerConstructionList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< ObjectType, std::string >& resolver, const kernel::EntityResolver_ABC& entities, const Common::MsgPlannedWorkList& message, kernel::Controller& controller )
    : Parameter< std::string >( parameter )
{
    for( int i = 0; i < message.elem_size(); ++i )
        AddParameter( *new EngineerConstruction( OrderParameter( tools::translate( "Parameter", "Obstacle %1" ).arg( i + 1 ).ascii(), "obstacle", false ), converter, resolver, entities, message.elem(i), controller ) );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstructionList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
EngineerConstructionList::EngineerConstructionList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< ObjectType, std::string >& resolver, const kernel::EntityResolver_ABC& entities, xml::xistream& xis, kernel::Controller& controller )
    : Parameter< std::string >( parameter )
{
    xis >> xml::list( "parameter", *this, &EngineerConstructionList::ReadEngineerConstruction, converter, resolver, entities, controller );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstructionList destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
EngineerConstructionList::~EngineerConstructionList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::ReadObstacle
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void EngineerConstructionList::ReadEngineerConstruction( xml::xistream& xis, const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< ObjectType, std::string >& resolver, const kernel::EntityResolver_ABC& entities, kernel::Controller& controller )
{
    AddParameter( *new EngineerConstruction( converter, resolver, entities, xis, controller ) );
}


namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( Common::MsgPlannedWorkList& message ) : message_( &message ) {}
        virtual void Visit( const EngineerConstruction& param )
        {
            param.CommitTo( *message_->add_elem() );
        }
        Common::MsgPlannedWorkList* message_;
    };
}

// -----------------------------------------------------------------------------
// Name: EngineerConstructionList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void EngineerConstructionList::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value ( !IsSet() );
    Common::MsgPlannedWorkList* list = message.mutable_value()->mutable_plannedworklist();
    if( IsSet() )
    {
        AsnSerializer serializer( *list );
        Accept( serializer );
    }
}

// -----------------------------------------------------------------------------
// Name: EngineerConstructionList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void EngineerConstructionList::Clean( Common::MsgMissionParameter& message ) const
{
    if( message.value().has_plannedworklist() )
        message.mutable_value()->clear_plannedworklist();
}

// -----------------------------------------------------------------------------
// Name: EngineerConstructionList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void EngineerConstructionList::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    Parameter< std::string >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstructionList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool EngineerConstructionList::IsSet() const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        if( !it->second->IsSet() )
            return false;
    return !elements_.empty();
}
