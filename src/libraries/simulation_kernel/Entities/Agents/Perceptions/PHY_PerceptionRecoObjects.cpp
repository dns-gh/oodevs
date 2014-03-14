// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRecoObjects.cpp $
// $Author: Nld $
// $Modtime: 2/05/05 17:18 $
// $Revision: 4 $
// $Workfile: PHY_PerceptionRecoObjects.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionRecoObjects.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "Entities/Objects/MIL_Object_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoObjectsReco constructor
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
PHY_PerceptionRecoObjectsReco::PHY_PerceptionRecoObjectsReco( const TER_Localisation& localisation, const MT_Vector2D& vCenter, double rGrowthSpeed, DEC_Decision_ABC& callerAgent )
    : vCenter_( vCenter )
    , localisation_( localisation )
    , rCurrentSize_( 0 )
    , rGrowthSpeed_( rGrowthSpeed )
    , callerAgent_( callerAgent )
    , bMaxSizeDone_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoObjectsReco::UpdateLocalisation
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoObjectsReco::UpdateLocalisation()
{
    if( bMaxSizeDone_ )
        return;

    rCurrentSize_ += rGrowthSpeed_;

    // $$$$ JVT : Beark
    T_PointVector points;
    points.push_back( vCenter_ );
    points.push_back( vCenter_ + MT_Vector2D( rCurrentSize_, 0. ) );
    circle_.Reset( TER_Localisation::eCircle, points );

    const T_PointVector& pointLocalisationFinale = localisation_.GetPoints();
    bMaxSizeDone_ = true;
    for( auto it = pointLocalisationFinale.begin(); bMaxSizeDone_ && it != pointLocalisationFinale.end(); ++it )
        bMaxSizeDone_ = circle_.IsInside( *it );
    if( bMaxSizeDone_ )
        callerAgent_.CallbackPerception( Id() );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoObjectsReco::IsInside
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
bool PHY_PerceptionRecoObjectsReco::IsInside( const TER_Localisation& localisation ) const
{
    return localisation_.IsIntersecting( localisation ) && circle_.IsIntersecting( localisation );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoObjectsReco::GetObjectsInside
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoObjectsReco::GetObjectsInside( TER_Object_ABC::T_ObjectVector& result ) const
{
    TER_World::GetWorld().GetObjectManager().GetListWithinCircle( vCenter_, rCurrentSize_, result );
    for( auto it = result.begin(); it != result.end(); )
        if( localisation_.IsIntersecting( (*it)->GetLocalisation() ) )
            ++it;
        else
            it = result.erase( it );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoObjects::PHY_PerceptionRecoObjects
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRecoObjects::PHY_PerceptionRecoObjects( PHY_RoleInterface_Perceiver& perceiver )
    : PHY_PerceptionWithLocation< PHY_PerceptionRecoObjectsReco >( perceiver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoObjects::~PHY_PerceptionRecoObjects
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRecoObjects::~PHY_PerceptionRecoObjects()
{
    for( auto it = recos_.begin(); it != recos_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoObjects::AddLocalisation
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
int PHY_PerceptionRecoObjects::AddLocalisation( const TER_Localisation& localisation, const MT_Vector2D& vCenter, double rSpeed, DEC_Decision_ABC& callerAgent )
{
    return Add( new PHY_PerceptionRecoObjectsReco( localisation, vCenter, rSpeed, callerAgent ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoObjects::RemoveLocalisation
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoObjects::RemoveLocalisation( int id )
{
    Remove( id );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoObjects::Update
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoObjects::Update()
{
    for( auto it = recos_.begin(); it != recos_.end(); ++it )
        (*it)->UpdateLocalisation();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoObjects::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoObjects::Compute( const DEC_Knowledge_Object& knowledge ) const
{
    for( auto it = recos_.begin(); it != recos_.end(); ++it )
        if( (*it)->IsInside( knowledge.GetLocalisation() ) )
            return PHY_PerceptionLevel::recognized_;
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoObjects::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoObjects::Execute( const TER_Object_ABC::T_ObjectVector& /*perceivableObjects*/ )
{
    TER_Object_ABC::T_ObjectVector perceivableObjects;
    for( auto itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        perceivableObjects.clear();
        (*itReco)->GetObjectsInside( perceivableObjects );
        for( TER_Object_ABC::CIT_ObjectVector it = perceivableObjects.begin(); it != perceivableObjects.end(); ++it )
        {
            MIL_Object_ABC& object = static_cast< MIL_Object_ABC& >( **it );
            if( object().CanBePerceived() && perceiver_.CanPerceive( object.GetType() ) )
                perceiver_.NotifyPerception( object, PHY_PerceptionLevel::identified_ ); // Identifié ou not seen pour les objets
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoObjects::HasLocalisationToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
bool PHY_PerceptionRecoObjects::HasLocalisationToHandle() const
{
    return !recos_.empty();
}
