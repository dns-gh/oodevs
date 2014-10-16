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
#include "Tools.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_gui/DictionaryUpdated.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/Tools.h"
#include "statusicons.h"
#include <boost/foreach.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------<
// Name: Contaminations constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Contaminations::Contaminations( Controller& controller, kernel::Entity_ABC& entity, const tools::Resolver_ABC< NBCAgent >& resolver,
                                gui::PropertiesDictionary& dico, const kernel::AgentType& type )
    : controller_            ( controller )
    , entity_                ( entity )
    , resolver_              ( resolver )
    , bNbcProtectionSuitWorn_( false )
    , contaminated_          ( false )
    , nDecontamination_      ( 0 )
    , quantity_              ( 0 )
    , dose_                  ( 0.f )
    , type_                  ( type.GetNbcSuit() )
    , suit_                  ( tools::ToString( type_ ) )
{
    dico.Register( entity_, tools::translate( "NBC", "NBC/NBC suit" ), bNbcProtectionSuitWorn_, true );
    dico.Register( entity_, tools::translate( "NBC", "NBC/Contaminating agents" ), contaminatingNbcAgents_, true );
    dico.Register( entity_, tools::translate( "NBC", "NBC/Decontamination process" ), nDecontamination_, true );
    dico.Register( entity_, tools::translate( "NBC", "NBC/Contamination quantity" ), quantity_, true );
    dico.Register( entity_, tools::translate( "NBC", "NBC/Dose" ), dose_, true );
    dico.Register( entity_, tools::translate( "NBC", "NBC/Contaminated" ), contaminated_, true );
    if( type_ != eAgentNone )
        dico.Register( entity_, tools::translate( "NBC", "NBC/Suit" ), suit_, true );
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
// Name: Contaminations::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Contaminations::DoUpdate( const sword::UnitAttributes& message )
{
    std::set< std::string > updated;

    UPDATE_SUBPROPERTY( message, nDecontamination_, contamination_state, decontamination_process, "NBC/Decontamination process", updated );
    UPDATE_SUBPROPERTY( message, quantity_, contamination_state, quantity, "NBC/Contamination quantity", updated );
    UPDATE_SUBPROPERTY( message, dose_, contamination_state, dose, "NBC/Dose", updated );
    UPDATE_SUBPROPERTY( message, contaminated_, contamination_state, contaminated, "NBC/Contaminated", updated );

    if( !contaminated_ )
        contaminatingNbcAgents_.clear();

   if( message.has_contamination_agents() )
    {
        contaminatingNbcAgents_.clear();
        contaminatingNbcAgents_.reserve( message.contamination_agents().elem_size() );
        for( int i = 0; i < message.contamination_agents().elem_size(); ++i )
            contaminatingNbcAgents_.push_back( &resolver_.Get( message.contamination_agents().elem( i ).id() ) );
        updated.insert( "NBC/Contaminating agents" );
    }

   UPDATE_PROPERTY( message, bNbcProtectionSuitWorn_, protective_suits, "NBC/NBC suit", updated );

   BOOST_FOREACH( const std::string& content, updated )
       controller_.Update( gui::DictionaryUpdated( entity_, tools::translate( "NBC", content.c_str() ) ) );

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
                .Display( tools::translate( "NBC", "Decontamination process:" ), nDecontamination_ )
                .Display( tools::translate( "NBC", "Contamination quantity:" ), quantity_ )
                .Display( tools::translate( "NBC", "Contaminated:" ), contaminated_ )
                .Display( tools::translate( "NBC", "Dose:" ), dose_ );
}

// -----------------------------------------------------------------------------
// Name: Contaminations::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Contaminations::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( contaminated_ && viewport.IsHotpointVisible() )
        tools.DrawIcon( xpm_nbc, where, 150.f, gui::GLView_ABC::pixels );
}
