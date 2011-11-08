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
#include "AgentComposition.h"
#include "AgentNature.h"
#include "DotationCapacityType.h"
#include "SymbolFactory.h"
#include "LogisticSupplyClass.h"
#include <xeumeuleu/xml.hpp>
#include "ENT/ENT_Tr.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentType::AgentType( xml::xistream& xis, const tools::Resolver_ABC< ComponentType, std::string >& componentResolver, const tools::Resolver_ABC< DecisionalModel, std::string >& modelResolver, const SymbolFactory& symbolFactory )
    : nature_            ( 0 )
    , nbrOfficers_       ( 0 )
    , nbrWarrantOfficers_( 0 )
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
        >> xml::end
        >> xml::start( "crew-ranks" )
            >> xml::list( "crew-rank", *this, &AgentType::ReadCrewRank )
        >> xml::end
        >> xml::optional
        >> xml::start( "logistics" )
            >> xml::list( "category", *this, &AgentType::ReadResourcesCategory )
        >> xml::end
        >> xml::optional
        >> xml::start( "stocks" )
            >> xml::list( "stock", *this, &AgentType::ReadStock )
        >> xml::end;

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
    for( CIT_Components it = equipments_.begin(); it != equipments_.end(); ++it )
        delete *it;
    for( CIT_Resources it = resources_.begin(); it != resources_.end(); ++it )
        delete *it;
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
// Name: AgentType::ReadCrewRank
// Created: ABR 2011-02-25
// -----------------------------------------------------------------------------
void AgentType::ReadCrewRank( xml::xistream& xis )
{
    std::string type = xis.attribute< std::string >( "type");
    if( type == "Officier" )
        nbrOfficers_ = xis.attribute< unsigned int >( "count");
    else if( type == "SousOfficier" )
        nbrWarrantOfficers_ = xis.attribute< unsigned int >( "count");
    else
        xis.error( "crew-rank: type \'" + type + "\' undefined" );
}

// -----------------------------------------------------------------------------
// Name: AgentType::ReadResourcesCategory
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void AgentType::ReadResourcesCategory( xml::xistream& xis )
{
    std::string resourceCategory = xis.attribute< std::string >( "name" );
    xis >> xml::list( "resource", *this, &AgentType::ReadResources );
}

// -----------------------------------------------------------------------------
// Name: AgentType::ReadResources
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void AgentType::ReadResources( xml::xistream& xis )
{
    resources_.push_back( new DotationCapacityType( xis ) );
}

// -----------------------------------------------------------------------------
// Name: AgentType::ReadStock
// Created: MMC 2011-08-30
// -----------------------------------------------------------------------------
void AgentType::ReadStock( xml::xistream& xis )
{
    const std::string logisticSupplyClass = xis.attribute< std::string >( "logistic-supply-class" );
    unsigned int threshold = xis.attribute< unsigned int >( "threshold" );
    stocks_[ logisticSupplyClass ] = threshold;
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
// Name: tools::Iterator< const DotationCapacityType& > AgentType::CreateResourcesIterator
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
tools::Iterator< const DotationCapacityType& > AgentType::CreateResourcesIterator() const
{
    return new tools::SimpleIterator< const DotationCapacityType&, T_Resources >( resources_ );
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
// Name: AgentType::GetNbrOfficers
// Created: ABR 2011-02-25
// -----------------------------------------------------------------------------
unsigned int AgentType::GetNbrOfficers() const
{
    return nbrOfficers_;
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetNbrWarrantOfficers
// Created: ABR 2011-02-25
// -----------------------------------------------------------------------------
unsigned int AgentType::GetNbrWarrantOfficers() const
{
    return nbrWarrantOfficers_;
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
// Name: AgentType::IsStockCategoryDefined
// Created: MMC 2011-08-30
// -----------------------------------------------------------------------------
bool AgentType::IsStockCategoryDefined( const LogisticSupplyClass& logClass ) const
{
    return stocks_.find( logClass.GetName() ) != stocks_.end();
}

// -----------------------------------------------------------------------------
// Name: AgentType::HasStocks
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
bool AgentType::HasStocks() const
{
    return !stocks_.empty();
}
