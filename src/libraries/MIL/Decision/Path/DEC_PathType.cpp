//*****************************************************************************
//
// $Created: JDY 03-04-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathType.cpp $
// $Author: Age $
// $Modtime: 16/06/05 15:42 $
// $Revision: 6 $
// $Workfile: DEC_PathType.cpp $
//
//*****************************************************************************
#include "MIL_pch.h"
#include "DEC_PathType.h"
#include "DEC_PathfinderRule.h"
#include "DEC_Path.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Objects/MIL_RealObjectType.h"

//-----------------------------------------------------------------------------
// Name: DEC_PathType constructor
// Created: JDY 03-04-14
//-----------------------------------------------------------------------------
DEC_PathType::DEC_PathType()
    : pathType_(eInfoMovement)
{
}


//-----------------------------------------------------------------------------
// Name: DEC_PathType destructor
// Created: JDY 03-04-14
//-----------------------------------------------------------------------------
DEC_PathType::~DEC_PathType()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_PathType::CreateRule
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
TerrainRule_ABC& DEC_PathType::CreateRule( const DEC_Path& path, const MT_Vector2D& from, const MT_Vector2D& to )
{
    // The terrain types depends on the type of path
    // And on whether the unit can fly
    const bool bCanFly = path.CanQueryMakerFly();
    TerrainData avoid  = AvoidedTerrain ( pathType_, bCanFly )
              , prefer = PreferedTerrain( pathType_, bCanFly );

    const bool bShort = pathType_ == eInfoRetreat || pathType_ == eInfoBackup;

    DEC_PathfinderRule::E_AltitudePreference altPreference = DEC_PathfinderRule::eDontCare;
    if( pathType_ == eInfoInfiltration )
        altPreference = DEC_PathfinderRule::ePreferLow;
    else if( ! bCanFly && pathType_ == eInfoRecon )
        altPreference = DEC_PathfinderRule::ePreferHigh;

    MT_Float rMaxFuseauDistance = 1000.;
    if( ! path.GetFuseau().IsInside( from ) )
        rMaxFuseauDistance += path.GetFuseau().Distance( from ) * 1.3; // $$$$ AGE 2005-06-08: Hard coded margin

    if( ! path.GetFuseau().IsInside( to ) // our destination is outside
     ||   bCanFly                         // I'm an alat cowboy
     ||   pathType_ == eInfoRetreat )     // I'm fleeing away
        rMaxFuseauDistance = std::numeric_limits< MT_Float >::max();
    
    DEC_PathfinderRule& rule = *new DEC_PathfinderRule( path, avoid, prefer, from, to, bShort, altPreference, rMaxFuseauDistance );

    const bool bAvoidEnis = pathType_ == eInfoRetreat || pathType_ == eInfoBackup || pathType_ == eInfoInfiltration;
    if( bAvoidEnis )
        for( DEC_Path::CIT_PathKnowledgeAgentVector itAgent = path.GetPathKnowledgeAgents().begin(); itAgent != path.GetPathKnowledgeAgents().end(); ++itAgent )
            rule.AddEnemyKnowledge( *itAgent );

    // Always
    for( DEC_Path::CIT_PathKnowledgeObjectVector itObject = path.GetPathKnowledgeObjects().begin(); itObject != path.GetPathKnowledgeObjects().end(); ++itObject )
    {
        MT_Float rCostIn, rCostOut;
        GetObjectCosts( itObject->GetTypeID(), rCostIn, rCostOut );
        rule.AddObjectKnowledge( *itObject, rCostIn, rCostOut );
    }
    return rule;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathType::PreferedTerrain
// Created: AGE 2005-03-25
// -----------------------------------------------------------------------------
TerrainData DEC_PathType::PreferedTerrain( E_PathType type, bool bFly )
{
    static TerrainData roads = TerrainData::SmallRoad()
                       .Merge( TerrainData::MediumRoad() )
                       .Merge( TerrainData::LargeRoad() )
                       .Merge( TerrainData::Motorway() );
    static TerrainData groundTerrains[6] =
    {
        roads,
        TerrainData::ForestBorder().Merge( roads ),
        TerrainData::Forest(),
        TerrainData(),
        TerrainData(),
        TerrainData()
    };
    static TerrainData rivers = TerrainData::SmallRiver()
                        .Merge( TerrainData::MediumRiver() )
                        .Merge( TerrainData::LargeRiver() );

    static TerrainData airTerrains[6] =
    {
        TerrainData(),
        TerrainData(),
        TerrainData(),
        TerrainData(),
        TerrainData(),
        TerrainData()
    };
    if( bFly && type < 6 )
        return airTerrains[ type ];
    if( ! bFly && type < 6 )
        return groundTerrains[ type ];
    return TerrainData();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathType::AvoidedTerrain
// Created: AGE 2005-03-25
// -----------------------------------------------------------------------------
TerrainData DEC_PathType::AvoidedTerrain( E_PathType type, bool bFly )
{
    static TerrainData urban = TerrainData::Urban().Merge( TerrainData::UrbanBorder() );
    static TerrainData groundTerrains[6] =
    {
        TerrainData(),
        TerrainData(),
        urban,
        TerrainData(),
        TerrainData(),
        TerrainData()
    };
    static TerrainData roads = TerrainData::SmallRoad()
                       .Merge( TerrainData::MediumRoad() )
                       .Merge( TerrainData::LargeRoad() )
                       .Merge( TerrainData::Motorway() );
    static TerrainData airAvoid = TerrainData::ForestBorder()
                          .Merge( urban )
                          .Merge( roads );
    static TerrainData airTerrains[6] =
    {
        airAvoid,
        urban,
        airAvoid,
        TerrainData(),
        TerrainData(),
        TerrainData()
    };
    if( bFly && type < 6 )
        return airTerrains[ type ];
    if( ! bFly && type < 6 )
        return groundTerrains[ type ];
    return TerrainData();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathType::GetObjectCosts
// Created: AGE 2005-03-30
// -----------------------------------------------------------------------------
void DEC_PathType::GetObjectCosts( uint nObjectTypeId, MT_Float& rCostIn, MT_Float& rCostOut )
{
    static std::vector< std::pair< MT_Float, MT_Float > > costs = InitializeObjectCosts();
    if( nObjectTypeId < costs.size() )
    {
        rCostIn  = costs[ nObjectTypeId ].first;
        rCostOut = costs[ nObjectTypeId ].second;
    }
    else
        rCostIn = rCostOut = 0;
}

// -----------------------------------------------------------------------------
// Name: std::pair< MT_Float, MT_Float > > DEC_PathType::InitializeObjectCosts
// Created: AGE 2005-03-30
// -----------------------------------------------------------------------------
std::vector< std::pair< MT_Float, MT_Float > > DEC_PathType::InitializeObjectCosts()
{
    typedef std::pair< MT_Float, MT_Float > T_CostPair;
    std::vector< std::pair< MT_Float, MT_Float > > costs;
    costs.resize( MIL_RealObjectType::GetObjectTypes().size() );

    const T_CostPair ignore( 0    , 0  );
    const T_CostPair avoid ( 10   , 0  );
    const T_CostPair loathe( 10000, 0  );
    const T_CostPair enjoy ( 0    , 1  );
    const T_CostPair prefer( 0    , 5  );

    costs[ MIL_RealObjectType::fosseAntiChar_          .GetID() ] = loathe;
    costs[ MIL_RealObjectType::abattis_                .GetID() ] = loathe;
    costs[ MIL_RealObjectType::barricade_              .GetID() ] = loathe;
    costs[ MIL_RealObjectType::bouchonMines_           .GetID() ] = loathe;
    costs[ MIL_RealObjectType::zoneMineeLineaire_      .GetID() ] = loathe;
    costs[ MIL_RealObjectType::zoneMineeParDispersion_ .GetID() ] = loathe;
    costs[ MIL_RealObjectType::eboulement_             .GetID() ] = loathe;
    costs[ MIL_RealObjectType::destructionRoute_       .GetID() ] = loathe;
    costs[ MIL_RealObjectType::destructionPont_        .GetID() ] = loathe;
    costs[ MIL_RealObjectType::pontFlottant_           .GetID() ] = ignore;
    costs[ MIL_RealObjectType::posteTir_               .GetID() ] = ignore;
    costs[ MIL_RealObjectType::zoneProtegee_           .GetID() ] = ignore;
    costs[ MIL_RealObjectType::zoneImplantationCanon_  .GetID() ] = avoid;
    costs[ MIL_RealObjectType::zoneImplantationCOBRA_  .GetID() ] = avoid;
    costs[ MIL_RealObjectType::zoneImplantationLRM_    .GetID() ] = avoid;
    costs[ MIL_RealObjectType::siteFranchissement_     .GetID() ] = ignore;
    costs[ MIL_RealObjectType::nuageNBC_               .GetID() ] = loathe;
    costs[ MIL_RealObjectType::siteDecontamination_    .GetID() ] = avoid;
    costs[ MIL_RealObjectType::plotRavitaillement_     .GetID() ] = avoid;
    costs[ MIL_RealObjectType::zoneBrouillageBrod_     .GetID() ] = loathe;
    costs[ MIL_RealObjectType::zoneBrouillageBromure_  .GetID() ] = loathe;
    costs[ MIL_RealObjectType::rota_                   .GetID() ] = loathe;
    costs[ MIL_RealObjectType::zoneNBC_                .GetID() ] = loathe;
    costs[ MIL_RealObjectType::airePoser_              .GetID() ] = avoid;
    costs[ MIL_RealObjectType::piste_                  .GetID() ] = prefer;
    costs[ MIL_RealObjectType::plateForme_             .GetID() ] = avoid;
    costs[ MIL_RealObjectType::zoneMobiliteAmelioree_  .GetID() ] = prefer;
    costs[ MIL_RealObjectType::zonePoserHelicoptere_   .GetID() ] = avoid;
    costs[ MIL_RealObjectType::aireLogistique_         .GetID() ] = avoid;
    costs[ MIL_RealObjectType::campPrisonniers_        .GetID() ] = avoid;
    costs[ MIL_RealObjectType::campRefugies_           .GetID() ] = avoid;
    costs[ MIL_RealObjectType::itineraireLogistique_   .GetID() ] = enjoy;
    costs[ MIL_RealObjectType::posteControle_          .GetID() ] = prefer;
    costs[ MIL_RealObjectType::terrainLargage_         .GetID() ] = avoid;
    costs[ MIL_RealObjectType::zoneForbiddenFire_      .GetID() ] = loathe;
    costs[ MIL_RealObjectType::zoneForbiddenMove_      .GetID() ] = loathe;
    costs[ MIL_RealObjectType::zoneImplantationMortier_.GetID() ] = avoid;

    return costs;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathType::ConvertPathTypeToString
// Created: NLD 2003-09-25
// -----------------------------------------------------------------------------
std::string DEC_PathType::ConvertPathTypeToString() const
{
    static std::string names[6] = {
        "eInfoMovement",
        "eInfoRecon",
        "eInfoInfiltration",
        "eInfoAssault",
        "eInfoRetreat",
        "eInfoBackup"
    };
    if( pathType_ < 6 )
        return names[ pathType_ ];
    return "bad type";
}
