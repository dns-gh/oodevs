// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FirePropagationCapacity.h"
#include "Object.h"
#include "FireAttribute.h"
#include "MIL_PropagationManager.h"
#include "Entities/MIL_EntityManager.h"
#include "Tools/MIL_Tools.h"
#include "MIL_ObjectBuilder_ABC.h"
#include "BurnCapacity.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( FirePropagationCapacity, "FirePropagationCapacity" )

// -----------------------------------------------------------------------------
// Name: FirePropagationCapacity constructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
FirePropagationCapacity::FirePropagationCapacity( xml::xistream& xis, MIL_PropagationManager& manager )
	: pManager_ ( &manager )
	, timeOfCreation_ ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
	, timeOfDeath_ ( 0 )
    , needUpdate_ ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePropagationCapacity constructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
FirePropagationCapacity::FirePropagationCapacity()
	: pManager_ ( 0 )
	, timeOfCreation_ ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
	, timeOfDeath_ ( 0 )
    , needUpdate_ ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePropagationCapacity constructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
FirePropagationCapacity::FirePropagationCapacity( const FirePropagationCapacity& from )
    : pManager_ ( from.pManager_ )
	, timeOfCreation_ ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
	, timeOfDeath_ ( 0 )
    , needUpdate_ ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePropagationCapacity destructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
FirePropagationCapacity::~FirePropagationCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtinguishableCapacity::serialize
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void FirePropagationCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );        
    file //& pManager_
         & timeOfCreation_
	     & timeOfDeath_;
}

