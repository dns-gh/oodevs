// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationConcentration.h"
#include "CoordinateConverter.h"

// -----------------------------------------------------------------------------
// Name: PopulationConcentration constructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationConcentration::PopulationConcentration( const ASN1T_MsgPopulationConcentrationCreation& asnMsg, const CoordinateConverter& converter, float density )
    : position_( converter.ConvertToXY( asnMsg.position ) )
    , density_ ( density )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration destructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationConcentration::~PopulationConcentration()
{
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetName
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
std::string PopulationConcentration::GetName() const
{
    static std::string concentration( "Concentration" );
    return concentration;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::DoUpdate
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
void PopulationConcentration::DoUpdate( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
    if( asnMsg.m.nb_humains_vivantsPresent )
        nLivingHumans_ = asnMsg.nb_humains_vivants;

    if( asnMsg.m.nb_humains_mortsPresent )
        nDeadHumans_ = asnMsg.nb_humains_morts;

    PopulationPart_ABC::DoUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetLivingHumans
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
unsigned int PopulationConcentration::GetLivingHumans() const
{
    return nLivingHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetDeadHumans
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
unsigned int PopulationConcentration::GetDeadHumans() const
{
    return nDeadHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetDensity
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
unsigned int PopulationConcentration::GetDensity() const
{
    return density_;
}
