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

#include "Decision/Path/DEC_PathType.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "xeumeuleu/xml.h"

using namespace xml;

DEC_Agent_PathClass::T_Rules       DEC_Agent_PathClass::rules_;

struct DEC_Agent_PathClass::LoadingWrapper
{
    void ReadUnitRule( xml::xistream& xis )
    {
        DEC_Agent_PathClass::ReadUnitRule( xis );
    }
};

// =============================================================================
// MANAGER
// =============================================================================

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

    xis >> start( "unit-rules" )
            >> list( "rule", loader, &LoadingWrapper::ReadUnitRule )
        >> end();

    CheckRulesExistence();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadUnitRule
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadUnitRule( xml::xistream& xis )
{
    std::string strType;
    bool        bFlying;
    bool        bAutonomous;

    xis >> attribute( "type"      , strType     )
        >> attribute( "flying"    , bFlying     )
        >> attribute( "autonomous", bAutonomous );

    std::string strBase;
    const DEC_Agent_PathClass* pBase = 0;
    strBase = "nothing";
    xis >> optional() >> attribute( "inherits", strBase );
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

// =============================================================================
// INSTANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass constructor
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
DEC_Agent_PathClass::DEC_Agent_PathClass( xml::xistream& xis, const DEC_Agent_PathClass* pCopyFrom /*= 0*/ )
    : bShort_                            ( false )
    , rPreferedTerrainCost_              ( 0 )
    , rAvoidedTerrainCost_               ( 0 )
    , bAvoidObjects_                     ( true )
    , objectCosts_                       ( MIL_RealObjectType::GetObjectTypes().size(), 0 )
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

    xis >> optional() >> start( "optimisation" )
                        >> attribute( "shortest", bShort_ )
                    >> end()

        >> optional() >> start( "preferred-terrains" )
                          >> optional() >> attribute( "strength", rPreferedTerrainCost_ )
                          >> list( "preferred-terrain", *this, &DEC_Agent_PathClass::ReadPrefferedTerrains, preferedTerrain_ )
                      >> end()

        >> optional() >> start( "avoided-terrains" )
                          >> optional() >> attribute( "strength", rAvoidedTerrainCost_ )
                          >> list( "avoided-terrain", *this, &DEC_Agent_PathClass::ReadAvoidedTerrain, avoidedTerrain_ )
                      >> end()

        >> optional() >> start( "preferred-altitude" )
                          >> attribute( "value", rAltitudePreference_ )
                      >> end()

        >> list( "zone", *this, &DEC_Agent_PathClass::ReadFuseau )

        >> list( "automat-zone", *this, &DEC_Agent_PathClass::ReadAutomataFuseau )

        >> list( "danger-direction", *this, &DEC_Agent_PathClass::ReadDangerDirection )

        >> list( "enemies", *this, &DEC_Agent_PathClass::ReadEnemiesCost )

        >> list( "object-costs", *this, &DEC_Agent_PathClass::ReadObjectsCost )

        >> list( "population-costs", *this, &DEC_Agent_PathClass::ReadPopulationsCost );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass destructor
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
DEC_Agent_PathClass::~DEC_Agent_PathClass()
{
    //NOTHING
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadObjectsCost
// Created: NLD 2007-02-08
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadObjectsCost( xml::xistream& xis )
{
    xis >> attribute( "avoid", bAvoidObjects_ )
        >> list( "object-cost", *this, &DEC_Agent_PathClass::ReadObject );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::xistream& xis );void ReadObject
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadObject( xml::xistream& xis )
{
    std::string strObjectType;
    xis >> attribute( "type", strObjectType );
    const MIL_RealObjectType* pType = MIL_RealObjectType::Find( strObjectType );
    if( !pType )
        xis.error( "Unknown object type" );

    assert( objectCosts_.size() > pType->GetID() );
    xis >> attribute( "value", objectCosts_[ pType->GetID() ] );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadTerrains
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadPrefferedTerrains( xml::xistream& xis, TerrainData& destinationData )
{
    destinationData = TerrainData();
    xis >> list( "preferred-terrain", *this, &DEC_Agent_PathClass::ReadTerrain, destinationData );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadAvoidedTerrain
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadAvoidedTerrain( xml::xistream& xis, TerrainData& destinationData )
{
    destinationData = TerrainData();
    xis >> list( "avoided-terrain", *this, &DEC_Agent_PathClass::ReadTerrain, destinationData );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadTerrain
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadTerrain( xml::xistream& xis, TerrainData& destinationData )
{
    std::string strType;
    xis >> attribute( "type", strType );
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
    xis >> optional() >> start( "outter-tolerance" )
                          >> optional() >> attribute( "cost-per-meter", rFuseauCostPerMeterOut_ )
                          >> optional() >> attribute( "without-automat", rMaximumFuseauDistance_ )
                          >> optional() >> attribute( "with-automat", rMaximumFuseauDistanceWithAutomata_ )
                      >> end()

        >>optional() >> start( "inner-comfort" )
                          >> optional() >> attribute( "cost-per-meter", rFuseauCostPerMeterIn_ )
                          >> attribute( "distance", rComfortFuseauDistance_ )
                     >> end();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadAutomataFuseau
// Created: AGE 2005-08-12
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadAutomataFuseau( xml::xistream& xis )
{
    xis >> optional() >> start( "outter-tolerance " )
                          >> optional() >> attribute( "cost-per-meter" ,rAutomataFuseauCostPerMeterOut_ )
                          >> attribute( "distance", rMaximumAutomataFuseauDistance_ )
                      >> end();
    if( rMaximumAutomataFuseauDistance_ < 10 )
        rMaximumAutomataFuseauDistance_ = 10;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadDangerDirection
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadDangerDirection( xml::xistream& xis )
{
    xis >> optional() >> attribute( "base-cost-beyond", rDangerDirectionBaseCost_ )
        >> optional() >> attribute( "cost-per-meter", rDangerDirectionLinearCost_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadEnemiesCost
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadEnemiesCost( xml::xistream& xis )
{
    xis >> optional() >> attribute( "cost-on-contact", rEnemyCostOnContact_ )
        >> optional() >> attribute( "cost-at-security-range", rEnemyCostAtSecurityRange_ )
        >> optional() >> attribute( "maximum-cost", rEnemyMaximumCost_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadPopulationsCost
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadPopulationsCost( xml::xistream& xis )
{
    std::string strAttitude;
    xis >> optional() >> attribute( "security-range", rPopulationSecurityRange_ )
        >> optional() >> attribute( "maximum", rPopulationMaximumCost_ )
        >> optional() >> attribute( "outside-of-population", rCostOutsideOfPopulation_ )
            >> list( "population-cost", *this, &DEC_Agent_PathClass::ReadPopulation );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadPopulation
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadPopulation( xml::xistream& xis )
{
    MT_Float rCost = 0.0f;
    std::string strAttitude;
    xis >> attribute( "attitude", strAttitude );
    const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( strAttitude );
    if( pAttitude )
    {
        xis >> attribute( "contact-cost", rCost );
        populationAttitudeCosts_[ pAttitude ] = rCost;
    }
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetPathClass
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
const DEC_Agent_PathClass& DEC_Agent_PathClass::GetPathClass( const DEC_PathType& pathType, const MIL_AgentPion& pion )
{
    const bool bCanFly     = pion.CanFly();
    const bool bAutonomous = pion.IsAutonomous();

    const DEC_Agent_PathClass* pClass = rules_[ T_RuleType( pathType.GetName(), T_BooleanPair( bCanFly, bAutonomous ) ) ];
    assert( pClass );
    return *pClass;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetObjectCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
MT_Float DEC_Agent_PathClass::GetObjectCost( const MIL_RealObjectType& objectType ) const
{
    assert( objectCosts_.size() > objectType.GetID() );
    return objectCosts_[ objectType.GetID() ];
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetPopulationAttitudeCost
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
MT_Float DEC_Agent_PathClass::GetPopulationAttitudeCost( const MIL_PopulationAttitude& attitude ) const
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
MT_Float DEC_Agent_PathClass::GetCostOutsideOfPopulation() const
{
    return rCostOutsideOfPopulation_;
}
