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
ActionParameterObstacleList::ActionParameterObstacleList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& resolver, const ASN1T_ListMissionGenObject& asn )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterObstacle( tools::translate( "ActionParameter", "Obstacle %1" ).arg( i + 1 ), converter, resolver, asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterObstacleList::ActionParameterObstacleList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& resolver, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterObstacleList::ReadObstacle, converter, resolver );
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
void ActionParameterObstacleList::ReadObstacle( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& resolver )
{
    AddParameter( *new ActionParameterObstacle( converter, resolver, xis ) );
}

namespace
{
    struct AsnSerializer : public ActionParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_ListMissionGenObject& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterObstacle& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_ListMissionGenObject* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterObstacleList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    ASN1T_ListMissionGenObject*& list = asn.value.u.listMissionGenObject = new ASN1T_ListMissionGenObject();
    asn.value.t = T_MissionParameter_value_listMissionGenObject;
    asn.null_value = ( list->n = Count() ) ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_MissionGenObject[list->n];
    AsnSerializer serializer( *list );
    Accept( serializer );
}

namespace
{
    struct AsnCleaner : public ActionParameterVisitor_ABC
    {
        explicit AsnCleaner( ASN1T_ListMissionGenObject& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterObstacle& param )
        {
            param.Clean( asn_->elem[current_++] );
        }
   
        ASN1T_ListMissionGenObject* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterObstacleList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listMissionGenObject )
    {
        AsnCleaner cleaner( *asn.value.u.listMissionGenObject );
        Accept( cleaner );
        delete[] asn.value.u.listMissionGenObject->elem;
    }
    delete asn.value.u.listMissionGenObject;
}
