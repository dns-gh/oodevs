// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "NBCPropagationCapacity.h"
#include "MIL_Object_ABC.h"
#include "NBCTypeAttribute.h"
#include "MIL_ObjectBuilder_ABC.h"
#include "ContaminationCapacity.h"
#include "MIL_PropagationManager.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Objects/MIL_ObjectLoader.h"
#include "Entities/Objects/CapacityFactory.h"
#include "MT_Tools/MT_Logger.h"
#include "Tools/MIL_Tools.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_IMPLEMENT( NBCPropagationCapacity )

// -----------------------------------------------------------------------------
// Name: NBCPropagationCapacity constructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
NBCPropagationCapacity::NBCPropagationCapacity( MIL_PropagationManager& manager )
    : pManager_        ( &manager )
    , timeOfLastUpdate_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCPropagationCapacity constructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
NBCPropagationCapacity::NBCPropagationCapacity()
: pManager_( &MIL_AgentServer::GetWorkspace().GetPropagationManager() )
, timeOfLastUpdate_( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: NBCPropagationCapacity constructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
NBCPropagationCapacity::NBCPropagationCapacity( const NBCPropagationCapacity& from )
    : pManager_        ( from.pManager_ )
    , timeOfLastUpdate_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCPropagationCapacity destructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
NBCPropagationCapacity::~NBCPropagationCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtinguishableCapacity::serialize
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void NBCPropagationCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: NBCPropagationCapacity::Register
// Created: RFT 2008-07-03
// -----------------------------------------------------------------------------
void NBCPropagationCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: NBCPropagationCapacity::Instanciate
// Created: RFT 2008-06-08
// -----------------------------------------------------------------------------
void NBCPropagationCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    NBCTypeAttribute& attr           = object.GetAttribute< NBCTypeAttribute >();
    NBCPropagationCapacity* capacity = new NBCPropagationCapacity( *this );
    object.AddCapacity< PropagationCapacity_ABC >( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );

    //We get the coordinates of the nbc cloud and adjust them in order that they become divisible by length and width of the fire
    MT_Vector2D vOrigin( object.GetLocalisation().ComputeBarycenter() );
    vOrigin.rX_ = (int)vOrigin.rX_ - (int)vOrigin.rX_ % attr.GetLength();
    vOrigin.rY_ = (int)vOrigin.rY_ - (int)vOrigin.rY_ % attr.GetWidth();

    //We define the origin vector of the nbc cloud
    object.UpdateLocalisation( GetLocalisation( vOrigin ) );
    pManager_->Flag( vOrigin , attr.GetLength() , attr.GetWidth() );
}

// -----------------------------------------------------------------------------
// Name: NBCPropagationCapacity::Update
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
void NBCPropagationCapacity::Update( MIL_Object_ABC& object, float time )
{
    NBCTypeAttribute& attr = object.GetAttribute< NBCTypeAttribute >();
    MT_Vector2D vOrigin( object.GetLocalisation().ComputeBarycenter() );

    //We retrieve the wind data
    const weather::Meteo::sWindData& wind = MIL_Tools::GetWind( vOrigin );
    MT_Vector2D vNormalizedWind , vPerpendicularToWind;

    //The normalized vector which indicates the wind direction
    const double sqrRx = pow( wind.vDirection_.rX_ , 2 );
    const double sqrRy = pow( wind.vDirection_.rY_ , 2 );
    vNormalizedWind.rX_ = attr.GetLength() * wind.vDirection_.rX_ / pow( sqrRy + sqrRx, 0.5);
    vNormalizedWind.rY_ = attr.GetWidth() * wind.vDirection_.rY_ / pow( sqrRy + sqrRx, 0.5);

    vNormalizedWind.rX_ = vNormalizedWind.rX_ - (int)vNormalizedWind.rX_ % attr.GetLength();
    vNormalizedWind.rY_ = vNormalizedWind.rY_ - (int)vNormalizedWind.rY_ % attr.GetWidth();

    //The normalized vector which indicates the wind perpendicular
    vPerpendicularToWind.rX_ = -vNormalizedWind.rY_;
    vPerpendicularToWind.rY_ = vNormalizedWind.rX_;

    //Propagate the nbc agents in accordance with wind data
    UpdateShape( object , vNormalizedWind , vPerpendicularToWind , wind.rSpeed_ );

    //Compute the nbc agents concentration in accordance with wind data
    if( ! attr.IsSource() )
    {
        bool bHasASource = UpdateState( object , vNormalizedWind , vPerpendicularToWind , wind.rSpeed_ );
        attr.ComputeAgentConcentrationEvolution( bHasASource );
    }
    else
        attr.UpdateSourceLifeDuration( time , timeOfLastUpdate_ );

    timeOfLastUpdate_ = time;
}