// -----------------------------------------------------------------------------
// Name: FirePropagationCapacity::Register
// Created: RFT 2008-07-03
// -----------------------------------------------------------------------------
void FirePropagationCapacity::Register( Object& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: FirePropagationCapacity::Instanciate
// Created: RFT 2008-06-08
// -----------------------------------------------------------------------------
void FirePropagationCapacity::Instanciate( Object& object ) const
{
    FirePropagationCapacity* capacity = new FirePropagationCapacity( *this );
    object.AddCapacity< PropagationCapacity_ABC >( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: FirePropagationCapacity::Update
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
void FirePropagationCapacity::Update( Object& object, uint time )
{	
    FireAttribute& attr = object.GetAttribute< FireAttribute >();
	const unsigned int timeSinceCreation = time - timeOfCreation_;
	
    if( needUpdate_ )
        InitializeUpdate( object, attr );
    attr.ComputeHeatEvolution( timeOfCreation_, time );
	int heat = UpdateState( object, attr, time );
	if ( heat > 0 && heat * (int)timeSinceCreation > attr.GetClass().GetPropagationThreshold() )
		Propagate( object );
}

void FirePropagationCapacity::InitializeUpdate( Object& object, const FireAttribute& attr )
{
    // We get the coordinates of the fire and adjust them in order that they become divisible by length and width of the fire 
    MT_Vector2D vOrigin( object.GetLocalisation().ComputeBarycenter() );
    vOrigin.rX_ = (int)vOrigin.rX_ - (int)vOrigin.rX_ % attr.GetLength();
    vOrigin.rY_ = (int)vOrigin.rY_ - (int)vOrigin.rY_ % attr.GetWidth();
    
    //We define the origin vector of the fire
    object.UpdateLocalisation( GetLocalisation( vOrigin ) );
    pManager_->Flag( vOrigin , attr.GetLength() , attr.GetWidth() );
    needUpdate_ = false;
}

// -----------------------------------------------------------------------------
// Name: MIL_DynamicFire::UpdateState()
// Created: RFT 24/04/2008
// Modified: RFT 15/05/2008
// -----------------------------------------------------------------------------
int FirePropagationCapacity::UpdateState( Object& object, const FireAttribute& attr, unsigned int time )
{
	//Set the time of death of the fire as soon as its temperature is below
    //Used to block the propagation of the fire on a fire which just died one tick ago
    if( attr.GetHeat() >= 0 )
		timeOfDeath_ = 0;
	else
	{
		if( timeOfDeath_ == 0 )
		{
			object.MarkForDestruction();
			//A reflechir
			timeOfDeath_ = time + 10;
		}
		if ( time >= timeOfDeath_ )
			pManager_->RemoveFlag( object.GetLocalisation().ComputeBarycenter() , attr.GetLength() , attr.GetWidth() );
	}
	return attr.GetHeat();
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation& ComputeNewDynamicFireLocalisation( localisation, wind )
// Created: RFT 06/05/2008
// Modified: RFT 15/05/2008
// -----------------------------------------------------------------------------
void FirePropagationCapacity::Propagate( Object& object )
{
    FireAttribute& attr = object.GetAttribute< FireAttribute >();
	MT_Vector2D vOrigin( object.GetLocalisation().ComputeBarycenter() );
    MT_Vector2D vPerpendicularToWind, vNormalizedWind;

    const PHY_Meteo::sWindData& wind = MIL_Tools::GetWind( vOrigin );

    //we normalize the wind vector and multiply it by length and width of the fire 
    vNormalizedWind.rX_ = attr.GetLength() * wind.vWindDirection_.rX_ / pow(wind.vWindDirection_.rY_ * wind.vWindDirection_.rY_ + wind.vWindDirection_.rX_ * wind.vWindDirection_.rX_ , 0.5);
    vNormalizedWind.rY_ = attr.GetWidth() * wind.vWindDirection_.rY_ / pow(wind.vWindDirection_.rY_ * wind.vWindDirection_.rY_ + wind.vWindDirection_.rX_ * wind.vWindDirection_.rX_ , 0.5);

    vNormalizedWind.rX_ = vNormalizedWind.rX_ - (int)vNormalizedWind.rX_ % attr.GetLength();
    vNormalizedWind.rY_ = vNormalizedWind.rY_ - (int)vNormalizedWind.rY_ % attr.GetWidth();

    vPerpendicularToWind.rX_ = -vNormalizedWind.rY_;
    vPerpendicularToWind.rY_ = vNormalizedWind.rX_;
    
    //Propagation to three out of four adjacent positions.
	CheckPropagation( vOrigin + vNormalizedWind, object );
	CheckPropagation( vOrigin + vPerpendicularToWind, object );
	CheckPropagation( vOrigin - vPerpendicularToWind, object );
	//Propagation taking in account wind speed and the length of the fire
    for( int i = 2; i < wind.rWindSpeed_ / attr.GetLength() ; i++ )
		CheckPropagation( vOrigin - i * vNormalizedWind, object );
}

namespace
{
	class MIL_FireBuilder : public MIL_ObjectBuilder_ABC
    {
    public:
        MIL_FireBuilder( const Object& object, const TER_Localisation& location ) 
            : object_ ( object ) 
			, location_ ( location )
        {
        }

        const MIL_ObjectType_ABC& GetType() const 
        {
            return object_.GetType();
        }

        virtual void Build( Object& object ) const
        {            
            object.Initialize( location_ );
            object.GetAttribute< FireAttribute >() = FireAttribute( object_.GetAttribute< FireAttribute >() );
            object.Attach< BurnCapacity >( *new BurnCapacity() );
        }

    private:
        const Object&           object_;
        const TER_Localisation&	location_;
    };
}

// -----------------------------------------------------------------------------
// Name: void FirePropagationCapacity::CheckPropagation( vOrigin, object )
// Created: RFT 06/05/2008
// Modified: RFT 15/05/2008
// -----------------------------------------------------------------------------
void FirePropagationCapacity::CheckPropagation( const MT_Vector2D& vOrigin, Object& object )
{
    FireAttribute& attr = object.GetAttribute< FireAttribute >();
	TER_Localisation location( GetLocalisation( vOrigin ) );

	if( !pManager_->IsFlagged( location , attr.GetLength() , attr.GetWidth() ) ) 
	{
		MIL_FireBuilder builder( object, location );
		MIL_EntityManager::GetSingleton().CreateObject( object.GetArmy(), builder );
        pManager_->Flag( vOrigin , attr.GetLength() , attr.GetWidth() );
	}
}

// -----------------------------------------------------------------------------
// Name: void FirePropagationCapacity::CheckPropagation( vOrigin, object )
// Created: RFT 06/05/2008
// Modified: RFT 15/05/2008
// -----------------------------------------------------------------------------
TER_Localisation FirePropagationCapacity::GetLocalisation( const MT_Vector2D& pt ) const
{
	T_PointVector pointVector;
    pointVector.push_back( pt );
	return TER_Localisation( TER_Localisation::ePoint, pointVector );
}
