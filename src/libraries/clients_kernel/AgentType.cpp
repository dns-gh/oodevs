// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "AgentType.h"
#include <xeumeuleu/xml.h>
#include "AgentNature.h"
#include "SymbolFactory.h"
#include "GlTools_ABC.h"
#include "Viewport_ABC.h"
#include <qgl.h>
using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: AgentType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentType::AgentType( xml::xistream& xis, const Resolver_ABC< ComponentType, std::string >& componentResolver, const Resolver_ABC< DecisionalModel, std::string >& modelResolver, const SymbolFactory& symbolFactory )
    : nature_( 0 )
{
    std::string modelName;
    xis >> attribute( "name", name_ )
        >> attribute( "type", type_ )
        >> attribute( "id", id_ )
        >> attribute(  "decisional-model", modelName );
    model_ = & modelResolver.Get( modelName );

    std::auto_ptr< AgentNature > nature( new AgentNature( xis ) );
    xis >> start( "equipments" )
        >> list( "equipment", *this, &AgentType::ReadEquipment, componentResolver )
        >> end();
    symbol_      = symbolFactory.CreateSymbol( nature->GetNature() );
    levelSymbol_ = symbolFactory.CreateLevelSymbol( nature->GetLevel() );
    hqSymbol_    = symbolFactory.CreateAutomatSymbol();
    nature_ = nature.release();
}

// -----------------------------------------------------------------------------
// Name: AgentType destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentType::~AgentType()
{
    delete nature_;
}

// -----------------------------------------------------------------------------
// Name: AgentType::ReadEquipment
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentType::ReadEquipment( xml::xistream& xis, const Resolver_ABC< ComponentType, std::string >& resolver )
{
    equipments_[ & resolver.Get( xml::attribute< std::string >( xis, "type" ) ) ]
              += xml::attribute< unsigned int >( xis, "count" );
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetId
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
unsigned long AgentType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetName
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
std::string AgentType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetDecisionalModel
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const DecisionalModel& AgentType::GetDecisionalModel() const
{
    return *model_;
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetNature
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
const AgentNature& AgentType::GetNature() const
{
    return *nature_;
}

// -----------------------------------------------------------------------------
// Name: Iterator< const ComponentType& > AgentType::CreateIterator
// Created: SBO 2007-11-09
// -----------------------------------------------------------------------------
Iterator< const ComponentType& > AgentType::CreateIterator() const
{
    return new KeyIterator< const ComponentType&, T_Components >( equipments_ );
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetSymbol
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
const std::string& AgentType::GetSymbol() const
{
    return symbol_;
}
    
// -----------------------------------------------------------------------------
// Name: AgentType::GetLevelSymbol
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
const std::string& AgentType::GetLevelSymbol() const
{
    return levelSymbol_;
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetHQSymbol
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
const std::string& AgentType::GetHQSymbol() const
{
    return hqSymbol_;
}

// -----------------------------------------------------------------------------
// Name: AgentType::IsTC2
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
bool AgentType::IsTC2() const
{
    return type_ == "Pion LOG TC2";
}

// -----------------------------------------------------------------------------
// Name: AgentType::IsLogisticSupply
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
bool AgentType::IsLogisticSupply() const
{
    return type_ == "Pion LOG BLD Ravitaillement"
        || type_ == "Pion LOG BLT Ravitaillement"
        || IsTC2();
}

// -----------------------------------------------------------------------------
// Name: AgentType::IsLogisticMaintenance
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
bool AgentType::IsLogisticMaintenance() const
{
    return type_ == "Pion LOG BLD Maintenance"
        || type_ == "Pion LOG BLT Maintenance"
        || IsTC2();
}

// -----------------------------------------------------------------------------
// Name: AgentType::IsLogisticMedical
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
bool AgentType::IsLogisticMedical() const
{
    return type_ == "Pion LOG BLD Sante"
        || type_ == "Pion LOG BLT Sante"
        || IsTC2();
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetComponentCount
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
unsigned int AgentType::GetComponentCount( const ComponentType& type ) const
{
    T_Components::const_iterator it = equipments_.find( &type );
    if( it != equipments_.end() )
        return it->second;
    return 0;
}
