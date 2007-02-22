//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "AutomatType.h"
#include "GlTools_ABC.h"
#include "Iterator.h"
#include "AutomatComposition.h"
#include "AgentType.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: AutomatType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AutomatType::AutomatType( xml::xistream& xis, const Resolver_ABC< AgentType, QString >& agentResolver
                                            , const Resolver_ABC< DecisionalModel, QString >& modelResolver )
{
    int id;
    std::string modelName, name;

    xis >> attribute( "nom", name )
        >> attribute( "type", type_ )
        >> content( "MosID", id )
        >> start( "Automate" )
            >> content( "ModeleDecisionnel", modelName )
            >> start( "Constitution" )
                >> list( "Pion", *this, &AutomatType::ReadAgent, agentResolver )
            >> end();
    id_ = id;
    name_ = name.c_str();
    model_ = & modelResolver.Get( modelName.c_str() );
    std::string pcType;
    xis >> end()
        >> start( "PionPC" )
            >> attribute( "type", pcType );
    pcType_ = & agentResolver.Get( pcType.c_str() );

    symbol_ = pcType_->GetSymbol();
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
// Name: AutomatType::ReadAgent
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
void AutomatType::ReadAgent( xml::xistream& xis, const Resolver_ABC< AgentType, QString >& agentResolver )
{
    units_.push_back( new AutomatComposition( xis, agentResolver ) );
}

// -----------------------------------------------------------------------------
// Name: AutomatType::CreateIterator
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
Iterator< const AutomatComposition& > AutomatType::CreateIterator() const
{
    return new SimpleIterator< const AutomatComposition&, T_UnitConstitution >( units_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatType::GetTypePC
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentType* AutomatType::GetTypePC() const
{   
    return pcType_;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::GetId
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
unsigned long AutomatType::GetId()
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::GetName
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
QString AutomatType::GetName() const
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
// Name: AutomatType::GetDecisionalModel
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const DecisionalModel& AutomatType::GetDecisionalModel() const
{
    return *model_;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::Draw
// Created: SBO 2006-03-21
// -----------------------------------------------------------------------------
void AutomatType::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    // $$$$ SBO 2007-02-22: display aggregated level ?
//    if( viewport.IsInside( where ) )
//        tools.DrawApp6Symbol( symbol_, where, 2 );
}

// -----------------------------------------------------------------------------
// Name: AutomatType::HasLogistics
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
bool AutomatType::HasLogistics() const
{
    return IsTC2() || IsLogisticMaintenance() || IsLogisticMedical() || IsLogisticSupply();
}

// -----------------------------------------------------------------------------
// Name: AutomatType::IsTC2
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
bool AutomatType::IsTC2() const
{
    return name_.find( "TC2" ) != -1;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::IsLogisticSupply
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
bool AutomatType::IsLogisticSupply() const
{
    return type_ == "Automate LOG BLD Ravitaillement"
        || type_ == "Automate LOG BLT Ravitaillement";
}

// -----------------------------------------------------------------------------
// Name: AutomatType::IsLogisticMaintenance
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
bool AutomatType::IsLogisticMaintenance() const
{
    return type_ == "Automate LOG BLD Maintenance"
        || type_ == "Automate LOG BLT Maintenance";
}

// -----------------------------------------------------------------------------
// Name: AutomatType::IsLogisticMedical
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
bool AutomatType::IsLogisticMedical() const
{
    return type_ == "Automate LOG BLD Sante"
        || type_ == "Automate LOG BLT Sante";
}
