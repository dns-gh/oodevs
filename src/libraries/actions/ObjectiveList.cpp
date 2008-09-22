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
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;
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
    xis >> list( "parameter", *this, &ObjectiveList::ReadParameter, converter );
}

// -----------------------------------------------------------------------------
// Name: ObjectiveList constructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ObjectiveList::ObjectiveList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_MissionObjectiveList& asn )
    : Parameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new Objective( OrderParameter( tools::translate( "Parameter", "Objective %1" ).arg( i + 1 ).ascii(), "objective", false ), converter, asn.elem[i] ) );
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
    xis >> attribute( "type", type );
    if( type == "objective" )
        AddParameter( *new Objective( xis, converter ) );
}


namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_MissionObjectiveList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const Objective& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_MissionObjectiveList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectiveList::CommitTo
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ObjectiveList::CommitTo( ASN1T_MissionParameter& asn ) const
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
    struct AsnCleaner : public ParameterVisitor_ABC
    {
        explicit AsnCleaner( ASN1T_MissionObjectiveList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const Objective& param )
        {
            param.Clean( asn_->elem[current_++] );
        }
   
        ASN1T_MissionObjectiveList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectiveList::Clean
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ObjectiveList::Clean( ASN1T_MissionParameter& asn ) const
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
// Name: ObjectiveList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool ObjectiveList::IsSet() const
{
    return !elements_.empty(); // $$$$ SBO 2008-03-19: each element must be set as well...
}
