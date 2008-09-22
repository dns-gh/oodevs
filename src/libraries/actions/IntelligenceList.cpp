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
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;
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
                                                                , const Resolver_ABC< Formation_ABC >& resolver, const FormationLevels& levels )
    : Parameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &IntelligenceList::ReadIntelligence, converter, resolver, levels );
}

// -----------------------------------------------------------------------------
// Name: IntelligenceList constructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
IntelligenceList::IntelligenceList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_IntelligenceList& asn
                                                                , const Resolver_ABC< Formation_ABC >& resolver, const FormationLevels& levels )
    : Parameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new Intelligence( OrderParameter( tools::translate( "Parameter", "Intelligence %1" ).arg( i ).ascii(), "intelligence", true ), converter, resolver, levels, asn.elem[i] ) );
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
void IntelligenceList::ReadIntelligence( xml::xistream& xis, const CoordinateConverter_ABC& converter, const Resolver_ABC< Formation_ABC >& resolver, const FormationLevels& levels  )
{
    AddParameter( *new Intelligence( converter, xis, resolver, levels ) );
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_IntelligenceList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const Intelligence& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }
        ASN1T_IntelligenceList* asn_;
        unsigned int current_;
    };
}


// -----------------------------------------------------------------------------
// Name: IntelligenceList::CommitTo
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void IntelligenceList::CommitTo( ASN1T_MissionParameter& asn ) const
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
    struct AsnCleaner : public ParameterVisitor_ABC
    {
        explicit AsnCleaner( ASN1T_IntelligenceList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const Intelligence& param )
        {
            param.Clean( asn_->elem[current_++] );
        }
        ASN1T_IntelligenceList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: IntelligenceList::Clean
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void IntelligenceList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.intelligenceList )
    {
        AsnCleaner cleaner( *asn.value.u.intelligenceList );
        Accept( cleaner );
        delete[] asn.value.u.intelligenceList;
    }
}

// -----------------------------------------------------------------------------
// Name: IntelligenceList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool IntelligenceList::IsSet() const
{
    return !elements_.empty(); // $$$$ SBO 2008-03-19: each element must be set as well...
}
