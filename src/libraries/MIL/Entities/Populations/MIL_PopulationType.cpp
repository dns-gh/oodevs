// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/MIL_PopulationType.cpp $
// $Author: Jvt $
// $Modtime: 26/04/05 15:52 $
// $Revision: 7 $
// $Workfile: MIL_PopulationType.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_PopulationType.h"

#include "MIL_Population.h"
#include "MIL_PopulationAttitude.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Decision/DEC_Workspace.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Functions/DEC_PopulationFunctions.h"
#include "Decision/Functions/DEC_ActionFunctions.h"
#include "Decision/Functions/DEC_MiscFunctions.h"
#include "Entities/Populations/Actions/PHY_Population_ActionMove.h"
#include "Entities/Populations/Actions/PHY_Population_ActionFireOnPion.h"
#include "Entities/Populations/Actions/PHY_Population_ActionFireOnPions.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Tools/MIL_Tools.h"
#include "MIL_AgentServer.h"

MIL_PopulationType::T_PopulationMap MIL_PopulationType::populations_;
MT_Float                            MIL_PopulationType::rEffectReloadingTimeDensity_ = 0.;
MT_Float                            MIL_PopulationType::rEffectReloadingTimeFactor_  = 0.;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_PopulationType::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing population types" );

    archive.Section( "Effets" );

    archive.Section( "TempsRechargement" );
    archive.ReadAttribute( "densitePopulation", rEffectReloadingTimeDensity_, CheckValueGreaterOrEqual( 0. ) );
    archive.ReadAttribute( "modificateur"     , rEffectReloadingTimeFactor_ , CheckValueGreaterOrEqual( 1. ) );
    archive.EndSection(); // TempsRechargement

    archive.EndSection(); // Effets


    archive.BeginList( "Populations" );
    while( archive.NextListElement() )
    {
        archive.Section( "Population" );

        std::string strName;
        archive.ReadAttribute( "nom", strName );

        const MIL_PopulationType*& pPopulation = populations_[ strName ];
        if( pPopulation )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Population type already exists", archive.GetContext() );

        pPopulation = new MIL_PopulationType( strName, archive );       
            
        archive.EndSection(); // Population
    }
    archive.EndList(); // Populations
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_PopulationType::Terminate()
{
    for( CIT_PopulationMap it = populations_.begin(); it != populations_.end(); ++it )
        delete it->second;
    populations_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType constructor
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
MIL_PopulationType::MIL_PopulationType( const std::string& strName, MIL_InputArchive& archive )
    : strName_              ( strName )
    , rConcentrationDensity_( 0. )
    , rDefaultFlowDensity_  ( 0. )
    , rMaxSpeed_            ( 0. )
    , pModel_               ( 0 )
    , slowDownData_         ( MIL_PopulationAttitude::GetAttitudes().size(), T_VolumeSlowDownData( PHY_Volume::GetVolumes().size(), sSlowDownData( 0., 0. ) ) )
    , attritionData_        ()
    , damageData_           ( PHY_RoePopulation::GetRoePopulations().size(), 0. )
    , pDIAFunctionTable_    ( new DIA_FunctionTable< MIL_Population >() )
{
    archive.ReadField( "MosID"                     , nID_                   );
    archive.ReadField( "DensiteConcentration"      , rConcentrationDensity_, CheckValueGreater( 0. ) );
    archive.ReadField( "DensiteNominaleDeplacement", rDefaultFlowDensity_  , CheckValueGreater( 0. ) );
    archive.ReadField( "VitesseDeplacement"        , rMaxSpeed_            , CheckValueGreater( 0. ) );
    rMaxSpeed_ = MIL_Tools::ConvertSpeedMosToSim( rMaxSpeed_ );

    archive.Section( "Effets" );
    InitializeSlowDownData( archive );
    InitializeFireData    ( archive );
    archive.EndSection(); // Effets
    
    std::string strModel;
    archive.ReadField( "ModeleDecisionnel", strModel );
    pModel_ = MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().FindModelPopulation( strModel );
    if( !pModel_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown population model", archive.GetContext() );

    InitializeDiaFunctions();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_PopulationType::~MIL_PopulationType()
{
    delete pDIAFunctionTable_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::InitializeSlowDownData
// Created: NLD 2005-10-20
// -----------------------------------------------------------------------------
void MIL_PopulationType::InitializeSlowDownData( MIL_InputArchive& archive )
{
    archive.Section( "Ralentissement" );

    if( !archive.BeginList( "Attitudes", MIL_InputArchive::eNothing ) )
    {
        archive.EndSection(); // Ralentissement
        return;
    }

    while( archive.NextListElement() )
    {
        archive.Section( "Attitude" );
        std::string strAttitude;
        archive.ReadAttribute( "nom", strAttitude );

        const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( strAttitude );
        if( !pAttitude )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown attitude '%s'", strAttitude.c_str() ), archive.GetContext() );

        assert( slowDownData_.size() > pAttitude->GetID() );
        T_VolumeSlowDownData& volumeSlowDownData = slowDownData_[ pAttitude->GetID() ];

        archive.BeginList( "VolumesPions" );
        while( archive.NextListElement() )
        {
            archive.Section( "VolumePion" );

            std::string strVolume;
            MT_Float    rPopulationDensity = 0.;
            MT_Float    rMaxSpeed          = 0.;

            archive.ReadAttribute( "nom", strVolume );
            archive.ReadAttribute( "densitePopulation", rPopulationDensity, CheckValueGreater       ( 0. ) );
            archive.ReadAttribute( "vitesseMaximale"  , rMaxSpeed         , CheckValueGreaterOrEqual( 0. ) );
            rMaxSpeed = MIL_Tools::ConvertSpeedMosToSim( rMaxSpeed );

            const PHY_Volume* pVolume = PHY_Volume::FindVolume( strVolume );
            if( !pVolume )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown volume '%s'", strVolume.c_str() ), archive.GetContext() );

            assert( volumeSlowDownData.size() > pVolume->GetID() );
            volumeSlowDownData[ pVolume->GetID() ] = sSlowDownData( rPopulationDensity, rMaxSpeed );

            archive.EndSection(); // VolumePion
        }
        archive.EndList(); // VolumesPions
        archive.EndSection(); // Attitude
    }
    archive.EndList(); // Attitudes
    archive.EndSection(); // Ralentissement
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::InitializeFireData
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
void MIL_PopulationType::InitializeFireData( MIL_InputArchive& archive )
{
    archive.Section( "Tir" );

    archive.Section( "Tireur" );
    attritionData_.Initialize( archive );
    archive.EndSection(); // Tireur

    archive.Section( "Cible" ); 
    if( archive.BeginList( "ReglesEngagementTireur", MIL_InputArchive::eNothing ) )
    {
        while( archive.NextListElement() )
        {
            archive.Section( "RegleEngagementTireur" );

            std::string strRoe;
            archive.ReadAttribute( "nom", strRoe );
            const PHY_RoePopulation* pRoe = PHY_RoePopulation::Find( strRoe );
            if( !pRoe )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown population roe '%s'", strRoe.c_str() ), archive.GetContext() );
            
            assert( damageData_.size() > pRoe->GetID() );

            archive.ReadField( "SurfaceAttrition", damageData_[ pRoe->GetID() ], CheckValueGreaterOrEqual( 0. ) );

            archive.EndSection(); // RegleEngagementTireur
        }
        archive.EndList(); // ReglesEngagementTireur
    }
    archive.EndSection(); // Cible

    archive.EndSection(); // Tir
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::InitializeDiaFunctions
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
void MIL_PopulationType::InitializeDiaFunctions()
{
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StopAction   < MIL_Population                   >, "DEC_StopAction"       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::SuspendAction< MIL_Population                   >, "DEC_PauseAction"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::ResumeAction < MIL_Population                   >, "DEC_ReprendAction"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_Population_ActionMove        >, "DEC_StartDeplacement" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_Population_ActionFireOnPions >, "DEC_StartTirSurPions" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_Population_ActionFireOnPion  >, "DEC_StartTirSurPion"  );

    // Knowledge
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::GetKnowledgeAgentRoePopulation, "DEC_ConnaissanceAgent_RoePopulation"   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::GetPionsAttacking             , "DEC_Connaissances_PionsPrenantAPartie" );

    // Debug
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::Debug, "DEC_Debug" );

    // RC
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::RC_Operational< MIL_Population >, "DEC_RC"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::RC_Message    < MIL_Population >, "DEC_Message" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::RC_Warning    < MIL_Population >, "DEC_Warning" );

    // Effects
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::SetPionMaxSpeed  , "DEC_Population_RalentissementPion_ChangeVitesse"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::ResetPionMaxSpeed, "DEC_Population_RalentissementPion_VitesseParDefaut" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::SetAttitude      , "DEC_Population_ChangerAttitude" );
} 

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::InstanciatePopulation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_Population& MIL_PopulationType::InstanciatePopulation( uint nID, MIL_InputArchive& archive ) const
{
    return *new MIL_Population( *this, nID, archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetPionMaxSpeed
// Created: NLD 2005-10-20
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationType::GetPionMaxSpeed( const MIL_PopulationAttitude& populationAttitude, MT_Float rPopulationDensity, const PHY_Volume& pionVolume ) const
{
    assert( slowDownData_.size() > populationAttitude.GetID() );
    const T_VolumeSlowDownData& volumeSlowDownData = slowDownData_[ populationAttitude.GetID() ];

    assert( volumeSlowDownData.size() > pionVolume.GetID() );
    const sSlowDownData& slowDownData = volumeSlowDownData[ pionVolume.GetID() ];

    if( rPopulationDensity == 0. || slowDownData.rPopulationDensity_ == 0. )
        return std::numeric_limits< MT_Float >::max();

    return ( slowDownData.rMaxSpeed_ * slowDownData.rPopulationDensity_ ) / rPopulationDensity;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetPionReloadingTimeFactor
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationType::GetPionReloadingTimeFactor( MT_Float rPopulationDensity ) const
{
    if( rEffectReloadingTimeDensity_ == 0. )
        return 1.;

    return std::max( 1., rPopulationDensity * rEffectReloadingTimeFactor_ / rEffectReloadingTimeDensity_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetDamageSurface
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationType::GetDamageSurface( const PHY_RoePopulation& roeFirer ) const
{
    assert( damageData_.size() > roeFirer.GetID() );
    return damageData_[ roeFirer.GetID() ];
}
