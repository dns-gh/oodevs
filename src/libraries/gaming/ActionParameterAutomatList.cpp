// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterAutomatList.h"
#include "ActionParameterAutomat.h"
#include "ActionParameterVisitor_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomatList constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAutomatList::ActionParameterAutomatList( const kernel::OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomatList constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAutomatList::ActionParameterAutomatList( const OrderParameter& parameter, const ASN1T_AutomatList& asn, const Resolver_ABC< Automat_ABC >& resolver )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterAutomat( OrderParameter( tools::translate( "ActionParameter", "Automat %1" ).arg( i + 1 ), "automat", false ), asn.elem[i], resolver ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomatList constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAutomatList::ActionParameterAutomatList( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Automat_ABC >& resolver )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterAutomatList::ReadAutomat, resolver );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomatList destructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAutomatList::~ActionParameterAutomatList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomatList::ReadAutomat
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ActionParameterAutomatList::ReadAutomat( xml::xistream& xis, const Resolver_ABC< Automat_ABC >& resolver )
{
    AddParameter( *new ActionParameterAutomat( xis, resolver ) );
}

namespace
{
    struct AsnSerializer : public ActionParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_AutomatList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterAutomat& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_AutomatList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomatList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ActionParameterAutomatList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_automatList;
    ASN1T_AutomatList*& list = asn.value.u.automatList = new ASN1T_AutomatList();
    asn.null_value = ( list->n = Count() ) ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_Automat[list->n];
    AsnSerializer serializer( *list );
    Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomatList::Clean
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ActionParameterAutomatList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.automatList )
        delete[] asn.value.u.automatList->elem;
    delete asn.value.u.automatList;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomatList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void ActionParameterAutomatList::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    ActionParameter< QString >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}
