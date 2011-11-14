// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Contaminations.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/DictionaryUpdated.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "statusicons.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Contaminations constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Contaminations::Contaminations( Controller& controller, kernel::Entity_ABC& entity, const tools::Resolver_ABC< NBCAgent >& resolver, PropertiesDictionary& dico )
    : controller_            ( controller )
    , entity_                ( entity )
    , resolver_              ( resolver )
    , bNbcProtectionSuitWorn_( false )
    , nContamination_        ( 0 )
    , quantity_              ( 0 )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: Contaminations destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Contaminations::~Contaminations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Contaminations::CreateDictionary
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void Contaminations::CreateDictionary( PropertiesDictionary& dico ) const
{
    dico.Register( entity_, tools::translate( "NBC", "NBC/NBC suit" ), bNbcProtectionSuitWorn_ );
    dico.Register( entity_, tools::translate( "NBC", "NBC/Contaminating agents" ), contaminatingNbcAgents_ );
    dico.Register( entity_, tools::translate( "NBC", "NBC/Contamination level" ), nContamination_ );
    dico.Register( entity_, tools::translate( "NBC", "NBC/Contamination quantity" ), quantity_ );
}

// -----------------------------------------------------------------------------
// Name: Contaminations::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Contaminations::DoUpdate( const sword::UnitAttributes& message )
{
    std::set< std::string > updated;

    UPDATE_SUBPROPERTY( message, nContamination_, contamination_state, percentage, "NBC", updated );
    UPDATE_SUBPROPERTY( message, quantity_, contamination_state, quantity, "NBC", updated );

    if( nContamination_ == 0 )
        contaminatingNbcAgents_.clear();

   if( message.has_contamination_agents() )
    {
        contaminatingNbcAgents_.clear();
        contaminatingNbcAgents_.reserve( message.contamination_agents().elem_size() );
        for( int i = 0; i < message.contamination_agents().elem_size(); ++i )
            contaminatingNbcAgents_.push_back( &resolver_.Get( message.contamination_agents().elem( i ).id() ) );
    }

   UPDATE_PROPERTY( message, bNbcProtectionSuitWorn_, protective_suits, "NBC", updated );

    if( !updated.empty() )
        controller_.Update( kernel::DictionaryUpdated( entity_, tools::translate( "NBC", "NBC" ) ) );

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Contaminations::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void Contaminations::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "NBC", "NBC" ) )
                .Display( tools::translate( "NBC", "NBC suit:" ),
                        bNbcProtectionSuitWorn_ ? tools::translate( "NBC", "on" ) : tools::translate( "NBC", "off" ) )
                .Display( tools::translate( "NBC", "Contaminating agents:" ), contaminatingNbcAgents_ )
                .Display( tools::translate( "NBC", "Contamination level:" ), nContamination_ )
                .Display( tools::translate( "NBC", "Contamination quantity:" ), quantity_ );
}

// -----------------------------------------------------------------------------
// Name: Contaminations::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Contaminations::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( ! contaminatingNbcAgents_.empty() && viewport.IsHotpointVisible() )
        tools.DrawIcon( xpm_nbc, where, 150.f );
}
