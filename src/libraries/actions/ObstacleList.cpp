// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "ObstacleList.h"
#include "Obstacle.h"
#include "ParameterVisitor_ABC.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: ObstacleList constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ObstacleList::ObstacleList( const OrderParameter& parameter )
    : Parameter< std::string >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleList constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ObstacleList::ObstacleList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& resolver, const Resolver_ABC< Automat_ABC >& automats, const ASN1T_PlannedWorkList& asn, kernel::Controller& controller )
    : Parameter< std::string >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new Obstacle( OrderParameter( tools::translate( "Parameter", "Obstacle %1" ).arg( i + 1 ).ascii(), "obstacle", false ), converter, resolver, automats, asn.elem[i], controller ) );
}

// -----------------------------------------------------------------------------
// Name: ObstacleList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ObstacleList::ObstacleList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& resolver, const Resolver_ABC< Automat_ABC >& automats, xml::xistream& xis, kernel::Controller& controller )
    : Parameter< std::string >( parameter )
{
    xis >> list( "parameter", *this, &ObstacleList::ReadObstacle, converter, resolver, automats, controller );
}

// -----------------------------------------------------------------------------
// Name: ObstacleList destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ObstacleList::~ObstacleList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleList::ReadObstacle
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ObstacleList::ReadObstacle( xml::xistream& xis, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& resolver, const Resolver_ABC< Automat_ABC >& automats, kernel::Controller& controller )
{
    AddParameter( *new Obstacle( converter, resolver, automats, xis, controller ) );
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_PlannedWorkList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const Obstacle& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_PlannedWorkList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ObstacleList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ObstacleList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    ASN1T_PlannedWorkList*& list = asn.value.u.plannedWorkList = new ASN1T_PlannedWorkList();
    asn.value.t = T_MissionParameter_value_plannedWorkList;
    list->n = Count();
    if( IsSet() )
    {
        list->elem = new ASN1T_PlannedWork[list->n];
        AsnSerializer serializer( *list );
        Accept( serializer );
    }
}

namespace
{
    struct AsnCleaner : public ParameterVisitor_ABC
    {
        explicit AsnCleaner( ASN1T_PlannedWorkList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const Obstacle& param )
        {
            param.Clean( asn_->elem[current_++] );
        }
   
        ASN1T_PlannedWorkList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ObstacleList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ObstacleList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.plannedWorkList )
    {
        AsnCleaner cleaner( *asn.value.u.plannedWorkList );
        Accept( cleaner );
        delete[] asn.value.u.plannedWorkList->elem;
    }
    delete asn.value.u.plannedWorkList;
}

// -----------------------------------------------------------------------------
// Name: ObstacleList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void ObstacleList::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    Parameter< std::string >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ObstacleList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool ObstacleList::IsSet() const
{
    return !elements_.empty(); // $$$$ SBO 2008-03-19: each element must be set as well...
}
