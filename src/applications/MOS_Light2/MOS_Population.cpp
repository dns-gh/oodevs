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
#include "MOS_Population.h"

#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_Team.h"
#include "MOS_TypePopulation.h"
#include "MOS_World.h"

#include "MOS_PopulationFlow.h"
#include "MOS_PopulationConcentration.h"

MIL_AgentID MOS_Population::nMaxId_ = 7000000;

// -----------------------------------------------------------------------------
// Name: MOS_Population constructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
MOS_Population::MOS_Population( const ASN1T_MsgPopulationCreation& asnMsg )
    : nPopulationID_( asnMsg.oid_population )
    , strName_      ( asnMsg.nom )
    , pType_        ( MOS_App::GetApp().GetAgentManager().FindTypePopulation( asnMsg.type_population ) )
    , pTeam_        ( MOS_App::GetApp().GetAgentManager().FindTeam( asnMsg.oid_camp ) )
{
    assert( pType_ );
    assert( pTeam_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_Population constructor
// Created: HME 2005-10-18
// -----------------------------------------------------------------------------
MOS_Population::MOS_Population()
    : nPopulationID_( nMaxId_++ )
    , pType_        ( 0 )
    , pTeam_        ( 0 )
    , strName_      ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_Population constructor
// Created: HME 2005-10-18
// -----------------------------------------------------------------------------
MOS_Population::MOS_Population( MT_Vector2D point, E_PopulationAttitude attitude, int persons, MOS_Team& team , std::string name, MOS_TypePopulation* type )
    : nPopulationID_( nMaxId_++ )
    , pType_        ( type )
    , pTeam_        ( &team )
    , strName_      ( name )
{
    CreatePopulationConcentration( point, attitude, persons );
}

// -----------------------------------------------------------------------------
// Name: MOS_Population destructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
MOS_Population::~MOS_Population()
{
	for( IT_ConcentrationMap itCon = concentrationMap_.begin(); itCon != concentrationMap_.end(); ++itCon )
		delete itCon->second;
	concentrationMap_.clear();

	for( IT_FlowMap itFlow = flowMap_.begin(); itFlow != flowMap_.end(); ++itFlow )
		delete itFlow->second;
	flowMap_.clear();
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::UpdatePopulationFlow			
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void MOS_Population::UpdatePopulationFlow( const ASN1T_MsgPopulationFluxUpdate& asnMsg )
{
	flowMap_[ asnMsg.oid_flux ]->Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::UpdatePopulationConcentration	
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void MOS_Population::UpdatePopulationConcentration( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
	concentrationMap_[ asnMsg.oid_concentration ]->Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::CreatePopulationFlow			
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void MOS_Population::CreatePopulationFlow( const ASN1T_MsgPopulationFluxCreation& asnMsg )
{
	MOS_PopulationFlow* pFlow = new MOS_PopulationFlow( asnMsg, *this );
	flowMap_[ asnMsg.oid_flux ] = pFlow;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::CreatePopulationConcentration	
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void MOS_Population::CreatePopulationConcentration( const ASN1T_MsgPopulationConcentrationCreation& asnMsg )
{
	MOS_PopulationConcentration* pCon = new MOS_PopulationConcentration( asnMsg, *this );
	concentrationMap_[ asnMsg.oid_concentration ] = pCon ;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::CreatePopulationConcentration
// Created: HME 2005-10-18
// -----------------------------------------------------------------------------
void MOS_Population::CreatePopulationConcentration( MT_Vector2D point, E_PopulationAttitude attitude, int persons )
{
    MOS_PopulationConcentration* pCon = new MOS_PopulationConcentration( point, attitude, persons, *this );
    concentrationMap_[ nMaxId_  ] = pCon ;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::DeletePopulationFlow			
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void MOS_Population::DeletePopulationFlow( const ASN1T_MsgPopulationFluxDestruction& asnMsg )
{
    IT_FlowMap it = flowMap_.find( asnMsg.oid_flux );
    assert( it != flowMap_.end() );
    MOS_App::GetApp().NotifyPopulationFlowDeleted( *it->second );
    flowMap_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::DeletePopulationConcentration	
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void MOS_Population::DeletePopulationConcentration( const ASN1T_MsgPopulationConcentrationDestruction& asnMsg )
{
    IT_ConcentrationMap it = concentrationMap_.find( asnMsg.oid_concentration );
    assert( it != concentrationMap_.end() );
    MOS_App::GetApp().NotifyPopulationConcentrationDeleted( *it->second );
    concentrationMap_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::UpdatePopulation				
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void MOS_Population::UpdatePopulation( const ASN1T_MsgPopulationUpdate& /*asnMsg*/ )
{
} 


// -----------------------------------------------------------------------------
// Name: MOS_Population::MT_Vector2D& GetPos
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
const MT_Vector2D& MOS_Population::GetPos() const
{
	if ( concentrationMap_.size() == 0 && flowMap_.size() == 0 )
		assert( false );
	if( flowMap_.size() != 0 )
        return flowMap_.begin()->second->GetHeadPosition();
	else
        return concentrationMap_.begin()->second->GetPos();
		
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetLivingHumans
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
uint MOS_Population::GetLivingHumans() const
{
	uint sum = 0;
	for( CIT_ConcentrationMap itCon = concentrationMap_.begin(); itCon != concentrationMap_.end(); ++itCon )
		sum += itCon->second->GetLivingHumans();
	for( CIT_FlowMap itFlow = flowMap_.begin(); itFlow != flowMap_.end(); ++itFlow )
		sum += itFlow->second->GetLivingHumans();
	return sum;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetDeadHumans
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
uint MOS_Population::GetDeadHumans() const
{
	uint sum = 0;
	for( CIT_ConcentrationMap itCon = concentrationMap_.begin(); itCon != concentrationMap_.end(); ++itCon )
		sum += itCon->second->GetDeadHumans();
	for( CIT_FlowMap itFlow = flowMap_.begin(); itFlow != flowMap_.end(); ++itFlow )
		sum += itFlow->second->GetDeadHumans();
	return sum;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::ReadODB
// Created: HME 2005-10-18
// -----------------------------------------------------------------------------
void MOS_Population::ReadODB( MOS_InputArchive& archive )
{
    archive.Section( "Population" );

    std::string strCategory;
    
    archive.ReadAttribute( "type", strCategory );
    pType_ = MOS_App::GetApp().GetAgentManager().FindTypePopulation( strCategory );
    assert( pType_ );

    archive.ReadAttribute( "id", nPopulationID_ );
    if( nPopulationID_ > nMaxId_ )
        nMaxId_ = nPopulationID_;

    if( ! archive.ReadField( "Nom", strName_, MOS_InputArchive::eNothing ) )
        strName_ = strCategory;

    std::string strPos;
    MT_Vector2D vPos;
    archive.ReadField( "Position", strPos );
    MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( strPos, vPos );

    std::string strAtt;
    E_PopulationAttitude eAttitude;
    archive.ReadField( "Attitude", strAtt );
    eAttitude = ENT_Tr::ConvertToPopulationAttitude( strAtt );

    std::string strTeam;
    archive.ReadField( "Camp", strTeam );
    pTeam_ = MOS_App::GetApp().GetAgentManager().FindTeam( strTeam );

    uint nPersons;
    archive.ReadField( "NombreHumains", nPersons );

    CreatePopulationConcentration( vPos, eAttitude , nPersons );
    
    archive.EndSection();
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::WriteODB
// Created: HME 2005-10-18
// -----------------------------------------------------------------------------
void MOS_Population::WriteODB( MT_XXmlOutputArchive& archive )
{
    archive.Section( "Population" );

    archive.WriteAttribute( "type", pType_->GetName() );
    archive.WriteAttribute( "id", nPopulationID_ );
    archive.WriteField    ( "Nom", strName_ );
    archive.WriteField    ( "Camp", pTeam_->GetName() );

    MOS_PopulationConcentration* pCon = concentrationMap_.begin()->second;
    std::string strPos;
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( pCon->GetPos() , strPos );
    archive.WriteField( "Position"     , strPos );
    archive.WriteField( "NombreHumains", pCon->GetLivingHumans() );
    archive.WriteField( "Attitude"     , ENT_Tr::ConvertFromPopulationAttitude( pCon->GetAttitude() ) );
    
    archive.EndSection();
}

