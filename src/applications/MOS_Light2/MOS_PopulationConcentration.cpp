// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_PopulationConcentration.h"

#include "MOS_App.h"
#include "MOS_World.h"

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentration constructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
MOS_PopulationConcentration::MOS_PopulationConcentration( uint id, MOS_Population* parent )
: 	parent_			( parent )
,	nID_			( id )
,	center_			( MT_Vector2D( 0, 0 ) )
{
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentration destructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
MOS_PopulationConcentration::~MOS_PopulationConcentration()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentration::Update
/** @param  asnMsg 
*/
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
void MOS_PopulationConcentration::Update( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
	nLivingHumans_		= asnMsg.nb_humains_vivants;
	nDeadHumans_		= asnMsg.nb_humains_morts;
	attitude_			= asnMsg.attitude;
	for( uint i = 0; i < asnMsg.forme.vecteur_point.n; ++i )
    {
        MT_Vector2D vTmp;
        MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.forme.vecteur_point.elem[i].data, vTmp );
        forme_.push_back( vTmp );
        center_ += vTmp;
    }
	    if( forme_.size() > 1 )
        center_ /= forme_.size();
}

