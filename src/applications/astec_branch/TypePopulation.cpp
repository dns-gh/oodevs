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

#include "astec_pch.h"
#include "TypePopulation.h"
#include "App.h"
#include "AgentManager.h"
#include "App.h"

// -----------------------------------------------------------------------------
// Name: TypePopulation constructor
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
TypePopulation::TypePopulation( const std::string& strName, InputArchive& archive )
    : strName_              ( strName )
    , pModel_               ( 0 )
    , rConcentrationDensity_( 0. )
    , rDefaultFlowDensity_  ( 0. )
    , rMaxSpeed_            ( 0. )
{
	archive.ReadField( "MosID", nID_ );
    std::string strModel;
    archive.ReadField( "ModeleDecisionnel", strModel );

    pModel_ = App::GetApp().GetAgentManager().FindModel( strModel );
    if( !pModel_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    App::GetApp().NotifyTypePopulationCreated( *this );

    archive.ReadField( "DensiteConcentration"      , rConcentrationDensity_ );
    archive.ReadField( "DensiteNominaleDeplacement", rDefaultFlowDensity_   );
    archive.ReadField( "VitesseDeplacement"        , rMaxSpeed_             );
}

// -----------------------------------------------------------------------------
// Name: TypePopulation destructor
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
TypePopulation::~TypePopulation( )
{

}