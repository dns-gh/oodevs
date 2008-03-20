// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterIntelligenceList.h"
#include "ActionParameterIntelligence.h"
#include "ActionParameterVisitor_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligenceList constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
ActionParameterIntelligenceList::ActionParameterIntelligenceList( const kernel::OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligenceList constructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
ActionParameterIntelligenceList::ActionParameterIntelligenceList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis
                                                                , const Resolver_ABC< Formation_ABC >& resolver, const FormationLevels& levels )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterIntelligenceList::ReadIntelligence, converter, resolver, levels );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligenceList constructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
ActionParameterIntelligenceList::ActionParameterIntelligenceList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_IntelligenceList& asn
                                                                , const Resolver_ABC< Formation_ABC >& resolver, const FormationLevels& levels )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterIntelligence( OrderParameter( tools::translate( "ActionParameter", "Intelligence %1" ).arg( i ).ascii(), "intelligence", true ), converter, resolver, levels, asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligenceList destructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
ActionParameterIntelligenceList::~ActionParameterIntelligenceList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligenceList::ReadIntelligence
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void ActionParameterIntelligenceList::ReadIntelligence( xml::xistream& xis, const CoordinateConverter_ABC& converter, const Resolver_ABC< Formation_ABC >& resolver, const FormationLevels& levels  )
{
    AddParameter( *new ActionParameterIntelligence( converter, xis, resolver, levels ) );
}

namespace
{
    struct AsnSerializer : public ActionParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_IntelligenceList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterIntelligence& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }
        ASN1T_IntelligenceList* asn_;
        unsigned int current_;
    };
}


// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligenceList::CommitTo
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void ActionParameterIntelligenceList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_intelligenceList;
    ASN1T_IntelligenceList*& list = asn.value.u.intelligenceList = new ASN1T_IntelligenceList();
    list->n = Count();
    if( IsSet() )
    {
        list->elem = new ASN1T_Intelligence[list->n];
        AsnSerializer serializer( *list );
        Accept( serializer );
    }
}

namespace
{
    struct AsnCleaner : public ActionParameterVisitor_ABC
    {
        explicit AsnCleaner( ASN1T_IntelligenceList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterIntelligence& param )
        {
            param.Clean( asn_->elem[current_++] );
        }
        ASN1T_IntelligenceList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligenceList::Clean
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void ActionParameterIntelligenceList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.intelligenceList )
    {
        AsnCleaner cleaner( *asn.value.u.intelligenceList );
        Accept( cleaner );
        delete[] asn.value.u.intelligenceList;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligenceList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool ActionParameterIntelligenceList::IsSet() const
{
    return !elements_.empty(); // $$$$ SBO 2008-03-19: each element must be set as well...
}
