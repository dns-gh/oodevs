// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Path_KnowledgeObjectDisaster.h"
#include "OnComponentComputer_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Objects/DisasterAttribute.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MobilityCapacity.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "propagation/Extractor_ABC.h"
#include "simulation_terrain/TER_Localisation.h"
#include "simulation_terrain/TER_World.h"

namespace
{
    class ComposanteTypesComputer : public OnComponentComputer_ABC
    {
    public:
        explicit ComposanteTypesComputer( std::set< const PHY_ComposanteTypePion* >& composantes )
            : composantes_( composantes )
        {
            // NOTHING
        }
        virtual ~ComposanteTypesComputer()
        {
            // NOTHING
        }
        virtual void ApplyOnComponent( PHY_ComposantePion& composante )
        {
            if( composante.IsUsable() )
                composantes_.insert( &composante.GetType() );
        }

    private:
        std::set< const PHY_ComposanteTypePion* >& composantes_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectDisaster constructor
// Created: JSR 2014-04-23
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeObjectDisaster::DEC_Path_KnowledgeObjectDisaster( const MIL_Agent_ABC& agent, const DEC_Knowledge_Object& knowledge )
    : localisation_( new TER_Localisation( knowledge.GetLocalisation() ) )
    , maxTrafficability_( knowledge.GetMaxTrafficability() )
    , objectType_( knowledge.GetType() )
    , maxSpeedModifier_( 1 )
{
    ComposanteTypesComputer computer( composantes_ );
    const_cast< MIL_Agent_ABC& >( agent ).Execute< OnComponentComputer_ABC >( computer );
    localisation_->Scale( 500 ); // to avoid the path to be too close to the known zone border, which could result in an unit blocking
    const DisasterAttribute* attribute = knowledge.RetrieveAttribute< DisasterAttribute >();
    if( attribute )
        extractors_ = attribute->GetExtractors();
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectDisaster destructor
// Created: JSR 2014-04-23
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeObjectDisaster::~DEC_Path_KnowledgeObjectDisaster()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectDisaster::ComputeCost
// Created: JSR 2014-04-23
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeObjectDisaster::ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& /*nToTerrainType*/, const TerrainData& /*nLinkTerrainType*/, double /*weight*/ ) const
{
    if( extractors_.empty() )
        return std::numeric_limits< double >::min();
    maxSpeedModifier_ = 1;
    const float maxValue = GetMaxValueOnPath( from, to );
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        maxSpeedModifier_ = std::min( maxSpeedModifier_, ( *it )->GetDisasterImpact( maxValue ) );
    return maxSpeedModifier_ > 0.3 ? 0
         : maxSpeedModifier_ > 0.2 ? 100
         : maxSpeedModifier_ > 0.1 ? 1000
         : 10000;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectDisaster::GetCostOut
// Created: JSR 2014-04-23
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeObjectDisaster::GetCostOut() const
{
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectDisaster::GetMaxTrafficability
// Created: JSR 2014-04-23
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeObjectDisaster::GetMaxTrafficability() const
{
    return maxTrafficability_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectDisaster::ComputeAgentMaxSpeed
// Created: JSR 2014-09-18
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeObjectDisaster::ComputeAgentMaxSpeed( double speed, double maxSpeed ) const
{
    const MobilityCapacity* mobility = objectType_.GetCapacity< MobilityCapacity >();
    if( mobility && mobility->IsMaxSpeed() )
        return maxSpeed * std::max( 0.1, maxSpeedModifier_ * mobility->ApplySpeedPolicy( 1., 1., 1., 1. ) );
    return speed * std::max( 0.1, maxSpeedModifier_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectDisaster::GetMaxValueOnPath
// Created: JSR 2014-04-24
// -----------------------------------------------------------------------------
float DEC_Path_KnowledgeObjectDisaster::GetMaxValueOnPath( const MT_Vector2D& from, const MT_Vector2D& to ) const
{
    if( !localisation_->Intersect2D( MT_Line( from, to ) ) )
        return 0;
    const double distance = from.Distance( to );
    const MT_Vector2D normalized = ( to - from ).Normalize();
    float maxValue = 0;
    for( auto it = extractors_.begin(); it != extractors_.end(); ++it )
    {
        const double delta = ( *it )->GetPixelSize().X();
        for( double d = 0; d < distance + delta; d += delta )
        {
            const MT_Vector2D pos = d >= distance ? to : ( from + d * normalized );
            if( localisation_->IsInside( pos ) )
            {
                double latitude, longitude;
                TER_World::GetWorld().SimToMosMgrsCoord( pos, latitude, longitude );
                maxValue = std::max( maxValue, ( *it )->GetValue( longitude, latitude ) );
            }
        }
    }
    return maxValue;
}
