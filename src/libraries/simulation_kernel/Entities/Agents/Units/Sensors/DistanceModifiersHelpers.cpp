// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DistanceModifiersHelpers.h"
#include "MIL_AgentServer.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "meteo/RawVisionData/PHY_RawVisionData.h"
#include "Urban/MIL_UrbanCache.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Urban/UrbanPhysicalCapacity.h"
#include "Urban/PHY_MaterialCompositionType.h"

namespace distance_modifiers
{

std::map< std::string, double > ReadDistanceModifiers( xml::xistream& xis, const std::string& parent )
{
    const std::string attr = "distance-modifier";
    std::map< std::string, double > factors;
    xis >> xml::start( parent )
            >> xml::list( attr, [&]( xml::xistream& xis )
            {
                const double factor = xis.attribute< double >( "value" );
                if( factor < 0 || factor > 1 )
                    throw MASA_EXCEPTION( xis.context() + attr + ": value not in [0..1]" );
                factors[ xis.attribute< std::string >( "type" ) ] = factor;
            })
        >> xml::end;
    return factors;
}

void ReadPostureFactors( xml::xistream& xis, const std::string& parent, std::vector< double >& factors )
{
    auto values = ReadDistanceModifiers( xis, parent );
    for( auto it = values.cbegin(); it != values.cend(); ++it )
    {
        const PHY_Posture* posture = PHY_Posture::FindPosture( it->first );
        if( !posture )
            throw MASA_EXCEPTION( "unknown distance-modifier: " + it->first );
        if( !posture->CanModifyDetection() )
            continue;
        factors.at( posture->GetID() ) = it->second;
    }
}

void InitializeTerrainModifiers( xml::xistream& xis, std::map< unsigned int, double >& factors )
{
    xis >> xml::start( "terrain-modifiers" )
        >> xml::list( "distance-modifier", [&]( xml::xistream& xis )
        {
            const std::string terrainType = xis.attribute< std::string >( "type" );
            const double rFactor = xis.attribute< double >( "value" );
            if( rFactor < 0 || rFactor > 1 )
                xis.error( "terrain-modifier: value not in [0..1]" );
            factors.insert( std::make_pair( PHY_RawVisionData::environmentAssociation_[ terrainType ], rFactor ) );
        })
        >> xml::end;
}

void InitializeUrbanFactors( xml::xistream& xis, std::vector< double >& factors )
{
    xis >> xml::start( "urbanBlock-material-modifiers" )
            >> xml::list( "distance-modifier", [&]( xml::xistream& xis )
            {
                const std::string materialType = xis.attribute< std::string >( "type" );
                const PHY_MaterialCompositionType* type = PHY_MaterialCompositionType::Find( materialType );
                if( !type )
                    throw MASA_EXCEPTION( xis.context() + "material type " + materialType + " doesn't exist" );
                const double rFactor = xis.attribute< double >( "value" );
                if( rFactor < 0 || rFactor > 1 )
                    throw MASA_EXCEPTION( xis.context() + "urbanBlock-modifier: value not in [0..1]" );
                factors[ type->GetId() ] = rFactor;
            })
        >> xml::end;
}

bool ComputeUrbanExtinction( const MT_Vector2D& vSource, const MT_Vector2D& vTarget, double& rVisionNRJ, const std::vector< double >& factors, bool posted )
{
    std::vector< const MIL_UrbanObject_ABC* > list;
    MIL_AgentServer::GetWorkspace().GetUrbanCache().GetUrbanBlocksWithinSegment( vSource, vTarget, list );
    if( list.empty() )
        return false;

    bool isAroundBU = false;

    for( auto it = list.begin(); it != list.end() && rVisionNRJ >= 0; ++it )
    {
        const MIL_UrbanObject_ABC& object = **it;
        const UrbanPhysicalCapacity* pPhysical = object.Retrieve< UrbanPhysicalCapacity >();
        if( pPhysical == 0 )
            continue;
        const PHY_MaterialCompositionType* materialCompositionType = PHY_MaterialCompositionType::Find( pPhysical->GetMaterial() );
        if( !materialCompositionType )
            continue;

        const TER_Localisation& footPrint = object.GetLocalisation();

        TER_DistanceLess cmp ( vSource );
        T_PointSet intersectPoints( cmp );
        if( footPrint.IsInside( vSource ) || footPrint.IsInside( vTarget ) || footPrint.Intersect2D( MT_Line( vSource, vTarget ), intersectPoints ) )
        {
            isAroundBU = true;
            double intersectionDistance = 0;
            if( intersectPoints.empty() )
                intersectionDistance = posted ? 0 : vSource.Distance( vTarget );
            else if( intersectPoints.size() == 1 )
            {
                if( footPrint.IsInside( vSource ) )
                    intersectionDistance = posted ? 0 : vSource.Distance( *intersectPoints.begin() );
                else if( footPrint.IsInside( vTarget ) )
                    intersectionDistance = vTarget.Distance( *intersectPoints.begin() );
            }
            else
                intersectionDistance = ( *intersectPoints.begin() ).Distance( *intersectPoints.rbegin() );
            if( intersectionDistance == 0 )
                continue;
            const double rDistanceModificator = factors[ materialCompositionType->GetId() ];
            const double occupationFactor = std::sqrt( pPhysical->GetOccupation() );
            if( occupationFactor == 1. && rDistanceModificator <= 1e-8 )
                rVisionNRJ = -1 ;
            else
            {
                static const double referenceDistance = 200; // $$$$ LDC Hard coded 200m. reference distance
                const double distanceFactor = std::min( ( intersectionDistance / referenceDistance ) * occupationFactor * ( 1 - rDistanceModificator ), 1. );
                rVisionNRJ -= rVisionNRJ * distanceFactor + intersectionDistance;
            }
        }
    }
    return isAroundBU;
}

}
