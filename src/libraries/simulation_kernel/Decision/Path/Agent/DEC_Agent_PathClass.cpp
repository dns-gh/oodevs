// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-08-04 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "DEC_Agent_PathClass.h"

#include "Decision/Path/DEC_PathType.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"

DEC_Agent_PathClass::T_Rules       DEC_Agent_PathClass::rules_;
DEC_Agent_PathClass::T_ObjectCosts DEC_Agent_PathClass::objectCosts_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::Initialize
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::Initialize( MIL_InputArchive& archive )
{
    InitializeObjectCosts();
    InitializeRules( archive );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::InitializeObjectCosts
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::InitializeObjectCosts()
{
    const MIL_RealObjectType::T_ObjectTypeMap& objectTypes = MIL_RealObjectType::GetObjectTypes();
    objectCosts_.resize( objectTypes.size() );

    const MT_Float ignore( 0 );
    const MT_Float avoid ( 10 );
    const MT_Float hate  ( 10000 );
    const MT_Float enjoy ( -1 );
    const MT_Float prefer( -5 );

    for( MIL_RealObjectType::CIT_ObjectTypeMap it = objectTypes.begin(); it != objectTypes.end(); ++it )
    {
        const MIL_RealObjectType& objectType = *it->second;
        switch( objectType.GetBehavior() )
        {
            case MIL_RealObjectType::eHate   : objectCosts_[ objectType.GetID() ] = hate;   break;
            case MIL_RealObjectType::eAvoid  : objectCosts_[ objectType.GetID() ] = avoid;  break;
            case MIL_RealObjectType::eIgnore : objectCosts_[ objectType.GetID() ] = ignore; break;
            case MIL_RealObjectType::eEnjoy  : objectCosts_[ objectType.GetID() ] = enjoy;  break;
            case MIL_RealObjectType::ePrefer : objectCosts_[ objectType.GetID() ] = prefer; break;
            default:
                assert( false );
        }       
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::InitializeRules
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::InitializeRules( MIL_InputArchive& archive )
{
    archive.BeginList( "AgentRules" );
    while( archive.NextListElement() )
    {
        archive.Section( "Rule" );
        std::string strType;
        bool        bFlying;
        bool        bAutonomous;

        archive.ReadAttribute( "type"      , strType     );
        archive.ReadAttribute( "flying"    , bFlying     );
        archive.ReadAttribute( "autonomous", bAutonomous );

        std::string strBase;
        const DEC_Agent_PathClass* pBase = 0;
        if( archive.ReadAttribute( "inherits", strBase, MIL_InputArchive::eNothing ) )
        {
            pBase  = rules_[ T_RuleType( strBase, T_BooleanPair( bFlying, bAutonomous ) ) ];
            if( !pBase )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "base rule '" + strBase + "' is undefined", archive.GetContext() );
        }
        DEC_Agent_PathClass*& pRule = rules_[ T_RuleType( strType, T_BooleanPair( bFlying, bAutonomous ) ) ];
        if( pRule )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Rule '" + strType + "' already defined", archive.GetContext() );
        pRule = new DEC_Agent_PathClass( archive, pBase );
        archive.EndSection(); // Rule
    };
    archive.EndList(); // AgentRules

    CheckRulesExistence();
}

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
DEC_Agent_PathClass::DEC_Agent_PathClass( MIL_InputArchive& archive, const DEC_Agent_PathClass* pCopyFrom /*= 0*/ )
    : bShort_                            ( false )
    , rPreferedTerrainCost_              ( 0 )
    , rAvoidedTerrainCost_               ( 0 )
    , bAvoidObjects_                     ( true )
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

    archive.ReadField( "GoStraight", bShort_, MIL_InputArchive::eNothing );
    if( archive.BeginList( "PreferTerrain", MIL_InputArchive::eNothing ) )
    {
        archive.ReadAttribute( "strength", rPreferedTerrainCost_, MIL_InputArchive::eNothing );
        ReadTerrains( archive, preferedTerrain_ );
        archive.EndList();
    }
    if( archive.BeginList( "AvoidTerrain", MIL_InputArchive::eNothing ) )
    {
        archive.ReadAttribute( "strength", rAvoidedTerrainCost_, MIL_InputArchive::eNothing );
        ReadTerrains( archive, avoidedTerrain_ );
        archive.EndList();
    }
    archive.ReadField( "AvoidObjects",       bAvoidObjects_,       MIL_InputArchive::eNothing );
    archive.ReadField( "AltitudePreference", rAltitudePreference_, MIL_InputArchive::eNothing );

    if( archive.Section( "Fuseau", MIL_InputArchive::eNothing ) )
    {
        ReadFuseau( archive );
        archive.EndSection(); // Fuseau
    }
    if( archive.Section( "AutomataFuseau", MIL_InputArchive::eNothing ) )
    {
        ReadAutomataFuseau( archive );
        archive.EndSection(); // AutomataFuseau
    }

    if( archive.Section( "DangerDirection", MIL_InputArchive::eNothing ) )
    {
        ReadDangerDirection( archive );
        archive.EndSection(); // DangerDirection
    }

    if( archive.Section( "Enemies", MIL_InputArchive::eNothing ) )
    {
        ReadEnemiesCost( archive );
        archive.EndSection(); // Enemies
    }

    if( archive.Section( "Populations", MIL_InputArchive::eNothing ) )
    {
        ReadPopulationsCost( archive );
        archive.EndSection(); // Populations
    }
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
// Name: DEC_Agent_PathClass::ReadTerrains
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadTerrains( MIL_InputArchive& archive, TerrainData& destinationData )
{
    destinationData = TerrainData();
    while( archive.NextListElement() )
    {
        archive.Section( "Terrain" );
        std::string strType;
        archive.ReadAttribute( "type", strType );
        const TerrainData data = TerrainData( strType );
        if( data.Area() == 0xFF )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown terrain type '" + strType + "'", archive.GetContext() );
        destinationData.Merge( data );
        archive.EndSection(); // Terrain
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadFuseau
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadFuseau( MIL_InputArchive& archive )
{
    if( archive.Section( "ToleratedDistanceOutside", MIL_InputArchive::eNothing ) )
    {
        archive.ReadAttribute( "costPerMeter", rFuseauCostPerMeterOut_, MIL_InputArchive::eNothing );
        archive.ReadField( "WithoutAutomata", rMaximumFuseauDistance_, MIL_InputArchive::eNothing );
        archive.ReadField( "WithAutomata", rMaximumFuseauDistanceWithAutomata_, MIL_InputArchive::eNothing );
        archive.EndSection(); // ToleratedDistanceOutside
    }
    if( archive.Section( "ComfortDistanceInside", MIL_InputArchive::eNothing ) )
    {
        archive.ReadAttribute( "costPerMeter", rFuseauCostPerMeterIn_, MIL_InputArchive::eNothing );
        archive.Read( rComfortFuseauDistance_ );
        archive.EndSection(); //ComfortDistanceInside
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadAutomataFuseau
// Created: AGE 2005-08-12
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadAutomataFuseau( MIL_InputArchive& archive )
{
    if( archive.Section( "ToleratedDistanceOutside", MIL_InputArchive::eNothing ) )
    {
        archive.ReadAttribute( "costPerMeter", rAutomataFuseauCostPerMeterOut_, MIL_InputArchive::eNothing );
        archive.Read( rMaximumAutomataFuseauDistance_ );
        if( rMaximumAutomataFuseauDistance_ < 10 )
            rMaximumAutomataFuseauDistance_ = 10;
        archive.EndSection(); // ToleratedDistanceOutside
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadDangerDirection
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadDangerDirection( MIL_InputArchive& archive )
{
    archive.ReadField( "BaseCostBeyond"    , rDangerDirectionBaseCost_  , MIL_InputArchive::eNothing );
    archive.ReadField( "CostPerMeterBeyond", rDangerDirectionLinearCost_, MIL_InputArchive::eNothing );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadEnemiesCost
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadEnemiesCost( MIL_InputArchive& archive )
{
    archive.ReadField( "CostOnContact"      , rEnemyCostOnContact_      , MIL_InputArchive::eNothing );
    archive.ReadField( "CostAtSecurityRange", rEnemyCostAtSecurityRange_, MIL_InputArchive::eNothing );
    archive.ReadField( "MaximumCost"        , rEnemyMaximumCost_        , MIL_InputArchive::eNothing );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::ReadPopulationsCost
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
void DEC_Agent_PathClass::ReadPopulationsCost( MIL_InputArchive& archive )
{
    std::string strAttitude;
    MT_Float rCost = 0.0f;
    archive.ReadField( "SecurityRange"         , rPopulationSecurityRange_, MIL_InputArchive::eNothing );
    archive.ReadField( "MaximumCost"           , rPopulationMaximumCost_  , MIL_InputArchive::eNothing );
    archive.ReadField( "CostOusideOfPopulation", rCostOutsideOfPopulation_, MIL_InputArchive::eNothing );
    archive.BeginList( "CostsOnContact", MIL_InputArchive::eNothing );
    while( archive.NextListElement() )
    {
        archive.Section( "CostOnContact" );
        archive.ReadAttribute( "attitude", strAttitude );
        const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( strAttitude );
        if( pAttitude )
        {
            archive.Read( rCost ); 
            populationAttitudeCosts_[ pAttitude ] = rCost;
        }
        archive.EndSection(); // CostOnContact
    }
    archive.EndList(); // CostsOnContact
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
// Name: DEC_Agent_PathClass::GetObjectCosts
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
MT_Float DEC_Agent_PathClass::GetObjectCosts( const MIL_RealObjectType& objectType ) const
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
