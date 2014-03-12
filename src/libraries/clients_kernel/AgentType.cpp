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
#include "ComponentType.h"
#include "SymbolFactory.h"
#include "LogisticSupplyClass.h"
#include "XmlTranslations.h"
#include <xeumeuleu/xml.hpp>
#include "ENT/ENT_Tr.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentType::AgentType( xml::xistream& xis, const tools::Resolver_ABC< ComponentType, std::string >& componentResolver,
                      const tools::Resolver_ABC< DecisionalModel, std::string >& modelResolver,
                      const SymbolFactory& symbolFactory, kernel::XmlTranslations& translations )
    : name_( translations.GetTranslation( "units", xis.attribute< std::string >( "name" ) ) )
    , nature_( 0 )
    , nbrOfficers_( 0 )
    , nbrWarrantOfficers_( 0 )
    , nbcSuit_( eAgentNone )
{
    std::string modelName, nbcSuit;
    xis >> xml::attribute( "type", type_ )
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
        >> xml::start( "nbc" )
            >> xml::attribute( "suit", nbcSuit )
        >> xml::end
        >> xml::optional
        >> xml::start( "logistics" )
            >> xml::list( "category", *this, &AgentType::ReadResourcesCategory )
        >> xml::end
        >> xml::optional
        >> xml::start( "stocks" )
            >> xml::list( "stock", *this, &AgentType::ReadStock )
        >> xml::end;

    nbcSuit_ = ENT_Tr::ConvertToAgentNbcSuit( nbcSuit );
    symbol_      = symbolFactory.CreateSymbol( nature->GetNature() );
    moveSymbol_  = symbolFactory.CreateMoveSymbol( nature->GetNature() );
    staticSymbol_ = symbolFactory.CreateStaticSymbol( nature->GetNature() );
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
    for( auto it = equipments_.begin(); it != equipments_.end(); ++it )
        delete *it;
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
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
        throw MASA_EXCEPTION( xis.context() + "crew-rank: type \'" + type + "\' undefined" );
}

// -----------------------------------------------------------------------------
// Name: AgentType::ReadResourcesCategory
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void AgentType::ReadResourcesCategory( xml::xistream& xis )
{
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
// Name: AgentType::GetLocalizedName
// Created: JSR 2013-09-30
// -----------------------------------------------------------------------------
const std::string& AgentType::GetLocalizedName() const
{
    return name_->Translate();
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetKeyName
// Created: JSR 2013-09-30
// -----------------------------------------------------------------------------
const std::string& AgentType::GetKeyName() const
{
    return name_->Key();
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
// Name: AgentType::CreateIterator
// Created: SBO 2007-11-09
// -----------------------------------------------------------------------------
tools::Iterator< const AgentComposition& > AgentType::CreateIterator() const
{
    return new tools::SimpleIterator< const AgentComposition&, T_Components >( equipments_ );
}

// -----------------------------------------------------------------------------
// Name: AgentType::CreateResourcesIterator
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
// Name: AgentType::GetMoveSymbol
// Created: LDC 2013-04-09
// -----------------------------------------------------------------------------
const std::string& AgentType::GetMoveSymbol() const
{
    return moveSymbol_;
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetStaticSymbol
// Created: LDC 2013-04-09
// -----------------------------------------------------------------------------
const std::string& AgentType::GetStaticSymbol() const
{
    return staticSymbol_;
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
// Name: AgentType::GetWidth
// Created: LDC 2013-04-11
// -----------------------------------------------------------------------------
float AgentType::GetWidth() const
{
    float result = 0.f;
    for( auto it = equipments_.begin(); it != equipments_.end(); ++it )
    {
        if( (*it)->IsMajor() )
        {
            const ComponentType& type = (*it)->GetType();
            unsigned int count = (*it)->GetCount();
            result += type.GetWidth() * count + type.GetSpacing() * ( count - 1 );
        }
    }
    return result;
}
    
// -----------------------------------------------------------------------------
// Name: AgentType::GetDepth
// Created: LDC 2013-04-11
// -----------------------------------------------------------------------------
float AgentType::GetDepth() const
{
    return nature_->GetDepth();
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetLength
// Created: MMC 2013-04-12
// -----------------------------------------------------------------------------
float AgentType::GetLength( bool isSpeedSafety /*= true*/ ) const
{
    float result = 0.f;
    for( auto it = equipments_.begin(); it != equipments_.end(); ++it )
    {
        const ComponentType& type = (*it)->GetType();
        result += (*it)->GetCount() * ( type.GetLength() + ( isSpeedSafety ? type.GetSpeedSafetyDistance() : type.GetSafetyDistance() ) );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetTypeName
// Created: MMC 2012-03-19
// -----------------------------------------------------------------------------
const std::string& AgentType::GetTypeName() const
{
    return type_;
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
// Name: AgentType::GetComposantesWeight
// Created: NPT 2012-11-16
// -----------------------------------------------------------------------------
const float AgentType::GetComposantesWeight() const
{
    float weight = 0;
    for( auto it = equipments_.begin(); it != equipments_.end(); ++it )
        weight += ( *it )->GetType().GetWeight() * ( *it )->GetCount();
    return weight;
}

// -----------------------------------------------------------------------------
// Name: AgentType::IsTC2
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
bool AgentType::IsTC2() const
{
    return type_ == "Pion LOG TC2"
        || type_ == "Pion Organization";
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
// Name: AgentType::GetAllowedSupplyClasses
// Created: JSR 2014-03-12
// -----------------------------------------------------------------------------
void AgentType::GetAllowedSupplyClasses( std::set< std::string >& allowedSupplyClasses ) const
{
    for( auto it = stocks_.begin(); it != stocks_.end(); ++it )
        allowedSupplyClasses.insert( it->first );
}

// -----------------------------------------------------------------------------
// Name: AgentType::HasStocks
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
bool AgentType::HasStocks() const
{
    return !stocks_.empty();
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetNbcSuit
// Created: LGY 2012-11-26
// -----------------------------------------------------------------------------
E_AgentNbcSuit AgentType::GetNbcSuit() const
{
    return nbcSuit_;
}
