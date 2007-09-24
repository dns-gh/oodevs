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

#include "simulation_kernel_pch.h"

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

#include "xeumeuleu/xml.h"

using namespace xml;

MIL_PopulationType::T_PopulationMap MIL_PopulationType::populations_;
MT_Float                            MIL_PopulationType::rEffectReloadingTimeDensity_ = 0.;
MT_Float                            MIL_PopulationType::rEffectReloadingTimeFactor_  = 0.;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

struct MIL_PopulationType::LoadingWrapper
{
    void ReadPopulation( xml::xistream& xis )
    {
        MIL_PopulationType::ReadPopulation( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_PopulationType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing population types" );

    xis >> start( "populations" )
            >> start( "reloading-time-effect" )
                >> attribute( "population-density", rEffectReloadingTimeDensity_ )
                >> attribute( "modifier", rEffectReloadingTimeFactor_ )
            >> end();

    if( rEffectReloadingTimeDensity_ < 0 )
        xis.error( "reloading-time-effet: population-density < 0" );
    if( rEffectReloadingTimeFactor_ < 1 )
        xis.error( "reloading-time-effect: modifier < 1" );

    LoadingWrapper loader;

    xis     >> list( "population", loader, &LoadingWrapper::ReadPopulation )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::ReadPopulation
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void MIL_PopulationType::ReadPopulation( xml::xistream& xis )
{
    std::string strName;
    xis >> attribute( "name", strName );

    const MIL_PopulationType*& pPopulation = populations_[ strName ];
    if( pPopulation )
        xis.error( "Population type already exists" );

    pPopulation = new MIL_PopulationType( strName, xis );
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
MIL_PopulationType::MIL_PopulationType( const std::string& strName, xml::xistream& xis )
    : strName_              ( strName )
    , rConcentrationDensity_( 0. )
    , rDefaultFlowDensity_  ( 0. )
    , rMaxSpeed_            ( 0. )
    , pModel_               ( 0 )
    , slowDownData_         ( MIL_PopulationAttitude::GetAttitudes().size(), T_VolumeSlowDownData( PHY_Volume::GetVolumes().size(), sSlowDownData( 0., 0. ) ) )
    , attritionData_        ()
    , damageData_           ( PHY_RoePopulation::GetRoePopulations().size(), sDamageData( 0., 0. ) )
    , pDIAFunctionTable_    ( new DIA_FunctionTable< MIL_Population >() )
{
    xis >> attribute( "id", nID_ )
        >> attribute( "concentration-density", rConcentrationDensity_ )
        >> attribute( "moving-base-density", rDefaultFlowDensity_ )
        >> attribute( "moving-speed", rMaxSpeed_ );

    if( rConcentrationDensity_ <= 0 )
        xis.error( "population: concentration-density <= 0" );
    if( rDefaultFlowDensity_ <= 0 )
        xis.error( "population: moving-base-density <= 0" );
    if( rMaxSpeed_ <= 0 )
        xis.error( "population: moving-speed" );

    rMaxSpeed_ = MIL_Tools::ConvertSpeedMosToSim( rMaxSpeed_ );

    InitializeSlowDownData( xis );
    InitializeFireData    ( xis );

    std::string strModel;
    xis >> attribute( "decisional-model", strModel );
    pModel_ = MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().FindModelPopulation( strModel );
    if( !pModel_ )
        xis.error( "Unknown population model" );

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
void MIL_PopulationType::InitializeSlowDownData( xml::xistream& xis )
{
    xis >> start( "slowing-effects" )
            >> list( "slowing-effect", *this, &MIL_PopulationType::ReadSlowingEffect )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::ReadSlowingEffect
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void MIL_PopulationType::ReadSlowingEffect( xml::xistream& xis )
{
    std::string strAttitude;
    xis >> attribute( "population-attitude", strAttitude );

    const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( strAttitude );
    if( !pAttitude )
        xis.error( "Unknown attitude '" + strAttitude + "'" );

    assert( slowDownData_.size() > pAttitude->GetID() );
    T_VolumeSlowDownData& volumeSlowDownData = slowDownData_[ pAttitude->GetID() ];

    xis >> list( "unit", *this, &MIL_PopulationType::ReadSlowingUnitEffect, volumeSlowDownData );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::ReadSlowingUnitEffect
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void MIL_PopulationType::ReadSlowingUnitEffect( xml::xistream& xis, T_VolumeSlowDownData& volumeSlowDownData )
{
    std::string strVolume;
    MT_Float    rPopulationDensity = 0.;
    MT_Float    rMaxSpeed          = 0.;

    xis >> attribute( "unit-size", strVolume )
        >> attribute( "population-density", rPopulationDensity )
        >> attribute( "max-speed", rMaxSpeed );

    if( rPopulationDensity <= 0 )
        xis.error( "unit: population-density <= 0" );
    if( rMaxSpeed < 0 )
        xis.error( "unit: max-speed < 0" );

    rMaxSpeed = MIL_Tools::ConvertSpeedMosToSim( rMaxSpeed );

    const PHY_Volume* pVolume = PHY_Volume::FindVolume( strVolume );
    if( !pVolume )
        xis.error( "Unknown volume '" + strVolume + "'" );

    assert( volumeSlowDownData.size() > pVolume->GetID() );
    volumeSlowDownData[ pVolume->GetID() ] = sSlowDownData( rPopulationDensity, rMaxSpeed );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::InitializeFireData
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
void MIL_PopulationType::InitializeFireData( xml::xistream& xis )
{
    xis >> start( "attrition-effects" );
    attritionData_.Initialize( xis );
    xis >> end();

    xis >> start( "unit-fire-effects" )
            >> list( "unit", *this, &MIL_PopulationType::ReadUnitFireEffect )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::ReadUnitFireEffect
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void MIL_PopulationType::ReadUnitFireEffect( xml::xistream& xis )
{
    std::string strRoe;
    xis >> attribute( "rule-of-engagment", strRoe );
    const PHY_RoePopulation* pRoe = PHY_RoePopulation::Find( strRoe );
    if( !pRoe )
        xis.error( "Unknown population roe '" + strRoe + "'" );

    assert( damageData_.size() > pRoe->GetID() );

    xis >> attribute( "attrition-surface", damageData_[ pRoe->GetID() ].rAttritionSurface_ )
        >> attribute( "ph", damageData_[ pRoe->GetID() ].rPH_ );

    if( damageData_[ pRoe->GetID() ].rAttritionSurface_ < 0 )
        xis.error( "unit-fire-effect: rule-of-engagment < 0" );
    if( damageData_[ pRoe->GetID() ].rPH_ < 0 )
        xis.error( "unit-fire-effect: ph < 0" );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::InitializeDiaFunctions
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
void MIL_PopulationType::InitializeDiaFunctions()
{
    // Actions
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StopAction   < MIL_Population                   >, "DEC_StopAction"       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::SuspendAction< MIL_Population                   >, "DEC_PauseAction"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::ResumeAction < MIL_Population                   >, "DEC_ReprendAction"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_Population_ActionMove        >, "DEC_StartDeplacement" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_Population_ActionFireOnPions >, "DEC_StartTirSurPions" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_Population_ActionFireOnPion  >, "DEC_StartTirSurPion"  );

    // Knowledge agents
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::GetKnowledgeAgentRoePopulation, "DEC_ConnaissanceAgent_RoePopulation"   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::GetPionsAttacking             , "DEC_Connaissances_PionsPrenantAPartie" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::GetPionsSecuring              , "DEC_Connaissances_PionsSecurisant"     );

    // Knowledge objects
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::GetKnowledgeObjectLocalisation, "DEC_ConnaissanceObjet_Localisation"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::IsKnowledgeObjectValid        , "DEC_ConnaissanceObjet_EstValide"       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::GetObjectsInZone              , "DEC_Connaissances_ObjetsDansZone"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::DamageObject                  , "DEC_ConnaissanceObjet_Degrader"        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::GetKnowledgeObjectDistance    , "DEC_ConnaissanceObjet_Distance"        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::GetKnowledgeObjectClosestPoint, "DEC_ConnaissanceObjet_PointPlusProche" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::IsEnemy                       , "DEC_ConnaissanceObjet_EstEnnemi"       );

    // Debug
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::DebugDrawPoint , "DEC_DebugAffichePoint"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::DebugDrawPoints, "DEC_DebugAffichePoints" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::Debug          , "DEC_Debug"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::Trace          , "DEC_Trace"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::DecisionalState, "DEC_DecisionalState"    );

    // RC
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::RC_Operational< MIL_Population >, "DEC_RC"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::RC_Message    < MIL_Population >, "DEC_Message" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::RC_Warning    < MIL_Population >, "DEC_Warning" );

    // Effects
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::SetPionMaxSpeed  , "DEC_Population_RalentissementPion_ChangeVitesse"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::ResetPionMaxSpeed, "DEC_Population_RalentissementPion_VitesseParDefaut" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::SetAttitude      , "DEC_Population_ChangerAttitude"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::GetAttitude      , "DEC_Population_Attitude"                            );

    // Etats decisionnel
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::NotifyDominationStateChanged, "DEC_Population_ChangeEtatDomination" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::GetNbrAliveHumans           , "DEC_Population_Vivants"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::GetNbrDeadHumans            , "DEC_Population_Morts"                );
} 

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::InstanciatePopulation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_Population& MIL_PopulationType::InstanciatePopulation( uint nID, MIL_Army& army, xml::xistream& xis ) const
{
    return *new MIL_Population( *this, nID, army, xis );
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
    return damageData_[ roeFirer.GetID() ].rAttritionSurface_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetDamagePH
// Created: SBO 2006-01-25
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationType::GetDamagePH( const PHY_RoePopulation& roeFirer ) const
{
    assert( damageData_.size() > roeFirer.GetID() );
    return damageData_[ roeFirer.GetID() ].rPH_;
}
