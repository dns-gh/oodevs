// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "TerrainHeuristicCapacity.h"
#include "Object.h"
#include "Tools/MIL_Tools.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( TerrainHeuristicCapacity, "TerrainHeuristicCapacity" )

// -----------------------------------------------------------------------------
// Name: TerrainHeuristicCapacity constructor
// Created: LDC 2009-03-06
// -----------------------------------------------------------------------------
TerrainHeuristicCapacity::TerrainHeuristicCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainHeuristicCapacity::TerrainHeuristicCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
TerrainHeuristicCapacity::TerrainHeuristicCapacity( xml::xistream& xis )
{
    // Placement scores
    xis >> xml::list( "terrain", *this, &TerrainHeuristicCapacity::ReadTerrain );
}

// -----------------------------------------------------------------------------
// Name: TerrainHeuristicCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
TerrainHeuristicCapacity::TerrainHeuristicCapacity( const TerrainHeuristicCapacity& from )
{
    environmentScores_ = from.environmentScores_;
}
	
// -----------------------------------------------------------------------------
// Name: TerrainHeuristicCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
TerrainHeuristicCapacity::~TerrainHeuristicCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::ReadTerrain
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void TerrainHeuristicCapacity::ReadTerrain( xml::xistream& xis )
{
    std::string strTerrainType( xml::attribute( xis, "type", std::string() ) );
    uint nScore = xml::attribute( xis, "value", 0 );

    TerrainData nLandType = MIL_Tools::ConvertLandType( strTerrainType );
    if( nLandType.Area() == 0xFF )
        xis.error( "Unknown land type" );

    environmentScores_.push_back( std::make_pair( nLandType, nScore ) );
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void TerrainHeuristicCapacity::serialize
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
template< typename Archive > void TerrainHeuristicCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
         & environmentScores_;
}
    
// -----------------------------------------------------------------------------
// Name: TerrainHeuristicCapacity::Register
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void TerrainHeuristicCapacity::Register( Object& object )
{
    object.AddCapacity( this );
} 

// -----------------------------------------------------------------------------
// Name: TerrainHeuristicCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void TerrainHeuristicCapacity::Instanciate( Object& object ) const
{
    object.AddCapacity( new TerrainHeuristicCapacity( *this ) );
}

// -----------------------------------------------------------------------------
// Name: TerrainHeuristicCapacity::ComputePlacementScore
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
int TerrainHeuristicCapacity::ComputePlacementScore( const MT_Vector2D& pos, const TerrainData& nPassability ) const
{
    int nScore = -1;
    for( CIT_EnvironmentScoreMap itEnvScore = environmentScores_.begin(); itEnvScore != environmentScores_.end(); ++itEnvScore )
    {
        if( nPassability.ContainsAll( itEnvScore->first ) )
        {
            if( nScore == -1 )
                nScore = 0;
            nScore += itEnvScore->second;
        }
    }
    return nScore;
}
