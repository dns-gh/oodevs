// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterObjectiveList.h"
#include "ActionParameterObjective.h"
#include "ActionParameterVisitor_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectiveList constructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ActionParameterObjectiveList::ActionParameterObjectiveList( const OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectiveList constructor
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
ActionParameterObjectiveList::ActionParameterObjectiveList( const OrderParameter& parameter, xml::xistream& xis, const CoordinateConverter_ABC& converter )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterObjectiveList::ReadParameter, converter );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectiveList constructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ActionParameterObjectiveList::ActionParameterObjectiveList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_MissionObjectiveList& asn )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterObjective( OrderParameter( tools::translate( "ActionParameter", "Objective %1" ).arg( i + 1 ).ascii(), "objective", false ), converter, asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectiveList destructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ActionParameterObjectiveList::~ActionParameterObjectiveList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectiveList::ReadParameter
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterObjectiveList::ReadParameter( xml::xistream& xis, const CoordinateConverter_ABC& converter )
{
    std::string type;
    xis >> attribute( "type", type );
    if( type == "objective" )
        AddParameter( *new ActionParameterObjective( xis, converter ) );
}


namespace
{
    struct AsnSerializer : public ActionParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_MissionObjectiveList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterObjective& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_MissionObjectiveList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectiveList::CommitTo
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterObjectiveList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    ASN1T_MissionObjectiveList*& list = asn.value.u.missionObjectiveList = new ASN1T_MissionObjectiveList();
    asn.value.t = T_MissionParameter_value_missionObjectiveList;
    list->n = Count();
    if( IsSet() )
    {
        list->elem = new ASN1T_MissionObjective[list->n];
        AsnSerializer serializer( *list );
        Accept( serializer );
    }
}

namespace
{
    struct AsnCleaner : public ActionParameterVisitor_ABC
    {
        explicit AsnCleaner( ASN1T_MissionObjectiveList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterObjective& param )
        {
            param.Clean( asn_->elem[current_++] );
        }
   
        ASN1T_MissionObjectiveList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectiveList::Clean
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterObjectiveList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.missionObjectiveList )
    {
        AsnCleaner cleaner( *asn.value.u.missionObjectiveList );
        Accept( cleaner );
        delete[] asn.value.u.missionObjectiveList->elem;
    }
    delete asn.value.u.missionObjectiveList;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectiveList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool ActionParameterObjectiveList::IsSet() const
{
    return !elements_.empty(); // $$$$ SBO 2008-03-19: each element must be set as well...
}
