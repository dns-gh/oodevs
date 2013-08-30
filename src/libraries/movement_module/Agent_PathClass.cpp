// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Agent_PathClass.h"
#include "PathType.h"
#include "wrapper/View.h"
#include "wrapper/Hook.h"
#include <tools/Exception.h>
#include <module_api/Log.h>
#include <xeumeuleu/xml.hpp>

using namespace sword::movement;

DECLARE_HOOK( FindObjectType, unsigned int, ( const char* type ) )
DECLARE_HOOK( FindPopulationAttitude, bool, ( const char* attitude, unsigned int& id ) )

Agent_PathClass::T_Rules Agent_PathClass::rules_;

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::CheckRulesExistence
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
void Agent_PathClass::CheckRulesExistence()
{
    const PathType::T_PathTypeMap& types = PathType::GetPathTypes();
    for( auto it = types.begin(); it != types.end(); ++it )
    {
        const std::string& strTypeName = it->second->GetName();
        if( rules_.find( T_RuleType( strTypeName, T_BooleanPair( false, false ) ) ) == rules_.end() )
            throw MASA_EXCEPTION( "Rule '" + strTypeName + "' is not defined for non flying, inhabited units"  );
        if( rules_.find( T_RuleType( strTypeName, T_BooleanPair( true, false ) ) ) == rules_.end() )
            throw MASA_EXCEPTION( "Rule '" + strTypeName + "' is not defined for flying, inhabited units" );
        if( rules_.find( T_RuleType( strTypeName, T_BooleanPair( false, true ) ) ) == rules_.end() )
            throw MASA_EXCEPTION( "Rule '" + strTypeName + "' is not defined for non flying, autonomous units" );
        if( rules_.find( T_RuleType( strTypeName, T_BooleanPair( true, true ) ) ) == rules_.end() )
            throw MASA_EXCEPTION( "Rule '" + strTypeName + "' is not defined for flying, autonomous units" );
    }
}
// -----------------------------------------------------------------------------
// Name: Agent_PathClass::Initialize
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
void Agent_PathClass::Initialize( xml::xistream& xis, const std::vector< unsigned int >& dangerousObjects )
{
    xis >> xml::start( "unit-rules" )
            >> xml::list( "rule", boost::bind( &Agent_PathClass::ReadUnitRule, _1, boost::cref( dangerousObjects ) ) )
        >> xml::end;
    CheckRulesExistence();
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::Terminate
// Created: SLI 2013-02-22
// -----------------------------------------------------------------------------
void Agent_PathClass::Terminate()
{
    for( auto it = rules_.begin(); it != rules_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::ReadUnitRule
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void Agent_PathClass::ReadUnitRule( xml::xistream& xis, const std::vector< unsigned int >& dangerousObjects )
{
    std::string strType;
    bool bFlying;
    bool bAutonomous;
    xis >> xml::attribute( "type" , strType )
        >> xml::attribute( "flying" , bFlying )
        >> xml::attribute( "autonomous", bAutonomous );
    std::string strBase;
    const Agent_PathClass* pBase = 0;
    strBase = "nothing";
    xis >> xml::optional >> xml::attribute( "inherits", strBase );
    if( strBase != "nothing" )
    {
        pBase  = rules_[ T_RuleType( strBase, T_BooleanPair( bFlying, bAutonomous ) ) ];
        if( !pBase )
            xis.error( "base rule '" + strBase + "' is undefined" );
    }
    Agent_PathClass*& pRule = rules_[ T_RuleType( strType, T_BooleanPair( bFlying, bAutonomous ) ) ];
    if( pRule )
        xis.error( "Rule '" + strType + "' already defined" );
    pRule = new Agent_PathClass( xis, dangerousObjects, pBase );
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass constructor
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
Agent_PathClass::Agent_PathClass( xml::xistream& xis, const std::vector< unsigned int >& dangerousObjects, const Agent_PathClass* pCopyFrom /*= 0*/ )
    : bShort_                            ( false )
    , bAvoidObjects_                     ( true )
    , rObstructionThreshold_             ( 10000 )
    , bFlying_                           ( xis.attribute< bool >( "flying" ) )
    , rPreferedTerrainCost_              ( 0 )
    , rAvoidedTerrainCost_               ( 0 )
    , rAltitudePreference_               ( 0 )
    , rMaximumFuseauDistance_            ( 1000 )
    , rMaximumFuseauDistanceWithAutomata_( 10000 )
    , rComfortFuseauDistance_            ( 500 )
    , rFuseauCostPerMeterOut_            ( 1 )
    , rFuseauCostPerMeterIn_             ( 0.002 )
    , rMaximumAutomataFuseauDistance_    ( 10 )
    , rAutomataFuseauCostPerMeterOut_    ( 1 )
    , rDangerDirectionBaseCost_          ( 1 )
    , rDangerDirectionLinearCost_        ( 0.001 )
    , rEnemyCostOnContact_               ( 0 )
    , rEnemyCostAtSecurityRange_         ( 0 )
    , rEnemyMaximumCost_                 ( 100 )
    , rPopulationSecurityRange_          ( 50 )
    , rPopulationMaximumCost_            ( 100 )
    , rCostOutsideOfPopulation_          ( 10 )
{
    if( pCopyFrom )
        *this = *pCopyFrom;
    else
        for( std::vector< unsigned int >::const_iterator it = dangerousObjects.begin(); it != dangerousObjects.end(); ++it )
        {
            unsigned int id = *it;
            if( objectCosts_.size() <= id )
                objectCosts_.resize( id + 1, 0 );
            assert( objectCosts_.size() > id );
            objectCosts_[ id ] = rObstructionThreshold_;
        }

    xis >> xml::optional
        >> xml::start( "optimisation" )
            >> xml::attribute( "shortest", bShort_ )
        >> xml::end

        >> xml::optional
        >> xml::start( "preferred-terrains" )
            >> xml::optional >> xml::attribute( "strength", rPreferedTerrainCost_ )
            >> xml::list( "preferred-terrain", *this, &Agent_PathClass::ReadPrefferedTerrains, preferedTerrain_ )
        >> xml::end

        >> xml::optional
        >> xml::start( "avoided-terrains" )
            >> xml::optional >> xml::attribute( "strength", rAvoidedTerrainCost_ )
            >> xml::list( "avoided-terrain", *this, &Agent_PathClass::ReadAvoidedTerrain, avoidedTerrain_ )
        >> xml::end

        >> xml::optional
        >> xml::start( "preferred-altitude" )
            >> xml::attribute( "value", rAltitudePreference_ )
        >> xml::end
        >> xml::list( "zone", *this, &Agent_PathClass::ReadFuseau )
        >> xml::list( "automat-zone", *this, &Agent_PathClass::ReadAutomataFuseau )
        >> xml::list( "danger-direction", *this, &Agent_PathClass::ReadDangerDirection )
        >> xml::list( "enemies", *this, &Agent_PathClass::ReadEnemiesCost )
        >> xml::list( "object-costs", *this, &Agent_PathClass::ReadObjectsCost )
        >> xml::list( "population-costs", *this, &Agent_PathClass::ReadPopulationsCost );
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass destructor
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
Agent_PathClass::~Agent_PathClass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::ReadObjectsCost
// Created: NLD 2007-02-08
// -----------------------------------------------------------------------------
void Agent_PathClass::ReadObjectsCost( xml::xistream& xis )
{
    xis >> xml::attribute( "avoid", bAvoidObjects_ )
        >> xml::attribute( "threshold", rObstructionThreshold_ )
        >> xml::list( "object-cost", *this, &Agent_PathClass::ReadObject );
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::xistream& xis );void ReadObject
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void Agent_PathClass::ReadObject( xml::xistream& xis )
{
    std::string strType( xis.attribute< std::string >( "type", std::string() ) );
    try
    {
        unsigned int id = GET_HOOK( FindObjectType )( strType.c_str() );
        if( objectCosts_.size() <= id )
            objectCosts_.resize( id + 1, 0 );
        assert( objectCosts_.size() > id );
        xis >> xml::attribute( "value", objectCosts_[ id ] );
    }
    catch( const std::exception& e )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, tools::GetExceptionMsg( e ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::ReadTerrains
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void Agent_PathClass::ReadPrefferedTerrains( xml::xistream& xis, TerrainData& destinationData )
{
    destinationData = TerrainData();
    xis >> xml::list( "preferred-terrain", *this, &Agent_PathClass::ReadTerrain, destinationData );
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::ReadAvoidedTerrain
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void Agent_PathClass::ReadAvoidedTerrain( xml::xistream& xis, TerrainData& destinationData )
{
    destinationData = TerrainData();
    xis >> xml::list( "avoided-terrain", *this, &Agent_PathClass::ReadTerrain, destinationData );
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::ReadTerrain
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void Agent_PathClass::ReadTerrain( xml::xistream& xis, TerrainData& destinationData )
{
    std::string strType;
    xis >> xml::attribute( "type", strType );
    const TerrainData data = TerrainData( strType );
    if( data.Area() == 0xFF )
        xis.error( "Unknown terrain type '" + strType + "'" );
    destinationData.Merge( data );
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::ReadFuseau
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void Agent_PathClass::ReadFuseau( xml::xistream& xis )
{
    xis >> xml::optional
        >> xml::start( "outter-tolerance" )
            >> xml::optional >> xml::attribute( "cost-per-meter", rFuseauCostPerMeterOut_ )
            >> xml::optional >> xml::attribute( "without-automat", rMaximumFuseauDistance_ )
            >> xml::optional >> xml::attribute( "with-automat", rMaximumFuseauDistanceWithAutomata_ )
        >> xml::end
        >> xml::optional
        >> xml::start( "inner-comfort" )
            >> xml::optional >> xml::attribute( "cost-per-meter", rFuseauCostPerMeterIn_ )
            >> xml::attribute( "distance", rComfortFuseauDistance_ )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::ReadAutomataFuseau
// Created: AGE 2005-08-12
// -----------------------------------------------------------------------------
void Agent_PathClass::ReadAutomataFuseau( xml::xistream& xis )
{
    xis >> xml::optional
        >> xml::start( "outter-tolerance" )
            >> xml::optional >> xml::attribute( "cost-per-meter" ,rAutomataFuseauCostPerMeterOut_ )
            >> xml::attribute( "distance", rMaximumAutomataFuseauDistance_ )
        >> xml::end;
    rMaximumAutomataFuseauDistance_ = std::max( 10., rMaximumAutomataFuseauDistance_ );
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::ReadDangerDirection
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void Agent_PathClass::ReadDangerDirection( xml::xistream& xis )
{
    xis >> xml::optional >> xml::attribute( "base-cost-beyond", rDangerDirectionBaseCost_ )
        >> xml::optional >> xml::attribute( "cost-per-meter", rDangerDirectionLinearCost_ );
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::ReadEnemiesCost
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void Agent_PathClass::ReadEnemiesCost( xml::xistream& xis )
{
    xis >> xml::optional >> xml::attribute( "cost-on-contact", rEnemyCostOnContact_ )
        >> xml::optional >> xml::attribute( "cost-at-security-range", rEnemyCostAtSecurityRange_ )
        >> xml::optional >> xml::attribute( "maximum-cost", rEnemyMaximumCost_ );
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::ReadPopulationsCost
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
void Agent_PathClass::ReadPopulationsCost( xml::xistream& xis )
{
    std::string strAttitude;
    xis >> xml::optional >> xml::attribute( "security-range", rPopulationSecurityRange_ )
        >> xml::optional >> xml::attribute( "maximum", rPopulationMaximumCost_ )
        >> xml::optional >> xml::attribute( "outside-of-population", rCostOutsideOfPopulation_ )
        >> xml::list( "population-cost", *this, &Agent_PathClass::ReadPopulation );
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::ReadPopulation
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void Agent_PathClass::ReadPopulation( xml::xistream& xis )
{
    double rCost = 0.0f;
    std::string strAttitude;
    xis >> xml::attribute( "attitude", strAttitude );
    unsigned int id = 0;
    if( GET_HOOK( FindPopulationAttitude )( strAttitude.c_str(), id ) )
    {
        xis >> xml::attribute( "contact-cost", rCost );
        populationAttitudeCosts_[ id ] = rCost;
    }
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetPathClass
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
const Agent_PathClass& Agent_PathClass::GetPathClass( const PathType& pathType, const sword::wrapper::View& entity )
{
    const bool bCanFly     = entity[ "can-fly" ];
    const bool bAutonomous = entity[ "is-autonomous" ];
    const Agent_PathClass* pClass = rules_[ T_RuleType( pathType.GetName(), T_BooleanPair( bCanFly, bAutonomous ) ) ];
    assert( pClass );
    return *pClass;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetObjectCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double Agent_PathClass::GetObjectCost( unsigned int objectType ) const
{
   if( objectCosts_.size() <= objectType )
        return 0.;
    return objectCosts_[ objectType ];
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetPopulationAttitudeCost
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
double Agent_PathClass::GetPopulationAttitudeCost( unsigned int attitudeID ) const
{
    T_PopulationAttitudeCosts::const_iterator it = populationAttitudeCosts_.find( attitudeID );
    if( it == populationAttitudeCosts_.end() )
        return 0.0f;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetEnemyMaximumCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double Agent_PathClass::GetEnemyMaximumCost() const
{
    return rEnemyMaximumCost_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetEnemyCostOnContact
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double Agent_PathClass::GetEnemyCostOnContact() const
{
    return rEnemyCostOnContact_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetEnemyCostAtSecurityRange
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double Agent_PathClass::GetEnemyCostAtSecurityRange() const
{
    return rEnemyCostAtSecurityRange_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetPreferedTerrainCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double Agent_PathClass::GetPreferedTerrainCost() const
{
    return rPreferedTerrainCost_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetPreferedTerrain
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
const TerrainData& Agent_PathClass::GetPreferedTerrain() const
{
    return preferedTerrain_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetAvoidedTerrainCost_
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double Agent_PathClass::GetAvoidedTerrainCost() const
{
    return rAvoidedTerrainCost_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetAvoidedTerrain_
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
const TerrainData& Agent_PathClass::GetAvoidedTerrain() const
{
    return avoidedTerrain_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetAltitudePreference
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double Agent_PathClass::GetAltitudePreference() const
{
    return rAltitudePreference_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetComfortFuseauDistance
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double Agent_PathClass::GetComfortFuseauDistance() const
{
    return rComfortFuseauDistance_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetFuseauCostPerMeterOut
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double Agent_PathClass::GetFuseauCostPerMeterOut() const
{
    return rFuseauCostPerMeterOut_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetFuseauCostPerMeterIn
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double Agent_PathClass::GetFuseauCostPerMeterIn() const
{
    return rFuseauCostPerMeterIn_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetMaximumFuseauDistance
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double Agent_PathClass::GetMaximumFuseauDistance() const
{
    return rMaximumFuseauDistance_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetMaximumFuseauDistanceWithAutomata
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double Agent_PathClass::GetMaximumFuseauDistanceWithAutomata() const
{
    return rMaximumFuseauDistanceWithAutomata_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetAutomateFuseauCostPerMeterOut
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double Agent_PathClass::GetAutomateFuseauCostPerMeterOut() const
{
    return rAutomataFuseauCostPerMeterOut_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetMaximumAutomataFuseauDistance
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double Agent_PathClass::GetMaximumAutomataFuseauDistance() const
{
    return rMaximumAutomataFuseauDistance_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetDangerDirectionBaseCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double Agent_PathClass::GetDangerDirectionBaseCost() const
{
    return rDangerDirectionBaseCost_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetDangerDirectionLinearCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double Agent_PathClass::GetDangerDirectionLinearCost() const
{
    return rDangerDirectionLinearCost_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::AvoidObjects
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
bool Agent_PathClass::AvoidObjects() const
{
    return bAvoidObjects_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::AvoidEnemies
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
bool Agent_PathClass::AvoidEnemies() const
{
    return rEnemyCostOnContact_ || rEnemyCostAtSecurityRange_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::HandlePopulations
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
bool Agent_PathClass::HandlePopulations() const
{
    return !populationAttitudeCosts_.empty();
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::IsShort
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
bool Agent_PathClass::IsShort() const
{
    return bShort_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::IsFlying
// Created: LDC 2011-08-09
// -----------------------------------------------------------------------------
bool Agent_PathClass::IsFlying() const
{
    return bFlying_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetPopulationSecurityRange
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
double Agent_PathClass::GetPopulationSecurityRange() const
{
    return rPopulationSecurityRange_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetPopulationMaximumCost
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
double Agent_PathClass::GetPopulationMaximumCost() const
{
    return rPopulationMaximumCost_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetThreshold
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
double Agent_PathClass::GetThreshold() const
{
    return rObstructionThreshold_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetCostOutsideOfPopulation
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
double Agent_PathClass::GetCostOutsideOfPopulation() const
{
    return rCostOutsideOfPopulation_;
}
