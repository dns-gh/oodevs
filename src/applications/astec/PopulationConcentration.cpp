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

#include "PopulationPart_ABC.h"
#include "App.h"
#include "World.h"
#include "Population.h"

// -----------------------------------------------------------------------------
// Name: PopulationConcentration constructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationConcentration::PopulationConcentration( const ASN1T_MsgPopulationConcentrationCreation& asnMsg, const Population& parent )
    : PopulationPart_ABC( asnMsg.oid_concentration, parent )	
    , position_		        ( 0, 0 )
    , strName_              ( "Concentration" )
{
	App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.position.data, position_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration constructor
// Created: HME 2005-10-18
// -----------------------------------------------------------------------------
PopulationConcentration::PopulationConcentration( MT_Vector2D point, E_PopulationAttitude attitude , int persons, const Population& parent  )
:   PopulationPart_ABC  ( 0, parent, attitude, persons )	
,   position_		        ( point )
,   strName_                ( "Concentration" )
{
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


// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetDensity
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
MT_Float PopulationConcentration::GetDensity() const
{
    return parent_.GetType().GetConcentrationDensity();   
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetArea
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
MT_Float PopulationConcentration::GetArea() const
{
    return GetLivingHumans() / GetDensity();
}
