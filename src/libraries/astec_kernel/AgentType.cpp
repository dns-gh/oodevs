// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_kernel_pch.h"
#include "AgentType.h"
#include "xeumeuleu/xml.h"
#include "AgentNature.h"
#include "SymbolFactory.h"
#include "astec_gui/GlTools_ABC.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: AgentType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentType::AgentType( xml::xistream& xis, const Resolver_ABC< ComponentType, std::string >& componentResolver, const Resolver_ABC< DecisionalModel, std::string >& modelResolver, const SymbolFactory& symbolFactory )
    : nature_( 0 )
{
    std::string modelName;
    int id;
    xis >> attribute( "nom", name_ )
        >> content( "MosID", id )
        >> content(  "ModeleDecisionnel", modelName );
    id_ = id;
    model_ = & modelResolver.Get( modelName );

    std::auto_ptr< AgentNature > nature( new AgentNature( xis ) );
    xis >> start( "Equipements" )
        >> list( "Equipement", *this, &AgentType::ReadEquipment, componentResolver )
        >> end();
    nature_ = nature.release();
    symbol_      = symbolFactory.CreateSymbol( *nature_ );
    levelSymbol_ = symbolFactory.CreateLevelSymbol( *nature_ );
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
    std::string name;
    xis >> attribute( "nom", name );
    equipments_.push_back( & resolver.Get( name ) );
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
const std::string& AgentType::GetName() const
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
// Name: AgentType::Draw
// Created: SBO 2006-03-21
// -----------------------------------------------------------------------------
void AgentType::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsInside( where ) )
    {
        tools.DrawApp6Symbol( symbol_, where );
        tools.DrawApp6Symbol( levelSymbol_, where );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentType::DrawAggregated
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void AgentType::DrawAggregated( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools )
{
    if( viewport.IsInside( where ) )
        tools.DrawApp6Symbol( symbol_, where, 2.f );
}
