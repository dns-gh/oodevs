// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterObstacleList.h"
#include "ActionParameterObstacle.h"
#include "ActionParameterVisitor_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleList constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterObstacleList::ActionParameterObstacleList( const OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleList constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterObstacleList::ActionParameterObstacleList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& resolver, const Resolver_ABC< Automat_ABC >& automats, const ASN1T_PlannedWorkList& asn )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterObstacle( OrderParameter( tools::translate( "ActionParameter", "Obstacle %1" ).arg( i + 1 ), "obstacle", false ), converter, resolver, automats, asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterObstacleList::ActionParameterObstacleList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& resolver, const Resolver_ABC< Automat_ABC >& automats, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterObstacleList::ReadObstacle, converter, resolver, automats );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleList destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterObstacleList::~ActionParameterObstacleList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleList::ReadObstacle
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterObstacleList::ReadObstacle( xml::xistream& xis, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& resolver, const Resolver_ABC< Automat_ABC >& automats )
{
    AddParameter( *new ActionParameterObstacle( converter, resolver, automats, xis ) );
}

namespace
{
    struct AsnSerializer : public ActionParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_PlannedWorkList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterObstacle& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_PlannedWorkList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterObstacleList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    ASN1T_PlannedWorkList*& list = asn.value.u.plannedWorkList = new ASN1T_PlannedWorkList();
    asn.value.t = T_MissionParameter_value_plannedWorkList;
    asn.null_value = ( list->n = Count() ) ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_PlannedWork[list->n];
    AsnSerializer serializer( *list );
    Accept( serializer );
}

namespace
{
    struct AsnCleaner : public ActionParameterVisitor_ABC
    {
        explicit AsnCleaner( ASN1T_PlannedWorkList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterObstacle& param )
        {
            param.Clean( asn_->elem[current_++] );
        }
   
        ASN1T_PlannedWorkList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterObstacleList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.plannedWorkList )
    {
        AsnCleaner cleaner( *asn.value.u.plannedWorkList );
        Accept( cleaner );
        delete[] asn.value.u.plannedWorkList->elem;
    }
    delete asn.value.u.plannedWorkList;
}
