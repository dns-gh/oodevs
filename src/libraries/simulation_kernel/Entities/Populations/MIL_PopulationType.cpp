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
#include "Decision/DEC_Workspace.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/PHY_Speeds.h"
#include "MT_Tools/MT_Logger.h"
#include "MIL_AgentServer.h"
#include "Tools/MIL_Tools.h"
#include "tools/Codec.h"

MIL_PopulationType::T_PopulationMap MIL_PopulationType::populations_;
double MIL_PopulationType::rEffectReloadingTimeDensity_ = 0.;
double MIL_PopulationType::rEffectReloadingTimeFactor_  = 0.;
double MIL_PopulationType::delay_                       = 0u;

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_PopulationType::Initialize( xml::xistream& xis )
{
    std::string delay = "1h";
    MT_LOG_INFO_MSG( "Initializing population types" );
    xis >> xml::start( "populations" )
            >> xml::start( "reloading-time-effect" )
                >> xml::attribute( "population-density", rEffectReloadingTimeDensity_ )
                >> xml::attribute( "modifier", rEffectReloadingTimeFactor_ )
            >> xml::end
            >> xml::optional
                >> xml::start( "time-between-nbc-applications" )
                    >> xml::attribute( "delay", delay )
                >> xml::end;
    tools::DecodeTime( delay, delay_ );
    delay_ = MIL_Tools::ConvertSecondsToSim( delay_ );

    if( rEffectReloadingTimeDensity_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "reloading-time-effet: population-density < 0" );
    if( rEffectReloadingTimeFactor_ < 1 )
        throw MASA_EXCEPTION( xis.context() + "reloading-time-effect: modifier < 1" );

    xis >> xml::list( "population", &MIL_PopulationType::ReadPopulation )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::ReadPopulation
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void MIL_PopulationType::ReadPopulation( xml::xistream& xis )
{
    std::string strName = xis.attribute< std::string >( "name" );
    const MIL_PopulationType*& pPopulation = populations_[ strName ];
    if( pPopulation )
        throw MASA_EXCEPTION( xis.context() + "Population type already exists" );
    pPopulation = new MIL_PopulationType( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_PopulationType::Terminate()
{
    for( auto it = populations_.begin(); it != populations_.end(); ++it )
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
    , rArmedIndividuals_    ( 0. )
    , rMale_                ( 1. )
    , rFemale_              ( 0. )
    , rChildren_            ( 0. )
    , pModel_               ( 0 )
    , slowDownData_         ( MIL_PopulationAttitude::GetAttitudes().size(), T_VolumeSlowDownData( PHY_Volume::GetVolumes().size(), sSlowDownData( 0., 0. ) ) )
    , attritionData_        ()
    , damageData_           ( PHY_RoePopulation::GetRoePopulations().size(), sDamageData( 0., 0. ) )
    , urbanDestructionData_ ( MIL_PopulationAttitude::GetAttitudes().size(), sUrbanDestructionData( 0.0, 0.0 ) )
    , canCollideWithFlow_   ( false )
{
    double rMaxSpeed = 0;
    xis >> xml::attribute( "id", nID_ )
        >> xml::attribute( "concentration-density", rConcentrationDensity_ )
        >> xml::attribute( "moving-base-density", rDefaultFlowDensity_ )
        >> xml::attribute( "moving-speed", rMaxSpeed )
        >> xml::optional >> xml::attribute( "armed-individuals", rArmedIndividuals_ )
        >> xml::optional >> xml::attribute( "collides-with-crowds", canCollideWithFlow_ );

    if( rConcentrationDensity_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "population: concentration-density <= 0" );
    if( rDefaultFlowDensity_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "population: moving-base-density <= 0" );
    if( rMaxSpeed <= 0 )
        throw MASA_EXCEPTION( xis.context() + "population: moving-speed" );

    rMaxSpeed = MIL_Tools::ConvertSpeedMosToSim( rMaxSpeed );

    speeds_.reset( new PHY_Speeds( xis, rMaxSpeed ) );

    xis >> xml::start( "repartition" )
        >> xml::attribute( "male", rMale_ )
        >> xml::attribute( "female", rFemale_ )
        >> xml::attribute( "children", rChildren_ )
        >> xml::end;

    std::string decontaminationDelay = "300s";
    xis >> xml::start( "nbc" )
         >> xml::attribute( "decontaminationDelay", decontaminationDelay )
        >> xml::end;
    tools::DecodeTime( decontaminationDelay, decontaminationDelay_ );
    decontaminationDelay_ = MIL_Tools::ConvertSecondsToSim( decontaminationDelay_ );

    InitializeSlowDownData( xis );
    InitializeFireData    ( xis );

    pModel_ = MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().FindModelPopulation( xis.attribute< std::string >( "decisional-model" ) );
    if( !pModel_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown population model" );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType constructor
// Created: LDC 2009-04-24
// -----------------------------------------------------------------------------
MIL_PopulationType::MIL_PopulationType( const DEC_Model_ABC& model, double rConcentrationDensity )
    : nID_( 0 )
    , rConcentrationDensity_( rConcentrationDensity )
    , rDefaultFlowDensity_( 0. )
    , rArmedIndividuals_( 0. )
    , rMale_( 1. )
    , rFemale_( 0. )
    , rChildren_( 0. )
    , pModel_( &model )
    , decontaminationDelay_( 0. )
    , canCollideWithFlow_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_PopulationType::~MIL_PopulationType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::InitializeSlowDownData
// Created: NLD 2005-10-20
// -----------------------------------------------------------------------------
void MIL_PopulationType::InitializeSlowDownData( xml::xistream& xis )
{
    xis >> xml::start( "slowing-effects" )
            >> xml::list( "slowing-effect", *this, &MIL_PopulationType::ReadSlowingEffect )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::ReadSlowingEffect
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void MIL_PopulationType::ReadSlowingEffect( xml::xistream& xis )
{
    std::string strAttitude = xis.attribute< std::string >( "population-attitude" );
    const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( strAttitude );
    if( !pAttitude )
        throw MASA_EXCEPTION( xis.context() + "Unknown attitude '" + strAttitude + "'" );
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
    double rPopulationDensity = 0.;
    double rMaxSpeed          = 0.;

    xis >> xml::attribute( "unit-size", strVolume )
        >> xml::attribute( "population-density", rPopulationDensity )
        >> xml::attribute( "max-speed", rMaxSpeed );

    if( rPopulationDensity <= 0 )
        throw MASA_EXCEPTION( xis.context() + "unit: population-density <= 0" );
    if( rMaxSpeed < 0 )
        throw MASA_EXCEPTION( xis.context() + "unit: max-speed < 0" );

    rMaxSpeed = MIL_Tools::ConvertSpeedMosToSim( rMaxSpeed );

    const PHY_Volume* pVolume = PHY_Volume::FindVolume( strVolume );
    if( !pVolume )
        throw MASA_EXCEPTION( xis.context() + "Unknown volume '" + strVolume + "'" );

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
    xis >> xml::end;

    xis >> xml::start( "unit-fire-effects" )
            >> xml::list( "unit", *this, &MIL_PopulationType::ReadUnitFireEffect )
        >> xml::end;

    xis >> xml::start( "urban-destruction-effects" )
            >> xml::list( "urban-destruction-effect", *this, &MIL_PopulationType::ReadUrbanDestructionEffect )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::ReadUnitFireEffect
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void MIL_PopulationType::ReadUnitFireEffect( xml::xistream& xis )
{
    std::string strRoe = xis.attribute< std::string >( "rule-of-engagment" );
    const PHY_RoePopulation* pRoe = PHY_RoePopulation::Find( strRoe );
    if( !pRoe )
        throw MASA_EXCEPTION( xis.context() + "Unknown population roe '" + strRoe + "'" );

    assert( damageData_.size() > pRoe->GetID() );

    xis >> xml::attribute( "attrition-surface", damageData_[ pRoe->GetID() ].rAttritionSurface_ )
        >> xml::attribute( "ph", damageData_[ pRoe->GetID() ].rPH_ );

    if( damageData_[ pRoe->GetID() ].rAttritionSurface_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "unit-fire-effect: rule-of-engagment < 0" );
    if( damageData_[ pRoe->GetID() ].rPH_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "unit-fire-effect: ph < 0" );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::ReadUrbanDestructionEffect
// Created: MMC 2011-03-31
// -----------------------------------------------------------------------------
void MIL_PopulationType::ReadUrbanDestructionEffect( xml::xistream& xis )
{
    std::string strAttitude = xis.attribute< std::string >( "attitude" );
    const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( strAttitude );
    if( !pAttitude )
        throw MASA_EXCEPTION( xis.context() + "Unknown attitude '" + strAttitude + "'" );
    assert( urbanDestructionData_.size() > pAttitude->GetID() );

    urbanDestructionData_[ pAttitude->GetID() ].rDensity_ = xis.attribute< double >( "density" );
    if( urbanDestructionData_[ pAttitude->GetID() ].rDensity_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "urban-destruction-effect: density < 0" );

    double rTime;
    tools::ReadTimeAttribute( xis, "time", rTime );
    if( rTime < 0 )
        throw MASA_EXCEPTION( xis.context() + "urban-destruction-effect: time < 0" );
    urbanDestructionData_[ pAttitude->GetID() ].rTime_ = MIL_Tools::ConvertSecondsToSim( rTime );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetPionMaxSpeed
// Created: NLD 2005-10-20
// -----------------------------------------------------------------------------
double MIL_PopulationType::GetPionMaxSpeed( const MIL_PopulationAttitude& populationAttitude, double rPopulationDensity, const PHY_Volume& pionVolume ) const
{
    assert( slowDownData_.size() > populationAttitude.GetID() );
    const T_VolumeSlowDownData& volumeSlowDownData = slowDownData_[ populationAttitude.GetID() ];

    assert( volumeSlowDownData.size() > pionVolume.GetID() );
    const sSlowDownData& slowDownData = volumeSlowDownData[ pionVolume.GetID() ];

    if( rPopulationDensity == 0. || slowDownData.rPopulationDensity_ == 0. )
        return std::numeric_limits< double >::max();

    return ( slowDownData.rMaxSpeed_ * slowDownData.rPopulationDensity_ ) / rPopulationDensity;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetPionReloadingTimeFactor
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
double MIL_PopulationType::GetPionReloadingTimeFactor( double rPopulationDensity ) const
{
    if( rEffectReloadingTimeDensity_ == 0. )
        return 1.;

    return std::max( 1., rPopulationDensity * rEffectReloadingTimeFactor_ / rEffectReloadingTimeDensity_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetDamageSurface
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
double MIL_PopulationType::GetDamageSurface( const PHY_RoePopulation& roeFirer ) const
{
    assert( damageData_.size() > roeFirer.GetID() );
    return damageData_[ roeFirer.GetID() ].rAttritionSurface_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetDamagePH
// Created: SBO 2006-01-25
// -----------------------------------------------------------------------------
double MIL_PopulationType::GetDamagePH( const PHY_RoePopulation& roeFirer ) const
{
    assert( damageData_.size() > roeFirer.GetID() );
    return damageData_[ roeFirer.GetID() ].rPH_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetUrbanDestructionDensity
// Created: MMC 2011-03-31
// -----------------------------------------------------------------------------
double MIL_PopulationType::GetUrbanDestructionDensity( const MIL_PopulationAttitude& attitude ) const
{
    assert( urbanDestructionData_.size() > attitude.GetID() );
    return urbanDestructionData_[ attitude.GetID() ].rDensity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetUrbanDestructionTime
// Created: MMC 2011-03-31
// -----------------------------------------------------------------------------
double MIL_PopulationType::GetUrbanDestructionTime( const MIL_PopulationAttitude& attitude ) const
{
    assert( urbanDestructionData_.size() > attitude.GetID() );
    return urbanDestructionData_[ attitude.GetID() ].rTime_;
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
double MIL_PopulationType::GetConcentrationDensity() const
{
    return rConcentrationDensity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetDefaultFlowDensity
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
double MIL_PopulationType::GetDefaultFlowDensity() const
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
unsigned int MIL_PopulationType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetArmedIndividuals
// Created: JSR 2011-03-07
// -----------------------------------------------------------------------------
double MIL_PopulationType::GetArmedIndividuals() const
{
    return rArmedIndividuals_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetMale
// Created: JSR 2011-03-07
// -----------------------------------------------------------------------------
double MIL_PopulationType::GetMale() const
{
    return rMale_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetFemale
// Created: JSR 2011-03-07
// -----------------------------------------------------------------------------
double MIL_PopulationType::GetFemale() const
{
    return rFemale_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetChildren
// Created: JSR 2011-03-07
// -----------------------------------------------------------------------------
double MIL_PopulationType::GetChildren() const
{
    return rChildren_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetMaxSpeed
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
double MIL_PopulationType::GetMaxSpeed() const
{
    return speeds_? speeds_->GetMaxSpeed() : 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetMaxSpeed
// Created: JSR 2014-02-14
// -----------------------------------------------------------------------------
double MIL_PopulationType::GetMaxSpeed( const TerrainData& data ) const
{
    double speed = speeds_ ? speeds_->GetMaxSpeed( data ) : 0;
    return speed == -1. ? speeds_->GetMaxSpeed() : speed;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetAttritionData
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
const PHY_AttritionData MIL_PopulationType::GetAttritionData( const MIL_PopulationAttitude& attitude, const PHY_Protection& protection, double armedIndividuals ) const
{
    return attritionData_.GetAttritionData( attitude, protection, armedIndividuals );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetPH
// Created: NLD 2005-11-04
// -----------------------------------------------------------------------------
double MIL_PopulationType::GetPH( const MIL_PopulationAttitude& attitude, double rDensity ) const
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
const MIL_PopulationType* MIL_PopulationType::Find( unsigned int nID )
{
    for( auto it = populations_.begin(); it != populations_.end(); ++it )
        if( it->second->nID_ == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetDelay
// Created: LGY 2011-03-31
// -----------------------------------------------------------------------------
double MIL_PopulationType::GetDelay() const
{
    return delay_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetDecontaminationDelay
// Created: LGY 2011-11-10
// -----------------------------------------------------------------------------
double MIL_PopulationType::GetDecontaminationDelay() const
{
    return decontaminationDelay_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::CanCollideWithFlow
// Created: JSR 2014-01-09
// -----------------------------------------------------------------------------
bool MIL_PopulationType::CanCollideWithFlow() const
{
    return canCollideWithFlow_;
}
