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
#include "MOS_TypePopulation.h"
#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_App.h"

// -----------------------------------------------------------------------------
// Name: MOS_TypePopulation constructor
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
MOS_TypePopulation::MOS_TypePopulation( const std::string& strName, MOS_InputArchive& archive )
    : strName_              ( strName )
    , pModel_               ( 0 )
    , rConcentrationDensity_( 0. )
    , rDefaultFlowDensity_  ( 0. )
    , rMaxSpeed_            ( 0. )
{
	archive.ReadField( "MosID", nID_ );
    std::string strModel;
    archive.ReadField( "ModeleDecisionnel", strModel );

    pModel_ = MOS_App::GetApp().GetAgentManager().FindModel( strModel );
    if( !pModel_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    MOS_App::GetApp().NotifyTypePopulationCreated( *this );

    archive.ReadField( "DensiteConcentration"      , rConcentrationDensity_ );
    archive.ReadField( "DensiteNominaleDeplacement", rDefaultFlowDensity_   );
    archive.ReadField( "VitesseDeplacement"        , rMaxSpeed_             );
}

// -----------------------------------------------------------------------------
// Name: MOS_TypePopulation destructor
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
MOS_TypePopulation::~MOS_TypePopulation( )
{

}