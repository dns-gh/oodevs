// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterIntelligence.h"
#include "ActionParameterString.h"
#include "ActionParameterBool.h"
#include "ActionParameterPoint.h"
#include "ActionParameterFormation.h"
#include "ActionParameterKarma.h"
#include "ActionParameterLevel.h"
#include "ActionParameterVisitor_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/IntelligenceHierarchies.h"
#include "clients_kernel/Positions.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligence constructor
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
ActionParameterIntelligence::ActionParameterIntelligence( const OrderParameter& parameter, const CoordinateConverter_ABC& converter )
    : ActionParameterEntity< Intelligence_ABC >( parameter, 0 )
    , converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligence constructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
ActionParameterIntelligence::ActionParameterIntelligence( const CoordinateConverter_ABC& converter, xml::xistream& xis, const Resolver_ABC< Formation_ABC >& resolver, const FormationLevels& levels )
    : ActionParameterEntity< Intelligence_ABC >( OrderParameter( attribute< std::string >( xis, "name" ).c_str(), "intelligence", false ), 0 )
    , converter_( converter )
{
    xis >> list( "parameter", *this, &ActionParameterIntelligence::ReadParameter, resolver, levels );
}

namespace
{
    kernel::Point MakePoint( const CoordinateConverter_ABC& converter, const ASN1T_CoordUTM& asn )
    {
        kernel::Point point;
        point.AddPoint( converter.ConvertToXY( asn ) );
        return point;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligence constructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
ActionParameterIntelligence::ActionParameterIntelligence( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Resolver_ABC< Formation_ABC >& resolver, const FormationLevels& levels, const ASN1T_Intelligence& asn )
    : ActionParameterEntity< Intelligence_ABC >( parameter, 0 )
    , converter_( converter )
{
    AddParameter( *new ActionParameterString   ( OrderParameter( tools::translate( "ActionParameter", "Name" )     , "name"     , false ), asn.name ) );
    AddParameter( *new ActionParameterString   ( OrderParameter( tools::translate( "ActionParameter", "Nature" )   , "nature"   , false ), asn.nature ) );
    AddParameter( *new ActionParameterKarma    ( OrderParameter( tools::translate( "ActionParameter", "Karma" )    , "karma"    , false ), asn.diplomacy ) );
    AddParameter( *new ActionParameterLevel    ( OrderParameter( tools::translate( "ActionParameter", "Level" )    , "level"    , false ), asn.level, levels ) );
    AddParameter( *new ActionParameterBool     ( OrderParameter( tools::translate( "ActionParameter", "Embarked" ) , "bool"     , false ), asn.embarked ) );
    AddParameter( *new ActionParameterFormation( OrderParameter( tools::translate( "ActionParameter", "Formation" ), "formation", false ), asn.formation, resolver ) );
    AddParameter( *new ActionParameterPoint    ( OrderParameter( tools::translate( "ActionParameter", "Point" )    , "point"    , false ), converter, MakePoint( converter, asn.location ) ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligence destructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
ActionParameterIntelligence::~ActionParameterIntelligence()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligence::ReadParameter
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void ActionParameterIntelligence::ReadParameter( xml::xistream& xis, const Resolver_ABC< Formation_ABC >& resolver, const FormationLevels& levels )
{
    const QString name = attribute< std::string >( xis, "name" ).c_str();
    const QString type = attribute< std::string >( xis, "type" ).c_str();

    if( type == "name" || type == "nature" )
        AddParameter( *new ActionParameterString( OrderParameter( name, type, false ), xis ) );
    else if( type == "karma" )
        AddParameter( *new ActionParameterKarma( OrderParameter( name, type, false ), xis ) );
    else if( type == "level" )
        AddParameter( *new ActionParameterLevel( OrderParameter( name, type, false ), xis, levels ) );
    else if( type == "bool" )
        AddParameter( *new ActionParameterBool( OrderParameter( name, type, false ), xis ) );
    else if( type == "formation" )
        AddParameter( *new ActionParameterFormation( OrderParameter( name, type, false ), xis, resolver ) );
    else if( type == "point" )
        AddParameter( *new ActionParameterPoint( OrderParameter( name, type, false ), converter_, xis ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligence::CommitTo
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void ActionParameterIntelligence::CommitTo( ASN1T_Intelligence& asn ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const QString type = it->second->GetType();
        if( type == "name" )
            static_cast< const ActionParameterString* >( it->second )->CommitTo( asn.name );
        else if( type == "nature" )
            static_cast< const ActionParameterString* >( it->second )->CommitTo( asn.nature );
        else if( type == "level" )
            static_cast< const ActionParameterLevel* >( it->second )->CommitTo( asn.level );
        else if( type == "karma" )
            static_cast< const ActionParameterKarma* >( it->second )->CommitTo( asn.diplomacy );
        else if( type == "bool" )
            static_cast< const ActionParameterBool* >( it->second )->CommitTo( asn.embarked );
        else if( type == "formation" )
            static_cast< const ActionParameterFormation* >( it->second )->CommitTo( asn.formation );
        else if( type == "point" )
            static_cast< const ActionParameterPoint* >( it->second )->CommitTo( asn.location );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligence::Clean
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void ActionParameterIntelligence::Clean( ASN1T_Intelligence& asn ) const
{
    // NOTHING  // $$$$ SBO 2007-10-23: too good to be true!
}

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligence::Accept
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void ActionParameterIntelligence::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligence::CommitToChildren
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
void ActionParameterIntelligence::CommitToChildren()
{
    const Intelligence_ABC& entity = *GetValue();
    const Formation_ABC* formation = static_cast< const Formation_ABC* >( entity.Get< IntelligenceHierarchies >().GetSuperior() );
    kernel::Point position;
    position.AddPoint( entity.Get< kernel::Positions >().GetPosition() );

    AddParameter( *new ActionParameterString   ( OrderParameter( tools::translate( "ActionParameter", "Name" )     , "name"     , false ), entity.GetName() ) );
    AddParameter( *new ActionParameterString   ( OrderParameter( tools::translate( "ActionParameter", "Nature" )   , "nature"   , false ), entity.GetSymbol() ) );
    AddParameter( *new ActionParameterKarma    ( OrderParameter( tools::translate( "ActionParameter", "Karma" )    , "karma"    , false ), entity.GetKarma() ) );
    AddParameter( *new ActionParameterLevel    ( OrderParameter( tools::translate( "ActionParameter", "Level" )    , "level"    , false ), entity.GetLevel() ) );
    AddParameter( *new ActionParameterBool     ( OrderParameter( tools::translate( "ActionParameter", "Embarked" ) , "bool"     , false ), entity.IsEmbarked() ) );
    AddParameter( *new ActionParameterFormation( OrderParameter( tools::translate( "ActionParameter", "Formation" ), "formation", false ), *formation ) );
    AddParameter( *new ActionParameterPoint    ( OrderParameter( tools::translate( "ActionParameter", "Point" )    , "point"    , false ), converter_, position ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterIntelligence::Serialize
// Created: SBO 2007-10-31
// -----------------------------------------------------------------------------
void ActionParameterIntelligence::Serialize( xml::xostream& xos ) const
{
    ActionParameter< const Intelligence_ABC* >::Serialize( xos );
}
