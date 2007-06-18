// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAgentList.h"
#include "ParamAgent.h"
#include "ParamVisitor_ABC.h"
#include "gaming/ActionParameterAgentList.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/OrderParameter.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAgentList constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAgentList::ParamAgentList( QObject* parent, const OrderParameter& parameter, ActionController& controller )
    : EntityListParameter< Agent_ABC >( parent, parameter, controller )
    , parameter_( parameter )
    , count_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ParamAgentList destructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAgentList::~ParamAgentList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::AddToMenu
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAgentList::AddToMenu( kernel::ContextMenu& menu )
{
    MakeMenu( tr( "Add agent" ), menu );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::CreateElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityParameter< kernel::Agent_ABC >* ParamAgentList::CreateElement( const kernel::Agent_ABC& potential )
{
    return new ParamAgent( this, tr( "Agent %1" ).arg( ++count_ ), potential );
}

namespace
{
    struct AsnSerializer : public ParamVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_UnitList& list ) : list_( list ), index_( 0 ) {}
        virtual void Visit( const Param_ABC& param )
        {
            if( index_ < list_.n )
                static_cast< const ParamAgent& >( param ).CommitTo( list_.elem[index_++] );
        }

        ASN1T_UnitList& list_;
        unsigned int index_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAgentList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    ASN1T_UnitList*& list = asn.value.u.unitList = new ASN1T_UnitList();
    asn.value.t = T_MissionParameter_value_unitList;
    list->n = Count();
    asn.null_value = list->n ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_Unit[ list->n ];
    AsnSerializer serializer( *list );
    Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAgentList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.unitList )
        delete[] asn.value.u.unitList->elem;
    delete asn.value.u.unitList;
}

namespace
{
    struct ActionSerializer : public ParamVisitor_ABC
    {
        explicit ActionSerializer( ActionParameter_ABC& parent ) : parent_( parent ) {}
        virtual void Visit( const Param_ABC& param )
        {
            static_cast< const ParamAgent& >( param ).CommitTo( parent_ );
        }

        ActionParameter_ABC& parent_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAgentList::CommitTo( Action_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterAgentList( parameter_ ) );
    ActionSerializer serializer( *param );
    Accept( serializer );
    action.AddParameter( *param.release() );
}