// -----------------------------------------------------------------------------
// Name: NBCPropagationCapacity::UpdateShape
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
void NBCPropagationCapacity::UpdateShape( MIL_Object_ABC& object, const MT_Vector2D& vNormalizedWind, const MT_Vector2D& vPerpendicularToWind, double windSpeed )
{
    NBCTypeAttribute& attr = object.GetAttribute< NBCTypeAttribute >();
    MT_Vector2D vOrigin( object.GetLocalisation().ComputeBarycenter() );
    double seq = windSpeed / attr.GetLength();
    for( int i = 0 ; i <= seq; i ++ )
    {
        for( int j = - ( int )( attr.GetPropagationAngle() / 2 ) ; j <= ( int )( attr.GetPropagationAngle() / 2 ) ; j++ )
        {
            Propagate( vOrigin + i * vNormalizedWind + j * vPerpendicularToWind , object );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: NBCPropagationCapacity::UpdateState
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
bool NBCPropagationCapacity::UpdateState( MIL_Object_ABC& object, const MT_Vector2D& vNormalizedWind, const MT_Vector2D& vPerpendicularToWind, double windSpeed )
{
    NBCTypeAttribute& attr = object.GetAttribute< NBCTypeAttribute >();
    MT_Vector2D vOrigin( object.GetLocalisation().ComputeBarycenter() );
    double seq = windSpeed / attr.GetLength();
    for( int i = 0 ; i <= seq; i ++ )
    {
        for( int j = -( int )( attr.GetPropagationAngle()/2 ) ; j <= ( int )( attr.GetPropagationAngle()/2 ) ; j++ )
        {
            if( pManager_->IsFlagged( GetLocalisation( vOrigin - i * vNormalizedWind + j * vPerpendicularToWind ) , attr.GetLength() , attr.GetWidth() ) )
                return true;
        }
    }
    return false;
}

namespace
{
    class MIL_NBCBuilder : public MIL_ObjectBuilder_ABC
    {
    public:
        MIL_NBCBuilder( const MIL_Object_ABC& object, const TER_Localisation& location )
            : object_ ( object )
            , location_ ( location )
        {
        }

        const MIL_ObjectType_ABC& GetType() const
        {
            return object_.GetType();
        }

        virtual void Build( MIL_Object_ABC& object ) const
        {
            object.Initialize( location_ );
            object.GetAttribute< NBCTypeAttribute >() = NBCTypeAttribute( object_.GetAttribute< NBCTypeAttribute >() );
            object.Attach< ContaminationCapacity >( *new ContaminationCapacity() );
        }

    private:
        const MIL_Object_ABC&           object_;
        const TER_Localisation&    location_;
    };
}

// -----------------------------------------------------------------------------
// Name: void NBCPropagationCapacity::Propagate
// Created: RFT 06/05/2008
// Modified: RFT 15/05/2008
// -----------------------------------------------------------------------------
void NBCPropagationCapacity::Propagate( const MT_Vector2D& vOrigin, MIL_Object_ABC& object )
{
    NBCTypeAttribute& attr = object.GetAttribute< NBCTypeAttribute >();
    TER_Localisation location( GetLocalisation( vOrigin ) );
    if( !pManager_->IsFlagged( location , attr.GetLength() , attr.GetWidth() ) )
    {
        MIL_NBCBuilder builder( object, location );
        try
        {
            MIL_EntityManager::GetSingleton().CreateObject( *object.GetArmy(), builder );
        }
        catch( std::exception& e )
        {
            MT_LOG_ERROR_MSG( e.what() );
        }
        pManager_->Flag( vOrigin, attr.GetLength(), attr.GetWidth() );
        //Definir peut etre ici une concentration initiale
    }
}

// -----------------------------------------------------------------------------
// Name: void NBCPropagationCapacity::GetLocalisation
// Created: RFT 06/05/2008
// Modified: RFT 15/05/2008
// -----------------------------------------------------------------------------
TER_Localisation NBCPropagationCapacity::GetLocalisation( const MT_Vector2D& pt ) const
{
    T_PointVector pointVector;
    pointVector.push_back( pt );
    return TER_Localisation( TER_Localisation::ePoint, pointVector );
}
