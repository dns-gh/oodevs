//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "AutomatType.h"
#include "clients_kernel/App6Symbol.h"
#include "tools/Iterator.h"
#include "AutomatComposition.h"
#include "AgentType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AutomatType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AutomatType::AutomatType( xml::xistream& xis, const tools::Resolver_ABC< AgentType, std::string >& agentResolver
                                            , const tools::Resolver_ABC< DecisionalModel, std::string >& modelResolver )
    : pcType_( 0 )
{
    std::string modelName;
    xis >> xml::attribute( "name", name_ )
        >> xml::attribute( "type", type_ )
        >> xml::attribute( "id", id_ )
        >> xml::attribute( "decisional-model", modelName )
        >> xml::list( "unit", *this, &AutomatType::ReadAgent, agentResolver );
    model_ = & modelResolver.Get( modelName );
    if( ! pcType_ )
        throw MASA_EXCEPTION( "Automat '" + name_ + "' has no command-post" );
    UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: AutomatType destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AutomatType::~AutomatType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatType::UpdateSymbol
// Created: LDC 2011-03-11
// -----------------------------------------------------------------------------
void AutomatType::UpdateSymbol()
{
    for( T_UnitConstitution::const_iterator it = units_.begin(); it != units_.end(); ++it )
        App6Symbol::Merge( (*it)->GetType().GetSymbol(), symbol_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatType::ReadAgent
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
void AutomatType::ReadAgent( xml::xistream& xis, const tools::Resolver_ABC< AgentType, std::string >& agentResolver )
{
    units_.push_back( new AutomatComposition( xis, agentResolver ) );
    bool commandPost = false;
    xis >> xml::optional >> xml::attribute( "command-post", commandPost );
    if( commandPost )
        pcType_ = & units_.back()->GetType();
}

// -----------------------------------------------------------------------------
// Name: AutomatType::NumberOfAgents
// Created: JSR 2012-07-02
// -----------------------------------------------------------------------------
unsigned int AutomatType::NumberOfAgents() const
{
    unsigned int number = 0;
    tools::Iterator< const AutomatComposition& > it = CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::AutomatComposition& composition = it.NextElement();
        number += composition.GetSensibleNumber();
    }
    return number;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::CreateIterator
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
tools::Iterator< const AutomatComposition& > AutomatType::CreateIterator() const
{
    return new tools::SimpleIterator< const AutomatComposition&, T_UnitConstitution >( units_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatType::GetTypePC
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
const AgentType* AutomatType::GetTypePC() const
{
    return pcType_;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::GetId
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
unsigned long AutomatType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::GetName
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
std::string AutomatType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::GetSymbol
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
std::string AutomatType::GetSymbol() const
{
    return symbol_;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::GetTypeName
// Created: MMC 2012-03-19
// -----------------------------------------------------------------------------
std::string AutomatType::GetTypeName() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::GetDecisionalModel
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const DecisionalModel& AutomatType::GetDecisionalModel() const
{
    return *model_;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::HasLogistics
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
bool AutomatType::HasLogistics() const
{
    return IsLogisticMaintenance() || IsLogisticMedical() || IsLogisticSupply();
}

// -----------------------------------------------------------------------------
// Name: AutomatType::IsTC2
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
bool AutomatType::IsTC2() const
{
    return type_ == "Automate LOG TC2"
        || type_ == "Automate Organization";
}

// -----------------------------------------------------------------------------
// Name: AutomatType::IsLogisticSupply
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
bool AutomatType::IsLogisticSupply() const
{
    return type_ == "Automate LOG BLD Ravitaillement"
        || type_ == "Automate LOG BLT Ravitaillement"
        || IsTC2();
}

// -----------------------------------------------------------------------------
// Name: AutomatType::IsLogisticMaintenance
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
bool AutomatType::IsLogisticMaintenance() const
{
    return type_ == "Automate LOG BLD Maintenance"
        || type_ == "Automate LOG BLT Maintenance"
        || IsTC2();
}

// -----------------------------------------------------------------------------
// Name: AutomatType::IsLogisticMedical
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
bool AutomatType::IsLogisticMedical() const
{
    return type_ == "Automate LOG BLD Sante"
        || type_ == "Automate LOG BLT Sante"
        || IsTC2();
}
