// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-29 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_InteractionManager.cpp $
// $Author: Age $
// $Modtime: 15/12/04 12:03 $
// $Revision: 4 $
// $Workfile: HLA_InteractionManager.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HLA_InteractionManager.h"
#include "HLA_DirectFire.h"
#include "HLA_IndirectFire.h"
#include "HLA_Neutralization.h"
#include "HLA_Activation.h"
#include "HLA_Bypassing.h"
#include "HLA_Construction.h"
#include "HLA_Mining.h"
#include <hla/Interaction.h>
#include <hla/Federate.h>
#include <hla/InteractionIdentifier.h>

using namespace hla;

// -----------------------------------------------------------------------------
// Name: HLA_InteractionManager constructor
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
HLA_InteractionManager::HLA_InteractionManager( Federate& federate )
    : pDirectFireInteraction_    ( 0 )
    , pIndirectFireInteraction_  ( 0 )
    , pNeutralizationInteraction_( 0 )
    , pActivationInteraction_    ( 0 )
    , pBypassingInteraction_     ( 0 )
    , pConstructionInteraction_  ( 0 )
    , pMiningInteraction_        ( 0 )
{
    pDirectFireInteraction_     = & HLA_DirectFire::CreateInteractionClass( *this );
    pIndirectFireInteraction_   = & HLA_IndirectFire::CreateInteractionClass( *this );
    pNeutralizationInteraction_ = & HLA_Neutralization::CreateInteractionClass( *this );
    pActivationInteraction_     = & HLA_Activation::CreateInteractionClass( *this );
    pBypassingInteraction_      = & HLA_Bypassing::CreateInteractionClass( *this );
    pConstructionInteraction_   = & HLA_Construction::CreateInteractionClass( *this );
    pMiningInteraction_         = & HLA_Mining::CreateInteractionClass( *this );

    federate.Register( InteractionIdentifier( "InteractionCiblee.Attrition.TirDirect" ), *pDirectFireInteraction_ );
    federate.Register( InteractionIdentifier( "InteractionCiblee.Attrition.TirIndirect" ), *pIndirectFireInteraction_ );
    federate.Register( InteractionIdentifier( "InteractionCiblee.Neutralisation" ), *pNeutralizationInteraction_ );
    federate.Register( InteractionIdentifier( "InteractionObjet.Activation" ), *pActivationInteraction_ );
    federate.Register( InteractionIdentifier( "InteractionObjet.Contournement" ), *pBypassingInteraction_ );
    federate.Register( InteractionIdentifier( "InteractionObjet.Construction" ), *pConstructionInteraction_ );
    federate.Register( InteractionIdentifier( "InteractionObjet.Valorisation" ), *pMiningInteraction_ );
}

// -----------------------------------------------------------------------------
// Name: HLA_InteractionManager destructor
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
HLA_InteractionManager::~HLA_InteractionManager()
{
    delete pMiningInteraction_;
    delete pConstructionInteraction_;
    delete pBypassingInteraction_;
    delete pActivationInteraction_;
    delete pNeutralizationInteraction_;
    delete pIndirectFireInteraction_;
    delete pDirectFireInteraction_;
}

// -----------------------------------------------------------------------------
// Name: HLA_InteractionManager::Send
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
void HLA_InteractionManager::Send( const HLA_DirectFire& interaction )
{
    assert( pDirectFireInteraction_ );
    pDirectFireInteraction_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: HLA_InteractionManager::Send
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
void HLA_InteractionManager::Send( const HLA_IndirectFire& interaction )
{
    assert( pIndirectFireInteraction_ );
    pIndirectFireInteraction_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: HLA_InteractionManager::Send
// Created: AGE 2004-12-02
// -----------------------------------------------------------------------------
void HLA_InteractionManager::Send( const HLA_Neutralization& interaction )
{
    assert( pNeutralizationInteraction_ );
    pNeutralizationInteraction_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: HLA_InteractionManager::Send
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_InteractionManager::Send( const HLA_Activation& interaction )
{
    assert( pActivationInteraction_ );
    pActivationInteraction_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: HLA_InteractionManager::Send
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_InteractionManager::Send( const HLA_Bypassing& interaction )
{
    assert( pBypassingInteraction_ );
    pBypassingInteraction_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: HLA_InteractionManager::Send
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_InteractionManager::Send( const HLA_Construction& interaction )
{
    assert( pConstructionInteraction_ );
    pConstructionInteraction_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: HLA_InteractionManager::Send
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_InteractionManager::Send( const HLA_Mining& interaction )
{
    assert( pMiningInteraction_ );
    pMiningInteraction_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: HLA_InteractionManager::Receive
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
void HLA_InteractionManager::Receive( HLA_DirectFire& interaction )
{
    interaction.Execute();
}

// -----------------------------------------------------------------------------
// Name: HLA_InteractionManager::Receive
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
void HLA_InteractionManager::Receive( HLA_IndirectFire& interaction )
{
    interaction.Execute();
}

// -----------------------------------------------------------------------------
// Name: HLA_InteractionManager::Receive
// Created: AGE 2004-12-02
// -----------------------------------------------------------------------------
void HLA_InteractionManager::Receive( HLA_Neutralization& interaction )
{
    interaction.Execute();
}

// -----------------------------------------------------------------------------
// Name: HLA_InteractionManager::Receive
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_InteractionManager::Receive( HLA_Activation& interaction )
{
    interaction.Execute();
}
    
// -----------------------------------------------------------------------------
// Name: HLA_InteractionManager::Receive
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_InteractionManager::Receive( HLA_Bypassing& interaction )
{
    interaction.Execute();
}
    
// -----------------------------------------------------------------------------
// Name: HLA_InteractionManager::Receive
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_InteractionManager::Receive( HLA_Construction& interaction )
{
    interaction.Execute();
}
    
// -----------------------------------------------------------------------------
// Name: HLA_InteractionManager::Receive
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_InteractionManager::Receive( HLA_Mining& interaction )
{
    interaction.Execute();
}
