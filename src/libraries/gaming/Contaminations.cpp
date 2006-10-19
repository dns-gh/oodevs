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
#include "clients_kernel/PropertiesDictionary.h"
#include "statusicons.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Contaminations constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Contaminations::Contaminations( Controller& controller, const Resolver_ABC< NBCAgent >& resolver, PropertiesDictionary& dico )
    : controller_( controller )
    , resolver_( resolver )
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
    dico.Register( *this, tools::translate( "NBC", "NBC/Tenue NBC mise" ), bNbcProtectionSuitWorn_ );
    dico.Register( *this, tools::translate( "NBC", "NBC/Agents contaminants" ), contaminatingNbcAgents_ );
    dico.Register( *this, tools::translate( "NBC", "NBC/Niveau de contamination" ), nContamination_ );
}

// -----------------------------------------------------------------------------
// Name: Contaminations::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Contaminations::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.etat_contaminationPresent )
        nContamination_ = message.etat_contamination;

    if( message.m.contamine_par_agents_nbcPresent )
    {
        contaminatingNbcAgents_.clear();
        contaminatingNbcAgents_.reserve( message.contamine_par_agents_nbc.n );
        for( uint i = 0; i < message.contamine_par_agents_nbc.n; ++i )
            contaminatingNbcAgents_.push_back( &resolver_.Get( message.contamine_par_agents_nbc.elem[i] ) );
    }

    if( message.m.en_tenue_de_protection_nbcPresent )
        bNbcProtectionSuitWorn_ = message.en_tenue_de_protection_nbc;

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Contaminations::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void Contaminations::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "NBC", "NBC" ) )
                .Display( tools::translate( "NBC", "Tenue NBC:" ), 
                        bNbcProtectionSuitWorn_ ? tools::translate( "NBC", "Mise" ) : tools::translate( "NBC", "Non mise" ) ) // $$$$ AGE 2006-02-22: Boolean
                .Display( tools::translate( "NBC", "Agents contaminants:" ), contaminatingNbcAgents_ )
                .Display( tools::translate( "NBC", "Contamination:" ), nContamination_ );
}

// -----------------------------------------------------------------------------
// Name: Contaminations::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Contaminations::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( ! contaminatingNbcAgents_.empty() && viewport.IsInside( where ) )
        tools.DrawIcon( xpm_nbc, where, 150.f );
}
