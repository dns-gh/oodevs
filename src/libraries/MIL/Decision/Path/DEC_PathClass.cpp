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
#include "DEC_PathClass.h"
#include "Agent/DEC_Agent_Path.h"
#include "Agent/DEC_Agent_PathfinderRule.h"
#include "Population/DEC_Population_Path.h"
#include "Population/DEC_Population_PathfinderRule.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Objects/MIL_RealObjectType.h"

// -----------------------------------------------------------------------------
// Name: DEC_PathClass constructor
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
DEC_PathClass::DEC_PathClass( MIL_InputArchive& archive, const DEC_PathClass* pCopyFrom /*= 0*/ )
    : bShort_                            ( false )
    , rPreferCost_                       ( 0 )
    , rAvoidCost_                        ( 0 )
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
    , rCostOnContact_                    ( 0 )
    , rCostAtSecurityRange_              ( 0 )
    , rMaximumCost_                      ( 100 )
{
    if( pCopyFrom )
        *this = *pCopyFrom;

    Initialize( archive );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass destructor
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
DEC_PathClass::~DEC_PathClass()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::CreateRule
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
TerrainRule_ABC& DEC_PathClass::CreateRule( const DEC_Agent_Path& path, const MT_Vector2D& from, const MT_Vector2D& to ) const
{
    DEC_Agent_PathfinderRule& rule = *new DEC_Agent_PathfinderRule( path.GetUnitSpeeds(), from, to, bShort_ );

    rule.SetAvoidedTerrain ( avoid_,  rAvoidCost_ );
    rule.SetPreferedTerrain( prefer_, rPreferCost_ );
    rule.SetAltitudePreference( path.GetUnitMaxSlope(), rAltitudePreference_ );
    if( ! path.GetFuseau().IsNull() )
    {
        MT_Float rMaxFuseauDistance = rMaximumFuseauDistance_;
        if( ! path.GetFuseau().IsInside( from ) )
            rMaxFuseauDistance += path.GetFuseau().Distance( from ) * 1.3; // $$$$ AGE 2005-06-08: 

        if( ! path.GetAutomataFuseau().IsNull() ) // I have a higher fuseau
            rMaxFuseauDistance = std::max( rMaxFuseauDistance, rMaximumFuseauDistanceWithAutomata_ );

        if( ! path.GetFuseau().IsInside( to ) ) // our destination is outside
            rMaxFuseauDistance = std::numeric_limits< MT_Float >::max();

        rule.SetFuseau( path.GetFuseau(), rMaxFuseauDistance, rFuseauCostPerMeterOut_, rComfortFuseauDistance_, rFuseauCostPerMeterIn_ );
    }
    if( ! path.GetAutomataFuseau().IsNull() )
    {
        MT_Float rMaxAutomataFuseauDistance = rMaximumAutomataFuseauDistance_;
        if( ! path.GetAutomataFuseau().IsInside( from ) )
            rMaxAutomataFuseauDistance += path.GetAutomataFuseau().Distance( from ) * 1.3;
        rule.SetAutomataFuseau( path.GetAutomataFuseau(), rMaxAutomataFuseauDistance, rAutomataFuseauCostPerMeterOut_ );
    }

    rule.SetDangerDirection( path.GetDirDanger(), rDangerDirectionBaseCost_, rDangerDirectionLinearCost_ );

    {
        if( rCostOnContact_ || rCostAtSecurityRange_ )
        {
            for( DEC_Agent_Path::CIT_PathKnowledgeAgentVector itAgent = path.GetPathKnowledgeAgents().begin(); itAgent != path.GetPathKnowledgeAgents().end(); ++itAgent )
                rule.AddEnemyKnowledge( *itAgent, rCostOnContact_, rCostAtSecurityRange_ );
            rule.SetMaximumEnemyCost( rMaximumCost_ );
        }
    }
    if( bAvoidObjects_ )
        for( DEC_Agent_Path::CIT_PathKnowledgeObjectVector itObject = path.GetPathKnowledgeObjects().begin(); itObject != path.GetPathKnowledgeObjects().end(); ++itObject )
            rule.AddObjectKnowledge( *itObject, GetObjectCost( itObject->GetTypeID() ) );

    return rule;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetObjectCost
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
MT_Float DEC_PathClass::GetObjectCost( uint nObjectTypeId )
{
    static T_ObjectCosts costs = InitializeObjectCosts();
    if( nObjectTypeId < costs.size() )
        return costs[ nObjectTypeId ];
    else
        return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::InitializeObjectCosts
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
DEC_PathClass::T_ObjectCosts DEC_PathClass::InitializeObjectCosts()
{
    // $$$$ AGE 2005-08-04: BOF a dit NLD
    T_ObjectCosts costs;

    const MIL_RealObjectType::T_ObjectTypeMap& objectTypes = MIL_RealObjectType::GetObjectTypes();
    costs.resize( objectTypes.size() );

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
            case MIL_RealObjectType::eHate   : costs[ objectType.GetID() ] = hate;   break;
            case MIL_RealObjectType::eAvoid  : costs[ objectType.GetID() ] = avoid;  break;
            case MIL_RealObjectType::eIgnore : costs[ objectType.GetID() ] = ignore; break;
            case MIL_RealObjectType::eEnjoy  : costs[ objectType.GetID() ] = enjoy;  break;
            case MIL_RealObjectType::ePrefer : costs[ objectType.GetID() ] = prefer; break;
            default:
                assert( false );
        }       
    }
    return costs;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::Initialize
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_PathClass::Initialize( MIL_InputArchive& archive )
{
    archive.ReadField( "GoStraight", bShort_, MIL_InputArchive::eNothing );
    if( archive.BeginList( "PreferTerrain", MIL_InputArchive::eNothing ) )
    {
        archive.ReadAttribute( "strength", rPreferCost_, MIL_InputArchive::eNothing );
        ReadTerrains( archive, prefer_ );
        archive.EndList();
    }
    if( archive.BeginList( "AvoidTerrain", MIL_InputArchive::eNothing ) )
    {
        archive.ReadAttribute( "strength", rAvoidCost_, MIL_InputArchive::eNothing );
        ReadTerrains( archive, avoid_ );
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
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::ReadTerrains
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_PathClass::ReadTerrains( MIL_InputArchive& archive, TerrainData& destinationData )
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
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::ReadFuseau
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_PathClass::ReadFuseau( MIL_InputArchive& archive )
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
// Name: DEC_PathClass::ReadAutomataFuseau
// Created: AGE 2005-08-12
// -----------------------------------------------------------------------------
void DEC_PathClass::ReadAutomataFuseau( MIL_InputArchive& archive )
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
// Name: DEC_PathClass::ReadDangerDirection
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_PathClass::ReadDangerDirection( MIL_InputArchive& archive )
{
    archive.ReadField( "BaseCostBeyond",     rDangerDirectionBaseCost_, MIL_InputArchive::eNothing );
    archive.ReadField( "CostPerMeterBeyond", rDangerDirectionLinearCost_, MIL_InputArchive::eNothing );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::ReadEnemiesCost
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_PathClass::ReadEnemiesCost( MIL_InputArchive& archive )
{
    archive.ReadField( "CostOnContact",       rCostOnContact_, MIL_InputArchive::eNothing );
    archive.ReadField( "CostAtSecurityRange", rCostAtSecurityRange_, MIL_InputArchive::eNothing );
    archive.ReadField( "MaximumCost",         rMaximumCost_, MIL_InputArchive::eNothing );
}
