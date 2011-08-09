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
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

DEC_Agent_PathClass::T_Rules DEC_Agent_PathClass::rules_;

struct DEC_Agent_PathClass::LoadingWrapper
{
    void ReadUnitRule( xml::xistream& xis )
    {
        DEC_Agent_PathClass::ReadUnitRule( xis );
    }
};

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
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Rule '" + strTypeName + "' is not defined for non flying, inhabited units"  );
        if( rules_.find( T_RuleType( strTypeName, T_BooleanPair( true, false ) ) ) == rules_.end() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Rule '" + strTypeName + "' is not defined for flying, inhabited units" );
        if( rules_.find( T_RuleType( strTypeName, T_BooleanPair( false, true ) ) ) == rules_.end() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Rule '" + strTypeName + "' is not defined for non flying, autonomous units" );
        if( rules_.find( T_RuleType( strTypeName, T_BooleanPair( true, true ) ) ) == rules_.end() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Rule '" + strTypeName + "' is not defined for flying, autonomous units" );
    }
}
// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::Initialize
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::Initialize( xml::xistream& xis )
{
    LoadingWrapper loader;
    xis >> xml::start( "unit-rules" )
            >> xml::list( "rule", loader, &LoadingWrapper::ReadUnitRule )
        >> xml::end;
    CheckRulesExistence();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadUnitRule
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadUnitRule( xml::xistream& xis )
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
            xis.error( "base rule '" + strBase + "' is undefined" );
    }
    DEC_Agent_PathClass*& pRule = rules_[ T_RuleType( strType, T_BooleanPair( bFlying, bAutonomous ) ) ];
    if( pRule )
        xis.error( "Rule '" + strType + "' already defined" );
    pRule = new DEC_Agent_PathClass( xis, pBase );
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
DEC_Agent_PathClass::DEC_Agent_PathClass( xml::xistream& xis, const DEC_Agent_PathClass* pCopyFrom /*= 0*/ )
    : bShort_                            ( false )
    , bAvoidObjects_                     ( true )
    , bFlying_                           ( xis.attribute< bool >( "flying" ) )
    , rPreferedTerrainCost_              ( 0 )
    , rAvoidedTerrainCost_               ( 0 )
    , objectCosts_                       ( )
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
    xis >> xml::optional
        >> xml::start( "optimisation" )
            >> xml::attribute( "shortest", bShort_ )
        >> xml::end

        >> xml::optional
        >> xml::start( "preferred-terrains" )
            >> xml::optional >> xml::attribute( "strength", rPreferedTerrainCost_ )
            >> xml::list( "preferred-terrain", *this, &DEC_Agent_PathClass::ReadPrefferedTerrains, preferedTerrain_ )
        >> xml::end

        >> xml::optional
        >> xml::start( "avoided-terrains" )
            >> xml::optional >> xml::attribute( "strength", rAvoidedTerrainCost_ )
            >> xml::list( "avoided-terrain", *this, &DEC_Agent_PathClass::ReadAvoidedTerrain, avoidedTerrain_ )
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
        const MIL_ObjectType_ABC& objectType = MIL_ObjectFactory::FindType( strType );
        unsigned int id = objectType.GetID();
        if( objectCosts_.size() <= id )
            objectCosts_.resize( id + 1, 0 );
        assert( objectCosts_.size() > id );
        xis >> xml::attribute( "value", objectCosts_[ id ] );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadTerrains
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadPrefferedTerrains( xml::xistream& xis, TerrainData& destinationData )
{
    destinationData = TerrainData();
    xis >> xml::list( "preferred-terrain", *this, &DEC_Agent_PathClass::ReadTerrain, destinationData );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadAvoidedTerrain
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadAvoidedTerrain( xml::xistream& xis, TerrainData& destinationData )
{
    destinationData = TerrainData();
    xis >> xml::list( "avoided-terrain", *this, &DEC_Agent_PathClass::ReadTerrain, destinationData );
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
        xis.error( "Unknown terrain type '" + strType + "'" );
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
    std::string strAttitude;
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
        populationAttitudeCosts_[ pAttitude ] = rCost;
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
double DEC_Agent_PathClass::GetPopulationAttitudeCost( const MIL_PopulationAttitude& attitude ) const
{
    T_PopulationAttitudeCosts::const_iterator it = populationAttitudeCosts_.find( &attitude );
    if( it == populationAttitudeCosts_.end() )
        return 0.0f;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetCostOutsideOfPopulation
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
double DEC_Agent_PathClass::GetCostOutsideOfPopulation() const
{
    return rCostOutsideOfPopulation_;
}
