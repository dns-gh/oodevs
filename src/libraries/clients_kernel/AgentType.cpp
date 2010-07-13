// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "AgentType.h"
#include "AgentComposition.h"
#include "AgentNature.h"
#include "SymbolFactory.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentType::AgentType( xml::xistream& xis, const tools::Resolver_ABC< ComponentType, std::string >& componentResolver, const tools::Resolver_ABC< DecisionalModel, std::string >& modelResolver, const SymbolFactory& symbolFactory )
    : nature_( 0 )
{
    std::string modelName;
    xis >> xml::attribute( "name", name_ )
        >> xml::attribute( "type", type_ )
        >> xml::attribute( "id", id_ )
        >> xml::attribute( "decisional-model", modelName );
    model_ = & modelResolver.Get( modelName );

    std::auto_ptr< AgentNature > nature( new AgentNature( xis ) );
    xis >> xml::start( "equipments" )
        >> xml::list( "equipment", *this, &AgentType::ReadEquipment, componentResolver )
        >> xml::end();
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
void AgentType::ReadEquipment( xml::xistream& xis, const tools::Resolver_ABC< ComponentType, std::string >& resolver )
{
    equipments_.push_back( new AgentComposition( xis, resolver ) );
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
// Name: tools::Iterator< const ComponentType& > AgentType::CreateIterator
// Created: SBO 2007-11-09
// -----------------------------------------------------------------------------
tools::Iterator< const AgentComposition& > AgentType::CreateIterator() const
{
    return new tools::SimpleIterator< const AgentComposition&, T_Components >( equipments_ );
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
