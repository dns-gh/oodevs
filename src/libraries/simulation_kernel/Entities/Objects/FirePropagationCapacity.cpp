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
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( FirePropagationCapacity, "FirePropagationCapacity" )

// -----------------------------------------------------------------------------
// Name: FirePropagationCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
FirePropagationCapacity::FirePropagationCapacity( xml::xistream& xis, MIL_PropagationManager& manager )
	: pManager_ ( &manager )
	, timeOfCreation_ ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
	, timeOfDeath_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePropagationCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
FirePropagationCapacity::FirePropagationCapacity()
	: pManager_ ( 0 )
	, timeOfCreation_ ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
	, timeOfDeath_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePropagationCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
FirePropagationCapacity::FirePropagationCapacity( const FirePropagationCapacity& from )
    : pManager_ ( from.pManager_ )
	, timeOfCreation_ ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
	, timeOfDeath_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePropagationCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
FirePropagationCapacity::~FirePropagationCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtinguishableCapacity::serialize
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void FirePropagationCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );        
    file & timeOfCreation_
	     & timeOfDeath_;
}

// -----------------------------------------------------------------------------
// Name: FirePropagationCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void FirePropagationCapacity::Register( Object& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: FirePropagationCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void FirePropagationCapacity::Instanciate( Object& object ) const
{    
    FirePropagationCapacity* capacity = new FirePropagationCapacity( *this );
    object.AddCapacity< PropagationCapacity_ABC >( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );

	//We get the coordinates of the fire and adjust them in order that they become divisible by 5
	MT_Vector2D vOrigin( object.GetLocalisation().ComputeBarycenter() );
	vOrigin.rX_ = (int)vOrigin.rX_ - (int)vOrigin.rX_ % 5;
    vOrigin.rY_ = (int)vOrigin.rY_ - (int)vOrigin.rY_ % 5;
    
	//We define the origin vector of the fire
	object.UpdateLocalisation( GetLocalisation( vOrigin ) );
	pManager_->Flag( vOrigin );
}

// -----------------------------------------------------------------------------
// Name: FirePropagationCapacity::Update
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
void FirePropagationCapacity::Update( Object& object, uint time )
{	
    static const uint threshold	= 10000; // to determine in order to control propagation
	const uint timeSinceCreation = time - timeOfCreation_;	
	
	int heat = UpdateState( object, time );
	if ( heat > 0 && heat * timeSinceCreation > threshold )
		Propagate( object );
}

// -----------------------------------------------------------------------------
// Name: MIL_DynamicFire::UpdateShape()
// Created: RFT 24/04/2008
// Modified: RFT 15/05/2008
// -----------------------------------------------------------------------------
int FirePropagationCapacity::UpdateState( Object& object, uint time )
{
	FireAttribute& attr = object.GetAttribute< FireAttribute >();

	attr.ComputeHeatEvolution( timeOfCreation_, time );
	int heat = attr.GetHeat();
	
	//Set the time of death of the fire as soon as its temperature is below
    //Used to block the propagation of the fire on a fire which just died one tick ago
    if( heat >= 0 )
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
			pManager_->RemoveFlag( object.GetLocalisation().ComputeBarycenter() );		
	}
	return heat;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation& ComputeNewDynamicFireLocalisation( localisation, wind )
// Created: RFT 06/05/2008
// Modified: RFT 15/05/2008
// -----------------------------------------------------------------------------
void FirePropagationCapacity::Propagate( Object& object )
{
	MT_Vector2D vOrigin( object.GetLocalisation().ComputeBarycenter() );
    MT_Vector2D vPerpendicularToWind, vNormalizedWind;

    const PHY_Meteo::sWindData& wind = MIL_Tools::GetWind( vOrigin );

    //we normalize the wind vector and multiply it by 5 
    vNormalizedWind.rX_ = 5*wind.vWindDirection_.rX_/pow(wind.vWindDirection_.rY_*wind.vWindDirection_.rY_ + wind.vWindDirection_.rX_*wind.vWindDirection_.rX_, 0.5);
    vNormalizedWind.rY_ = 5*wind.vWindDirection_.rY_/pow(wind.vWindDirection_.rY_*wind.vWindDirection_.rY_ + wind.vWindDirection_.rX_*wind.vWindDirection_.rX_, 0.5);

    vNormalizedWind.rX_ = vNormalizedWind.rX_ - (int)vNormalizedWind.rX_%5;
    vNormalizedWind.rY_ = vNormalizedWind.rY_ - (int)vNormalizedWind.rY_%5;

    vPerpendicularToWind.rX_ = -vNormalizedWind.rY_;
    vPerpendicularToWind.rY_ = vNormalizedWind.rX_;
    
    //Propagation to three out of four adjacent positions.
	CheckPropagation( vOrigin + vNormalizedWind, object );
	CheckPropagation( vOrigin + vPerpendicularToWind, object );
	CheckPropagation( vOrigin - vPerpendicularToWind, object );
	//Propagation taking in account wind speed
    for( int i = 2; i < wind.rWindSpeed_ / 5; i++ )
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
	TER_Localisation location( GetLocalisation( vOrigin ) );

	if( !pManager_->IsFlagged( location ) ) 
	{
		MIL_FireBuilder builder( object, location );
		MIL_EntityManager::GetSingleton().CreateObject( object.GetArmy(), builder );
		pManager_->Flag( vOrigin );
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
