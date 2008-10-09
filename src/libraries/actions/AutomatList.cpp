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
#include "xeumeuleu/xml.h"

using namespace xml;
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
AutomatList::AutomatList( const OrderParameter& parameter, const ASN1T_AutomatList& asn, const Resolver_ABC< Automat_ABC >& resolver, kernel::Controller& controller )
    : Parameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new Automat( OrderParameter( tools::translate( "Parameter", "Automat %1" ).arg( i + 1 ).ascii(), "automat", false ), asn.elem[i], resolver, controller ) );
}

// -----------------------------------------------------------------------------
// Name: AutomatList constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
AutomatList::AutomatList( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Automat_ABC >& resolver, kernel::Controller& controller )
    : Parameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &AutomatList::ReadAutomat, resolver, controller );
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
void AutomatList::ReadAutomat( xml::xistream& xis, const Resolver_ABC< Automat_ABC >& resolver, kernel::Controller& controller )
{
    AddParameter( *new Automat( xis, resolver, controller ) );
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_AutomatList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const Automat& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_AutomatList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: AutomatList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void AutomatList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_automatList;
    ASN1T_AutomatList*& list = asn.value.u.automatList = new ASN1T_AutomatList();
    list->n = Count();
    if( IsSet() )
    {
        list->elem = new ASN1T_Automat[list->n];
        AsnSerializer serializer( *list );
        Accept( serializer );
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatList::Clean
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void AutomatList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.automatList )
        delete[] asn.value.u.automatList->elem;
    delete asn.value.u.automatList;
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
    return !elements_.empty(); // $$$$ SBO 2008-03-19: each element must be set as well...
}
