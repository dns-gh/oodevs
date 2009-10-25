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
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Tools/MIL_Tools.h"
#include "MIL_AgentServer.h"
#include <xeumeuleu/xml.h>

MIL_PopulationType::T_PopulationMap MIL_PopulationType::populations_;
MT_Float                            MIL_PopulationType::rEffectReloadingTimeDensity_ = 0.;
MT_Float                            MIL_PopulationType::rEffectReloadingTimeFactor_  = 0.;

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

    xis >> xml::start( "populations" )
            >> xml::start( "reloading-time-effect" )
                >> xml::attribute( "population-density", rEffectReloadingTimeDensity_ )
                >> xml::attribute( "modifier", rEffectReloadingTimeFactor_ )
            >> xml::end();

    if( rEffectReloadingTimeDensity_ < 0 )
        xis.error( "reloading-time-effet: population-density < 0" );
    if( rEffectReloadingTimeFactor_ < 1 )
        xis.error( "reloading-time-effect: modifier < 1" );

    LoadingWrapper loader;

    xis     >> xml::list( "population", loader, &LoadingWrapper::ReadPopulation )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::ReadPopulation
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void MIL_PopulationType::ReadPopulation( xml::xistream& xis )
{
    std::string strName;
    xis >> xml::attribute( "name", strName );

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
{
    xis >> xml::attribute( "id", nID_ )
        >> xml::attribute( "concentration-density", rConcentrationDensity_ )
        >> xml::attribute( "moving-base-density", rDefaultFlowDensity_ )
        >> xml::attribute( "moving-speed", rMaxSpeed_ );

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
    xis >> xml::attribute( "decisional-model", strModel );
    pModel_ = MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().FindModelPopulation( strModel );
    if( !pModel_ )
        xis.error( "Unknown population model" );

    InitializeDiaFunctions();
}


// -----------------------------------------------------------------------------
// Name: MIL_PopulationType constructor
// Created: LDC 2009-04-24
// -----------------------------------------------------------------------------
MIL_PopulationType::MIL_PopulationType( const DEC_Model_ABC& model )
    : pModel_               ( &model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_PopulationType::~MIL_PopulationType()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::InitializeSlowDownData
// Created: NLD 2005-10-20
// -----------------------------------------------------------------------------
void MIL_PopulationType::InitializeSlowDownData( xml::xistream& xis )
{
    xis >> xml::start( "slowing-effects" )
            >> xml::list( "slowing-effect", *this, &MIL_PopulationType::ReadSlowingEffect )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::ReadSlowingEffect
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void MIL_PopulationType::ReadSlowingEffect( xml::xistream& xis )
{
    std::string strAttitude;
    xis >> xml::attribute( "population-attitude", strAttitude );

    const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( strAttitude );
    if( !pAttitude )
        xis.error( "Unknown attitude '" + strAttitude + "'" );

    assert( slowDownData_.size() > pAttitude->GetID() );
    T_VolumeSlowDownData& volumeSlowDownData = slowDownData_[ pAttitude->GetID() ];

    xis >> xml::list( "unit", *this, &MIL_PopulationType::ReadSlowingUnitEffect, volumeSlowDownData );
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

    xis >> xml::attribute( "unit-size", strVolume )
        >> xml::attribute( "population-density", rPopulationDensity )
        >> xml::attribute( "max-speed", rMaxSpeed );

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
    xis >> xml::start( "attrition-effects" );
    attritionData_.Initialize( xis );
    xis >> xml::end();

    xis >> xml::start( "unit-fire-effects" )
            >> xml::list( "unit", *this, &MIL_PopulationType::ReadUnitFireEffect )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::ReadUnitFireEffect
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void MIL_PopulationType::ReadUnitFireEffect( xml::xistream& xis )
{
    std::string strRoe;
    xis >> xml::attribute( "rule-of-engagment", strRoe );
    const PHY_RoePopulation* pRoe = PHY_RoePopulation::Find( strRoe );
    if( !pRoe )
        xis.error( "Unknown population roe '" + strRoe + "'" );

    assert( damageData_.size() > pRoe->GetID() );

    xis >> xml::attribute( "attrition-surface", damageData_[ pRoe->GetID() ].rAttritionSurface_ )
        >> xml::attribute( "ph", damageData_[ pRoe->GetID() ].rPH_ );

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
} 

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::InstanciatePopulation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_Population& MIL_PopulationType::InstanciatePopulation( xml::xistream& xis, MIL_Army& army ) const
{
    return *new MIL_Population( xis, *this, army );
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

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetModel
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
const DEC_Model_ABC& MIL_PopulationType::GetModel() const
{
    assert( pModel_ );
    return *pModel_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetConcentrationDensity
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationType::GetConcentrationDensity() const
{
    return rConcentrationDensity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetDefaultFlowDensity
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationType::GetDefaultFlowDensity() const
{
    return rDefaultFlowDensity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const std::string& MIL_PopulationType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetMosID
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
uint MIL_PopulationType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetMaxSpeed
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationType::GetMaxSpeed() const
{
    return rMaxSpeed_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetAttritionData
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
const PHY_AttritionData& MIL_PopulationType::GetAttritionData( const MIL_PopulationAttitude& attitude, const PHY_Protection& protection ) const
{
    return attritionData_.GetAttritionData( attitude, protection );    
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetPH
// Created: NLD 2005-11-04
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationType::GetPH( const MIL_PopulationAttitude& attitude, MT_Float rDensity ) const
{
    return attritionData_.GetPH( attitude, rDensity );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::Find
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
const MIL_PopulationType* MIL_PopulationType::Find( const std::string& strName )
{
    CIT_PopulationMap it = populations_.find( strName );   
    return it == populations_.end() ? 0 :  it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::Find
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
const MIL_PopulationType* MIL_PopulationType::Find( uint nID )
{
    for( CIT_PopulationMap it = populations_.begin(); it != populations_.end(); ++it )
        if( it->second->nID_ == nID )
            return it->second;
    return 0;
}
