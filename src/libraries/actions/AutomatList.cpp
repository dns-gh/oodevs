// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "AutomatList.h"
#include "Automat.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: AutomatList constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
AutomatList::AutomatList( const kernel::OrderParameter& parameter )
    : Parameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatList constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
AutomatList::AutomatList( const OrderParameter& parameter, const Common::AutomatIdList& message, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Parameter< QString >( parameter )
{
    for( int i = 0; i < message.elem_size(); ++i )
        AddParameter( *new Automat( OrderParameter( tools::translate( "Parameter", "%1 (item %2)" ).arg( parameter.GetName().c_str() ).arg( i + 1 ).ascii(), "automat", false ), message.elem(i).id(), resolver, controller ) );
}

// -----------------------------------------------------------------------------
// Name: AutomatList constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
AutomatList::AutomatList( const OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Parameter< QString >( parameter )
{
    xis >> xml::list( "parameter", *this, &AutomatList::ReadAutomat, resolver, controller );
}

// -----------------------------------------------------------------------------
// Name: AutomatList destructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
AutomatList::~AutomatList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatList::ReadAutomat
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void AutomatList::ReadAutomat( xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
{
    AddParameter( *new Automat( xis, resolver, controller ) );
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( Common::AutomatIdList& message ) : message_( &message ) {}
        virtual void Visit( const Automat& param )
        {
            param.CommitTo( *message_->add_elem() );
        }
        Common::AutomatIdList* message_;
    };
}

// -----------------------------------------------------------------------------
// Name: AutomatList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void AutomatList::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
    {
        AsnSerializer serializer( *message.mutable_value()->mutable_automatlist() );
        Accept( serializer );
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatList::Clean
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void AutomatList::Clean( Common::MsgMissionParameter& message ) const
{
    if( message.value().has_automatlist() )
        message.mutable_value()->clear_automatlist();
}

// -----------------------------------------------------------------------------
// Name: AutomatList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void AutomatList::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    Parameter< QString >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: AutomatList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool AutomatList::IsSet() const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        if( !it->second->IsSet() )
            return false;
    return !elements_.empty();
}
