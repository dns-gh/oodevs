// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "ObjectiveList.h"
#include "Objective.h"
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: ObjectiveList constructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ObjectiveList::ObjectiveList( const OrderParameter& parameter )
    : Parameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectiveList constructor
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
ObjectiveList::ObjectiveList( const OrderParameter& parameter, xml::xistream& xis, const CoordinateConverter_ABC& converter )
    : Parameter< QString >( parameter )
{
    xis >> xml::list( "parameter", *this, &ObjectiveList::ReadParameter, converter );
}

// -----------------------------------------------------------------------------
// Name: ObjectiveList constructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ObjectiveList::ObjectiveList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Common::MsgMissionObjectiveList& message )
    : Parameter< QString >( parameter )
{
    for( int i = 0; i < message.elem_size(); ++i )
        AddParameter( *new Objective( OrderParameter( tools::translate( "Parameter", "Objective %1" ).arg( i + 1 ).ascii(), "objective", false ), converter, message.elem().Get(i) ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectiveList destructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ObjectiveList::~ObjectiveList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectiveList::ReadParameter
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ObjectiveList::ReadParameter( xml::xistream& xis, const CoordinateConverter_ABC& converter )
{
    std::string type;
    xis >> xml::attribute( "type", type );
    if( type == "objective" )
        AddParameter( *new Objective( xis, converter ) );
}


namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( Common::MsgMissionObjectiveList& message ) : message_( &message ) {}
        virtual void Visit( const Objective& param )
        {
            param.CommitTo( *message_->add_elem() );
        }
        Common::MsgMissionObjectiveList* message_;
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectiveList::CommitTo
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ObjectiveList::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    Common::MsgMissionObjectiveList* list = message.mutable_value()->mutable_missionobjectivelist();
    if( IsSet() )
    {
        AsnSerializer serializer( *list );
        Accept( serializer );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectiveList::Clean
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ObjectiveList::Clean( Common::MsgMissionParameter& message ) const
{
    if( message.value().has_missionobjectivelist() )
        message.mutable_value()->clear_missionobjectivelist();
}

// -----------------------------------------------------------------------------
// Name: ObjectiveList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool ObjectiveList::IsSet() const
{
    return !elements_.empty(); // $$$$ SBO 2008-03-19: each element must be set as well...
}
