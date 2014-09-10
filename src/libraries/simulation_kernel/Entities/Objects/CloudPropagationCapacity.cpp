// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "CloudPropagationCapacity.h"
#include "MIL_Time_ABC.h"
#include "MIL_Object_ABC.h"
#include "NBCAttribute.h"
#include "simulation_terrain/TER_Localisation.h"
#include "Entities/Objects/MIL_NbcAgentType.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Tools/MIL_Tools.h"
#include "meteo/Meteo.h"

BOOST_CLASS_EXPORT_IMPLEMENT( CloudPropagationCapacity )

// -----------------------------------------------------------------------------
// Name: CloudPropagationCapacity constructor
// Created: LGY 2011-07-04
// -----------------------------------------------------------------------------
CloudPropagationCapacity::CloudPropagationCapacity()
    : time_( 0 )
    , rCurrentCircleRadius_     ( 0.f )
    , rCurrentPropagationLength_( 0.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CloudPropagationCapacity constructor
// Created: LGY 2011-07-04
// -----------------------------------------------------------------------------
CloudPropagationCapacity::CloudPropagationCapacity( const CloudPropagationCapacity& capacity )
    : origin_                   ( capacity.origin_ )
    , rCurrentCircleRadius_     ( capacity.rCurrentCircleRadius_ )
    , rCurrentPropagationLength_( capacity.rCurrentPropagationLength_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CloudPropagationCapacity destructor
// Created: LGY 2011-07-04
// -----------------------------------------------------------------------------
CloudPropagationCapacity::~CloudPropagationCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CloudPropagationCapacity::Instanciate
// Created: LGY 2011-07-04
// -----------------------------------------------------------------------------
void CloudPropagationCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    CloudPropagationCapacity* capacity = new CloudPropagationCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC* >( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: CloudPropagationCapacity::Register
// Created: LGY 2011-07-04
// -----------------------------------------------------------------------------
void CloudPropagationCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: CloudPropagationCapacity::Finalize
// Created: LGY 2011-07-04
// -----------------------------------------------------------------------------
void CloudPropagationCapacity::Finalize( MIL_Object_ABC& object )
{
    origin_ = object.GetLocalisation().ComputeBarycenter();
    time_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    rCurrentCircleRadius_ = object.GetLocalisation().GetCircleRadius();
}

namespace
{
    double GetDestructionDelay( const NBCAttribute::T_NBCAgents& agents )
    {
        double delay = 0;
        for( auto it = agents.begin(); it != agents.end(); ++it )
            if( (*it)->IsGasPoisonous() )
                if( (*it)->GetGasLifeTime() > delay )
                    delay = (*it)->GetGasLifeTime();
        return delay;
    }

    double GetPropagationAngle( const NBCAttribute::T_NBCAgents& agents )
    {
        double angle = -1.f;
        for( auto it = agents.begin(); it != agents.end(); ++it )
            if( (*it)->IsGasPoisonous() )
                if( (*it)->GetGasPropagationAngle() > angle )
                    angle = (*it)->GetGasPropagationAngle();
        return angle;
    }
}

// -----------------------------------------------------------------------------
// Name: CloudPropagationCapacity::Update
// Created: LGY 2011-07-04
// -----------------------------------------------------------------------------
void CloudPropagationCapacity::Update( MIL_Object_ABC& object, unsigned int time )
{
    const NBCAttribute* pNBC = object.RetrieveAttribute< NBCAttribute >();
    if( !pNBC )
        return;
    const NBCAttribute::T_NBCAgents& agents = pNBC->GetNBCAgents();
    if( time - time_ >= GetDestructionDelay( agents ) )
    {
        object.MarkForDestruction();
        return;
    }
    double angle = GetPropagationAngle( agents );
    if( angle == -1.f )
        return;
    TER_Localisation newLocalisation;
    if( UpdateLocalisation( angle, newLocalisation ) )
        object.UpdateLocalisation( newLocalisation );
}

// -----------------------------------------------------------------------------
// Name: CloudPropagationCapacity::UpdateLocalisation
// Created: LGY 2011-07-04
// -----------------------------------------------------------------------------
bool CloudPropagationCapacity::UpdateLocalisation( double angle, TER_Localisation& newLocalisation )
{
    const weather::WindData& wind = MIL_Tools::GetWind( origin_ );
    if( wind.rSpeed_ <= MIL_Tools::ConvertSpeedMosToSim( MIL_NbcAgentType::GetMinPropagationSpeed() ) || angle >= MT_PI )
    {
        if( wind.rSpeed_ <= 0. )
            return false;
        rCurrentCircleRadius_ += wind.rSpeed_;
    }
    else
        rCurrentPropagationLength_ = std::max( rCurrentPropagationLength_, rCurrentCircleRadius_ ) + wind.rSpeed_;

    if ( rCurrentCircleRadius_ >= rCurrentPropagationLength_ || angle >= MT_PI )
    {
        T_PointVector points;
        points.push_back( origin_ );
        points.push_back( origin_ + MT_Vector2D( 0., rCurrentCircleRadius_ ) );
        newLocalisation.Reset( TER_Localisation::eCircle, points );
    }
    else
    {
        T_PointVector points;
        const double rInitialAngle = Angle( MT_Vector2D( 0., 1. ), wind.vDirection_ );
        for( double rAngle = 0; rAngle <= MT_PI; rAngle += MT_PI / 8 )
            points.push_back( MT_Vector2D( origin_.rX_ + rCurrentCircleRadius_ * cos( rInitialAngle - rAngle ), origin_.rY_ + rCurrentCircleRadius_ * sin( rInitialAngle - rAngle ) ) );
        points.push_back( points.back()  + rCurrentPropagationLength_ * wind.vDirection_.Rotated( -0.5 * angle ) );
        points.push_back( points.front() + rCurrentPropagationLength_ * wind.vDirection_.Rotated(  0.5 * angle ) );
        newLocalisation.Reset( TER_Localisation::ePolygon, points );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: CloudPropagationCapacity::serialize
// Created: LGY 2011-07-04
// -----------------------------------------------------------------------------
template< typename Archive >
void CloudPropagationCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & origin_;
    file & rCurrentCircleRadius_;
    file & rCurrentPropagationLength_;
    file & time_;
}
