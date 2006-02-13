// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Contaminations.h"

// -----------------------------------------------------------------------------
// Name: Contaminations constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Contaminations::Contaminations( Controller& controller )
    : controller_( controller )    
{

}

// -----------------------------------------------------------------------------
// Name: Contaminations destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Contaminations::~Contaminations()
{

}

// -----------------------------------------------------------------------------
// Name: Contaminations::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Contaminations::Update( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.etat_contaminationPresent )
        nContamination_ = message.etat_contamination;

    if( message.m.contamine_par_agents_nbcPresent )
    {
        contaminatingNbcAgents_.clear();
        contaminatingNbcAgents_.resize( message.contamine_par_agents_nbc.n );
        for( uint i = 0; i < message.contamine_par_agents_nbc.n; ++i )
            contaminatingNbcAgents_.push_back( message.contamine_par_agents_nbc.elem[i] );
    }

    if( message.m.en_tenue_de_protection_nbcPresent )
        bNbcProtectionSuitWorn_ = message.en_tenue_de_protection_nbc;

    controller_.Update( *this );
}
