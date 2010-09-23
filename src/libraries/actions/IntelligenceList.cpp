// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "IntelligenceList.h"
#include "Intelligence.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: IntelligenceList constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
IntelligenceList::IntelligenceList( const kernel::OrderParameter& parameter )
    : Parameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligenceList constructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
IntelligenceList::IntelligenceList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis
                                                                , const kernel::EntityResolver_ABC& resolver, const FormationLevels& levels, kernel::Controller& controller )
    : Parameter< QString >( parameter )
{
    xis >> xml::list( "parameter", *this, &IntelligenceList::ReadIntelligence, converter, resolver, levels, controller );
}

// -----------------------------------------------------------------------------
// Name: IntelligenceList constructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------

IntelligenceList::IntelligenceList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Common::MsgIntelligenceList& message
                                   , const kernel::EntityResolver_ABC& resolver, const FormationLevels& levels, kernel::Controller& controller )
    : Parameter< QString >( parameter )
{
    for( int i = 0; i < message.elem_size(); ++i )
        AddParameter( *new Intelligence( OrderParameter( tools::translate( "Parameter", "%1 (item %2)" ).arg( parameter.GetName().c_str() ).arg( i + 1 ).ascii(), "intelligence", true ), converter, resolver, levels, message.elem(i), controller ) );
}

// -----------------------------------------------------------------------------
// Name: IntelligenceList destructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
IntelligenceList::~IntelligenceList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligenceList::ReadIntelligence
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void IntelligenceList::ReadIntelligence( xml::xistream& xis, const CoordinateConverter_ABC& converter, const kernel::EntityResolver_ABC& resolver, const FormationLevels& levels, kernel::Controller& controller )
{
    AddParameter( *new Intelligence( converter, xis, resolver, levels, controller ) );
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( Common::MsgIntelligenceList& message ) : message_( &message ) {}
        virtual void Visit( const Intelligence& param )
        {
            param.CommitTo( *message_->add_elem() );
        }
        Common::MsgIntelligenceList* message_;
    };
}


// -----------------------------------------------------------------------------
// Name: IntelligenceList::CommitTo
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void IntelligenceList::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
    {
        AsnSerializer serializer( *message.mutable_value()->mutable_intelligencelist() );
        Accept( serializer );
    }
}

// -----------------------------------------------------------------------------
// Name: IntelligenceList::Clean
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void IntelligenceList::Clean( Common::MsgMissionParameter& message ) const
{
    message.mutable_value()->clear_intelligencelist();
}

// -----------------------------------------------------------------------------
// Name: IntelligenceList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool IntelligenceList::IsSet() const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        if( !it->second->IsSet() )
            return false;
    return( Parameter< QString >::IsSet() && IsInRange() );
}
