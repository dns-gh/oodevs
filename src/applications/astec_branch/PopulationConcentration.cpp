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

#include "App.h"
#include "World.h"

// -----------------------------------------------------------------------------
// Name: PopulationConcentration constructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationConcentration::PopulationConcentration( const ASN1T_MsgPopulationConcentrationCreation& asnMsg )
    : position_		        ( 0, 0 )
    , strName_              ( "Concentration" )
{
	App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.position.data, position_ );
}


// -----------------------------------------------------------------------------
// Name: PopulationConcentration destructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationConcentration::~PopulationConcentration()
{
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::Update
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
void PopulationConcentration::Update( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
	if ( asnMsg.m.nb_humains_vivantsPresent )
		nLivingHumans_ = asnMsg.nb_humains_vivants;

	if ( asnMsg.m.nb_humains_mortsPresent )
	    nDeadHumans_ = asnMsg.nb_humains_morts;

	if ( asnMsg.m.attitudePresent )
		attitude_ = (E_PopulationAttitude)asnMsg.attitude;
}
