// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Agent_PathClass.h"
#include "Decision/DEC_PathType.h"
#include "MIL_AgentServer.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "MT_Tools/MT_Logger.h"

DEC_Agent_PathClass::T_Rules DEC_Agent_PathClass::rules_;

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::CheckRulesExistence
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::CheckRulesExistence()
{
    const DEC_PathType::T_PathTypeMap& types = DEC_PathType::GetPathTypes();
    for( DEC_PathType::CIT_PathTypeMap it = types.begin(); it != types.end(); ++it )
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
// Name: DEC_Agent_PathClass::Initialize
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::Initialize( xml::xistream& xis, const std::vector< unsigned int >& dangerousObjects )
{
    xis >> xml::start( "unit-rules" )
            >> xml::list( "rule", boost::bind( DEC_Agent_PathClass::ReadUnitRule, _1, boost::cref( dangerousObjects ) ) )
        >> xml::end;
    CheckRulesExistence();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadUnitRule
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadUnitRule( xml::xistream& xis, const std::vector< unsigned int >& dangerousObjects )
{
    std::string strType;
    bool bFlying;
    bool bAutonomous;
    xis >> xml::attribute( "type" , strType )
        >> xml::attribute( "flying" , bFlying )
        >> xml::attribute( "autonomous", bAutonomous );
    std::string strBase;
    const DEC_Agent_PathClass* pBase = 0;
    strBase = "nothing";
    xis >> xml::optional >> xml::attribute( "inherits", strBase );
    if( strBase != "nothing" )
    {
        pBase  = rules_[ T_RuleType( strBase, T_BooleanPair( bFlying, bAutonomous ) ) ];
        if( !pBase )
            throw MASA_EXCEPTION( xis.context() + "base rule '" + strBase + "' is undefined" );
    }
    DEC_Agent_PathClass*& pRule = rules_[ T_RuleType( strType, T_BooleanPair( bFlying, bAutonomous ) ) ];
    if( pRule )
        throw MASA_EXCEPTION( xis.context() + "Rule '" + strType + "' already defined" );
    pRule = new DEC_Agent_PathClass( xis, dangerousObjects, pBase );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::Terminate
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::Terminate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass constructor
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
DEC_Agent_PathClass::DEC_Agent_PathClass( xml::xistream& xis, const std::vector< unsigned int >& dangerousObjects, const DEC_Agent_PathClass* pCopyFrom /*= 0*/ )
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
            >> xml::list( "preferred-terrain", *this, &DEC_Agent_PathClass::ReadTerrain, preferedTerrain_ )
        >> xml::end

        >> xml::optional
        >> xml::start( "avoided-terrains" )
            >> xml::optional >> xml::attribute( "strength", rAvoidedTerrainCost_ )
            >> xml::list( "avoided-terrain", *this, &DEC_Agent_PathClass::ReadTerrain, avoidedTerrain_ )
        >> xml::end

        >> xml::optional
        >> xml::start( "preferred-altitude" )
            >> xml::attribute( "value", rAltitudePreference_ )
        >> xml::end
        >> xml::list( "zone", *this, &DEC_Agent_PathClass::ReadFuseau )
        >> xml::list( "automat-zone", *this, &DEC_Agent_PathClass::ReadAutomataFuseau )
        >> xml::list( "danger-direction", *this, &DEC_Agent_PathClass::ReadDangerDirection )
        >> xml::list( "enemies", *this, &DEC_Agent_PathClass::ReadEnemiesCost )
        >> xml::list( "object-costs", *this, &DEC_Agent_PathClass::ReadObjectsCost )
        >> xml::list( "population-costs", *this, &DEC_Agent_PathClass::ReadPopulationsCost );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass destructor
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
DEC_Agent_PathClass::~DEC_Agent_PathClass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadObjectsCost
// Created: NLD 2007-02-08
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadObjectsCost( xml::xistream& xis )
{
    xis >> xml::attribute( "avoid", bAvoidObjects_ )
        >> xml::attribute( "threshold", rObstructionThreshold_ )
        >> xml::list( "object-cost", *this, &DEC_Agent_PathClass::ReadObject );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::xistream& xis );void ReadObject
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadObject( xml::xistream& xis )
{
    std::string strType( xis.attribute< std::string >( "type", std::string() ) );
    try
    {
        const MIL_ObjectType_ABC& objectType = MIL_AgentServer::GetWorkspace().GetObjectFactory().FindType( strType );
        unsigned int id = objectType.GetID();
        if( objectCosts_.size() <= id )
            objectCosts_.resize( id + 1, 0 );
        assert( objectCosts_.size() > id );
        xis >> xml::attribute( "value", objectCosts_[ id ] );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadTerrain
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadTerrain( xml::xistream& xis, TerrainData& destinationData )
{
    std::string strType;
    xis >> xml::attribute( "type", strType );
    const TerrainData data = TerrainData( strType );
    if( data.Area() == 0xFF )
    {
        MT_LOG_ERROR_MSG( xis.context() + "Unknown terrain type '" + strType + "'" );
        return;
    }
    destinationData.Merge( data );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadFuseau
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadFuseau( xml::xistream& xis )
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
// Name: DEC_Agent_PathClass::ReadAutomataFuseau
// Created: AGE 2005-08-12
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadAutomataFuseau( xml::xistream& xis )
{
    xis >> xml::optional
        >> xml::start( "outter-tolerance" )
            >> xml::optional >> xml::attribute( "cost-per-meter" ,rAutomataFuseauCostPerMeterOut_ )
            >> xml::attribute( "distance", rMaximumAutomataFuseauDistance_ )
        >> xml::end;
    rMaximumAutomataFuseauDistance_ = std::max( 10., rMaximumAutomataFuseauDistance_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadDangerDirection
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadDangerDirection( xml::xistream& xis )
{
    xis >> xml::optional >> xml::attribute( "base-cost-beyond", rDangerDirectionBaseCost_ )
        >> xml::optional >> xml::attribute( "cost-per-meter", rDangerDirectionLinearCost_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadEnemiesCost
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadEnemiesCost( xml::xistream& xis )
{
    xis >> xml::optional >> xml::attribute( "cost-on-contact", rEnemyCostOnContact_ )
        >> xml::optional >> xml::attribute( "cost-at-security-range", rEnemyCostAtSecurityRange_ )
        >> xml::optional >> xml::attribute( "maximum-cost", rEnemyMaximumCost_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadPopulationsCost
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadPopulationsCost( xml::xistream& xis )
{
    xis >> xml::optional >> xml::attribute( "security-range", rPopulationSecurityRange_ )
        >> xml::optional >> xml::attribute( "maximum", rPopulationMaximumCost_ )
        >> xml::optional >> xml::attribute( "outside-of-population", rCostOutsideOfPopulation_ )
        >> xml::list( "population-cost", *this, &DEC_Agent_PathClass::ReadPopulation );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadPopulation
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadPopulation( xml::xistream& xis )
{
    double rCost = 0.0f;
    std::string strAttitude;
    xis >> xml::attribute( "attitude", strAttitude );
    const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( strAttitude );
    if( pAttitude )
    {
        xis >> xml::attribute( "contact-cost", rCost );
        populationAttitudeCosts_[ pAttitude->GetID() ] = rCost;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetPathClass
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
const DEC_Agent_PathClass& DEC_Agent_PathClass::GetPathClass( const DEC_PathType& pathType, const MIL_Agent_ABC& pion )
{
    const bool bCanFly     = pion.GetType().GetUnitType().CanFly();
    const bool bAutonomous = pion.IsAutonomous();
    const DEC_Agent_PathClass* pClass = rules_[ T_RuleType( pathType.GetName(), T_BooleanPair( bCanFly, bAutonomous ) ) ];
    assert( pClass );
    return *pClass;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetObjectCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetObjectCost( const MIL_ObjectType_ABC& objectType ) const
{
   if( objectCosts_.size() <= objectType.GetID() )
        return 0.;
    return objectCosts_[ objectType.GetID() ];
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetPopulationAttitudeCost
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetPopulationAttitudeCost( unsigned int attitudeID ) const
{
    T_PopulationAttitudeCosts::const_iterator it = populationAttitudeCosts_.find( attitudeID );
    if( it == populationAttitudeCosts_.end() )
        return 0.0f;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetEnemyMaximumCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetEnemyMaximumCost() const
{
    return rEnemyMaximumCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetEnemyCostOnContact
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetEnemyCostOnContact() const
{
    return rEnemyCostOnContact_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetEnemyCostAtSecurityRange
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetEnemyCostAtSecurityRange() const
{
    return rEnemyCostAtSecurityRange_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetPreferedTerrainCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetPreferedTerrainCost() const
{
    return rPreferedTerrainCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetPreferedTerrain
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
const TerrainData& DEC_Agent_PathClass::GetPreferedTerrain() const
{
    return preferedTerrain_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetAvoidedTerrainCost_
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetAvoidedTerrainCost() const
{
    return rAvoidedTerrainCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetAvoidedTerrain_
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
const TerrainData& DEC_Agent_PathClass::GetAvoidedTerrain() const
{
    return avoidedTerrain_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetAltitudePreference
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetAltitudePreference() const
{
    return rAltitudePreference_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetComfortFuseauDistance
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetComfortFuseauDistance() const
{
    return rComfortFuseauDistance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetFuseauCostPerMeterOut
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetFuseauCostPerMeterOut() const
{
    return rFuseauCostPerMeterOut_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetFuseauCostPerMeterIn
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetFuseauCostPerMeterIn() const
{
    return rFuseauCostPerMeterIn_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetMaximumFuseauDistance
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetMaximumFuseauDistance() const
{
    return rMaximumFuseauDistance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetMaximumFuseauDistanceWithAutomata
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetMaximumFuseauDistanceWithAutomata() const
{
    return rMaximumFuseauDistanceWithAutomata_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetAutomateFuseauCostPerMeterOut
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetAutomateFuseauCostPerMeterOut() const
{
    return rAutomataFuseauCostPerMeterOut_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetMaximumAutomataFuseauDistance
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetMaximumAutomataFuseauDistance() const
{
    return rMaximumAutomataFuseauDistance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetDangerDirectionBaseCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetDangerDirectionBaseCost() const
{
    return rDangerDirectionBaseCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetDangerDirectionLinearCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetDangerDirectionLinearCost() const
{
    return rDangerDirectionLinearCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::AvoidObjects
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
bool DEC_Agent_PathClass::AvoidObjects() const
{
    return bAvoidObjects_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::AvoidEnemies
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
bool DEC_Agent_PathClass::AvoidEnemies() const
{
    return rEnemyCostOnContact_ || rEnemyCostAtSecurityRange_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::HandlePopulations
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
bool DEC_Agent_PathClass::HandlePopulations() const
{
    return !populationAttitudeCosts_.empty();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::IsShort
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
bool DEC_Agent_PathClass::IsShort() const
{
    return bShort_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::IsFlying
// Created: LDC 2011-08-09
// -----------------------------------------------------------------------------
bool DEC_Agent_PathClass::IsFlying() const
{
    return bFlying_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetPopulationSecurityRange
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetPopulationSecurityRange() const
{
    return rPopulationSecurityRange_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetPopulationMaximumCost
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetPopulationMaximumCost() const
{
    return rPopulationMaximumCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetThreshold
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetThreshold() const
{
    return rObstructionThreshold_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetCostOutsideOfPopulation
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetCostOutsideOfPopulation() const
{
    return rCostOutsideOfPopulation_;
}