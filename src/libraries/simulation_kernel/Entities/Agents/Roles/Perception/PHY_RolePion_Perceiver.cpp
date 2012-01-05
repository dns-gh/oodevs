// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Perceiver.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Radars/PHY_RadarType.h"
#include "Entities/Agents/Units/Sensors/PHY_Sensor.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeObject.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionView.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionCoupDeSonde.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionRecoPoint.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionRecoLocalisation.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionRecoSurveillance.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionRecoObjects.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionRecoUrbanBlock.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionRadar.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionAlat.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionFlyingShell.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Entities/Orders/MIL_Report.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KS_Perception.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Network/NET_AgentServer.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "simulation_terrain/TER_Agent_ABC.h"
#include "simulation_terrain/TER_PopulationConcentration_ABC.h"
#include "simulation_terrain/TER_PopulationFlow_ABC.h"
#include "simulation_terrain/TER_PopulationManager.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/DetectionComputer_ABC.h"
#include "simulation_kernel/DetectionComputerFactory_ABC.h"
#include "simulation_kernel/Entities/Objects/MIL_Object_ABC.h"
#include "simulation_kernel/Entities/Populations/MIL_PopulationConcentration.h"
#include "simulation_kernel/Entities/Populations/MIL_PopulationFlow.h"
#include "simulation_kernel/PerceptionDistanceComputer_ABC.h"
#include "simulation_kernel/OnComponentFunctor_ABC.h"
#include "simulation_kernel/OnComponentFunctorComputer_ABC.h"
#include "simulation_kernel/OnComponentFunctorComputerFactory_ABC.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "simulation_kernel/VisionConeNotificationHandler_ABC.h"
#include "simulation_terrain/TER_AgentVisitor_ABC.h"
#include "simulation_terrain/TER_ObjectVisitor_ABC.h"
#include "simulation_terrain/TER_PopulationConcentrationVisitor_ABC.h"
#include "simulation_terrain/TER_PopulationFlowVisitor_ABC.h"
#include <urban/model.h>
#include <urban/TerrainObject_ABC.h>
#include <boost/serialization/map.hpp>

using namespace detection;

const unsigned int PHY_RolePion_Perceiver::nNbrStepsBetweenPeriphericalVision_ = 12; //$$$ En dur ...

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePion_Perceiver )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_Perceiver* role, const unsigned int /*version*/ )
{
    MIL_Agent_ABC* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePion_Perceiver* role, const unsigned int /*version*/ )
{
    MIL_Agent_ABC* pion;
    archive >> pion;
    ::new( role )PHY_RolePion_Perceiver( *pion, 0, 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver constructor
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
PHY_RolePion_Perceiver::PHY_RolePion_Perceiver( MIL_Agent_ABC& pion, const MT_Vector2D* perceiverPosition, const MT_Vector2D* perceiverDirection )
    : pion_                          ( pion )
    , perceiverPosition_             ( perceiverPosition )
    , perceiverDirection_            ( perceiverDirection )
    , rMaxAgentPerceptionDistance_   ( 0. )
    , rMaxObjectPerceptionDistance_  ( 0. )
    , bPeriphericalVisionEnabled_    ( false )
    , bRecordModeEnabled_            ( false )
    , vSensorInfo_                   ()
    , nSensorMode_                   ( eNormal )
    , bHasChanged_                   ( true )
    , bExternalMustChangePerception_ ( false )
    , bExternalMustChangeRadar_      ( false )
    , bExternalCanPerceive_          ( true )
    , bExternalMustUpdateVisionCones_( false )
    , bRadarStateHasChanged_         ( true )
    , pPerceptionCoupDeSonde_        ( 0 )
    , pPerceptionRecoPoint_          ( 0 )
    , pPerceptionRecoLocalisation_   ( 0 )
    , pPerceptionRecoUrbanBlock_     ( 0 )
    , pPerceptionRadar_              ( 0 )
    , pPerceptionAlat_               ( 0 )
    , pPerceptionSurveillance_       ( 0 )
    , pPerceptionRecoObjects_        ( 0 )
    , pPerceptionFlyingShell_        ( 0 )
{
    static unsigned int nNbr = 0;
    nNextPeriphericalVisionStep_ = ++nNbr % nNbrStepsBetweenPeriphericalVision_;
    pPerceptionView_ = new PHY_PerceptionView( *this, pion );
    activePerceptions_.push_back( pPerceptionView_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_RolePion_Perceiver::~PHY_RolePion_Perceiver()
{
    for( CIT_PerceptionVector it = activePerceptions_.begin(); it != activePerceptions_.end(); ++it )
        delete *it;
    activePerceptions_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver Initialization
// Created: MGD 2009-10-14
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::Initialization( const MT_Vector2D& perceiverPosition, const MT_Vector2D& perceiverDirection )
{
    perceiverPosition_ = &perceiverPosition;
    perceiverDirection_ = &perceiverDirection;
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        // =============================================================================
        // T_SurfaceAgentKeyPair
        // =============================================================================
        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_RolePion_Perceiver::T_SurfaceAgentKeyPair& pair, const unsigned int nVersion )
        {
            split_free( file, pair, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const PHY_RolePion_Perceiver::T_SurfaceAgentKeyPair& pair, const unsigned int )
        {
            assert( pair.first );
            unsigned id = pair.first->GetType().GetID();
            file << id
                 << pair.second;
        }

        template< typename Archive >
        void load( Archive& file, PHY_RolePion_Perceiver::T_SurfaceAgentKeyPair& pair, const unsigned int )
        {
            unsigned int nID;
            file >> nID;
            assert( PHY_SensorType::FindSensorType( nID ) );
            pair.first = PHY_SensorType::FindSensorType( nID )->GetTypeAgent();
            file >> pair.second;
        }

        // =============================================================================
        // T_SurfaceObjectKeyPair
        // =============================================================================
        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_RolePion_Perceiver::T_SurfaceObjectKeyPair& pair, const unsigned int nVersion )
        {
            split_free( file, pair, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const PHY_RolePion_Perceiver::T_SurfaceObjectKeyPair& pair, const unsigned int )
        {
            assert( pair.first );
            unsigned id = pair.first->GetType().GetID();
            file << id
                 << pair.second;
        }

        template< typename Archive >
        void load( Archive& file, PHY_RolePion_Perceiver::T_SurfaceObjectKeyPair& pair, const unsigned int )
        {
            unsigned int nID;

            file >> nID;
            assert( PHY_SensorType::FindSensorType( nID ) );
            pair.first = PHY_SensorType::FindSensorType( nID )->GetTypeObject();

            file >> pair.second;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Perceiver::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Perceiver >( *this )
         & bPeriphericalVisionEnabled_
         & nNextPeriphericalVisionStep_
         & surfacesAgent_
         & surfacesObject_
         & rMaxAgentPerceptionDistance_
         & rMaxObjectPerceptionDistance_;
}

// =============================================================================
// PERCEPTIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableSensors
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::EnableSensors()
{
    if( pPerceptionView_ )
        pPerceptionView_->Enable();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableSensors
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableSensors()
{
    if( pPerceptionView_ )
        pPerceptionView_->Disable();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableCoupDeSonde
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::EnableCoupDeSonde()
{
    if( pPerceptionCoupDeSonde_ )
        return;
    pPerceptionCoupDeSonde_ = new PHY_PerceptionCoupDeSonde( *this );
    activePerceptions_.push_back( pPerceptionCoupDeSonde_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableCoupDeSonde
// Created: NLD 2004-10-14
// Modified: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableCoupDeSonde()
{
    if( !pPerceptionCoupDeSonde_ )
        return;
    activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionCoupDeSonde_ ) );
    delete pPerceptionCoupDeSonde_;
    pPerceptionCoupDeSonde_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRecoAlat
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::EnableRecoAlat( const TER_Localisation& localisation )
{
    if( pPerceptionAlat_ )
        return;
    pPerceptionAlat_ = new PHY_PerceptionAlat( *this, localisation );
    activePerceptions_.push_back( pPerceptionAlat_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRecoAlat
// Created: NLD 2004-10-14
// Modified: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRecoAlat()
{
    if( !pPerceptionAlat_ )
        return;
    activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionAlat_ ) );
    delete pPerceptionAlat_;
    pPerceptionAlat_ = 0;
    pion_.GetKnowledge().GetKsPerception().MakePerceptionsAvailableTimed();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableSurveillanceLocalisation
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableSurveillanceLocalisation( const TER_Localisation& localisation )
{
    if( !pPerceptionSurveillance_ )
    {
        pPerceptionSurveillance_ = new PHY_PerceptionRecoSurveillance( *this );
        activePerceptions_.push_back( pPerceptionSurveillance_ );
    }
    return pPerceptionSurveillance_->AddLocalisation( localisation );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableSurveillanceLocalisation
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableSurveillanceLocalisation( int id )
{
    if( !pPerceptionSurveillance_ )
        return;
    pPerceptionSurveillance_->RemoveLocalisation( id );
    if( !pPerceptionSurveillance_->HasLocalisationToHandle() )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionSurveillance_ ) );
        delete pPerceptionSurveillance_;
        pPerceptionSurveillance_ = 0;
    }
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRecoPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableRecoPoint( const MT_Vector2D& center, double rSize, double rSpeed, DEC_Decision_ABC& callerAgent )
{
    if( !pPerceptionRecoPoint_ )
    {
        pPerceptionRecoPoint_ = new PHY_PerceptionRecoPoint( *this );
        activePerceptions_.push_back( pPerceptionRecoPoint_ );
    }
    return pPerceptionRecoPoint_->AddPoint( center, rSize, rSpeed, callerAgent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRecoPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRecoPoint( int id )
{
    if( !pPerceptionRecoPoint_ )
        return;
    pPerceptionRecoPoint_->RemovePoint( id );
    if( !pPerceptionRecoPoint_->HasPointToHandle() )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionRecoPoint_ ) );
        delete pPerceptionRecoPoint_;
        pPerceptionRecoPoint_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRecoObjects
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableRecoObjects( const TER_Localisation& localisation, const MT_Vector2D& vCenter, double rSpeed, DEC_Decision_ABC& callerAgent )
{
    if( !pPerceptionRecoObjects_ )
    {
        pPerceptionRecoObjects_ = new PHY_PerceptionRecoObjects( *this );
        activePerceptions_.push_back( pPerceptionRecoObjects_ );
    }
    return  pPerceptionRecoObjects_->AddLocalisation( localisation, vCenter, rSpeed, callerAgent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRecoObjects
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRecoObjects( int id )
{
    if( !pPerceptionRecoObjects_ )
        return;
    pPerceptionRecoObjects_->RemoveLocalisation( id );
    if( !pPerceptionRecoObjects_->HasLocalisationToHandle() )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionRecoObjects_ ) );
        delete pPerceptionRecoObjects_;
        pPerceptionRecoObjects_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnsurePerceptionRecoLocalisation
// Created: LDC 2009-12-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::EnsurePerceptionRecoLocalisation()
{
    if( !pPerceptionRecoLocalisation_ )
    {
        pPerceptionRecoLocalisation_ = new PHY_PerceptionRecoLocalisation( *this );
        activePerceptions_.push_back( pPerceptionRecoLocalisation_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRecoLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableRecoLocalisation( const TER_Localisation& localisation, float rGrowthSpeed, DEC_Decision_ABC& callerAgent )
{
    EnsurePerceptionRecoLocalisation();
    return pPerceptionRecoLocalisation_->AddLocalisationWithGrowthSpeed( localisation, rGrowthSpeed, callerAgent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRecoLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableRecoLocalisation( const TER_Localisation& localisation, DEC_Decision_ABC& callerAgent )
{
    EnsurePerceptionRecoLocalisation();
    return pPerceptionRecoLocalisation_->AddLocalisationWithDefaultGrowthSpeed( localisation, callerAgent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRecoLocalisation
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableRecoUrbanBlock( UrbanObjectWrapper* pUrbanBlock )
{
    if( !pPerceptionRecoUrbanBlock_ )
    {
        pPerceptionRecoUrbanBlock_ = new PHY_PerceptionRecoUrbanBlock( *this );
        activePerceptions_.push_back( pPerceptionRecoUrbanBlock_ );
    }
    return pPerceptionRecoUrbanBlock_->AddUrbanBlock( pUrbanBlock );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableControlLocalisation
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableControlLocalisation( const TER_Localisation& localisation, DEC_Decision_ABC& callerAgent )
{
    EnsurePerceptionRecoLocalisation();
    return pPerceptionRecoLocalisation_->AddLocalisationWithDefaultGrowthSpeed( localisation, callerAgent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRecoLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRecoLocalisation( int id )
{
    if( !pPerceptionRecoLocalisation_ )
        return;
    pPerceptionRecoLocalisation_->RemoveLocalisation( id );
    if( !pPerceptionRecoLocalisation_->HasLocalisationToHandle() )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionRecoLocalisation_ ) );
        delete pPerceptionRecoLocalisation_;
        pPerceptionRecoLocalisation_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRecoUrbanBlock
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRecoUrbanBlock( int id )
{
    if( !pPerceptionRecoUrbanBlock_ )
        return;
    pPerceptionRecoUrbanBlock_->RemoveUrbanBlock( id );
    if( !pPerceptionRecoUrbanBlock_->HasLocalisationToHandle() )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionRecoUrbanBlock_ ) );
        delete pPerceptionRecoUrbanBlock_;
        pPerceptionRecoUrbanBlock_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsUsingActiveRadar
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsUsingActiveRadar() const
{
    return ( pPerceptionRadar_ && pPerceptionRadar_->IsUsingActiveRadar() ) || pPerceptionFlyingShell_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsUsingActiveRadar
// Created: JSR 2010-03-18
// LTO
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsUsingActiveRadar( const PHY_RadarClass& radarClass ) const
{
    return ( pPerceptionRadar_ && pPerceptionRadar_->IsUsingActiveRadar( radarClass ) ) || pPerceptionFlyingShell_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRadarOnLocalisation
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableRadarOnLocalisation( const PHY_RadarClass& radarClass, const TER_Localisation& localisation )
{
    if( !pPerceptionRadar_ )
    {
        pPerceptionRadar_ = new PHY_PerceptionRadar( *this );
        activePerceptions_.push_back( pPerceptionRadar_ );
        bRadarStateHasChanged_ = true;
    }
    return pPerceptionRadar_->EnableRadar( radarClass, localisation );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRadarOnLocalisation
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRadarOnLocalisation( const PHY_RadarClass& radarClass, int id )
{
   if( !pPerceptionRadar_ )
        return;
    pPerceptionRadar_->DisableRadar( radarClass, id );
    if( !pPerceptionRadar_->HasRadarToHandle() )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionRadar_ ) );
        delete pPerceptionRadar_;
        pPerceptionRadar_ = 0;
        bRadarStateHasChanged_ = true;
    }
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRadar
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::EnableRadar( const PHY_RadarClass& radarClass )
{
    if( !pPerceptionRadar_ )
    {
        pPerceptionRadar_ = new PHY_PerceptionRadar( *this );
        activePerceptions_.push_back( pPerceptionRadar_ );
        bRadarStateHasChanged_ = true;
    }
    return pPerceptionRadar_->EnableRadar( radarClass );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRadar
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRadar( const PHY_RadarClass& radarClass )
{
    if( !pPerceptionRadar_ )
        return;
    pPerceptionRadar_->DisableRadar( radarClass );
    if( !pPerceptionRadar_->HasRadarToHandle() )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionRadar_ ) );
        delete pPerceptionRadar_;
        pPerceptionRadar_      = 0;
        bRadarStateHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableFlyingShellDetection
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableFlyingShellDetection( const TER_Localisation& localisation )
{
    if( !pPerceptionFlyingShell_ )
    {
        pPerceptionFlyingShell_ = new PHY_PerceptionFlyingShell( *this );
        activePerceptions_.push_back( pPerceptionFlyingShell_ );
        bRadarStateHasChanged_ = true;
    }
    return pPerceptionFlyingShell_->AddLocalisation( localisation );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableFlyingShellDetection
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableFlyingShellDetection( int id )
{
    if( !pPerceptionFlyingShell_ )
        return;
    pPerceptionFlyingShell_->RemoveLocalisation( id );
    if( !pPerceptionFlyingShell_->HasLocalisationToHandle() )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionFlyingShell_ ) );
        delete pPerceptionFlyingShell_;
        pPerceptionFlyingShell_ = 0;
        bRadarStateHasChanged_ = true;
    }
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetMaxAgentPerceptionDistance
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
double PHY_RolePion_Perceiver::GetMaxAgentPerceptionDistance() const
{
    std::auto_ptr< PerceptionDistanceComputer_ABC > computer( pion_.GetAlgorithms().detectionComputerFactory_->CreateDistanceComputer() );
    return rMaxAgentPerceptionDistance_ * pion_.Execute( *computer ).GetFactor();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::PHY_RolePion_Perceiver::GetMaxTheoreticalcAgentPerceptionDistance
// Created: MMC 2011-05-11
// -----------------------------------------------------------------------------
double PHY_RolePion_Perceiver::GetMaxTheoreticalcAgentPerceptionDistance() const
{
    return rMaxAgentPerceptionDistance_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetMaxObjectPerceptionDistance
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
double PHY_RolePion_Perceiver::GetMaxObjectPerceptionDistance() const
{
    std::auto_ptr< PerceptionDistanceComputer_ABC > computer( pion_.GetAlgorithms().detectionComputerFactory_->CreateDistanceComputer() );
    return rMaxObjectPerceptionDistance_ * pion_.Execute( *computer ).GetFactor();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::CanPerceive
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::CanPerceive() const
{
    return !pion_.IsDead() && !pion_.GetRole< PHY_RoleAction_MovingUnderground >().IsUnderground() && bExternalCanPerceive_;
}

// =============================================================================
// UPDATE
// =============================================================================
class sPerceptionRotation : public OnComponentFunctor_ABC
{
public:
    sPerceptionRotation()
        : rAngle_ ( std::numeric_limits< double >::max() )
    {
        // NOTHING
    }

    void operator()( PHY_ComposantePion& composante )//@TODO MGD See to add const
    {
        rAngle_ = std::min( rAngle_, composante.GetType().GetSensorRotationAngle() );
    }

    double GetAngle() const
    {
        return rAngle_ == std::numeric_limits< double >::max() ? 0. : rAngle_;
    }

private:
    double rAngle_;
};

// -----------------------------------------------------------------------------
struct sPerceptionDataSensors : private boost::noncopyable
{
public:
    sPerceptionDataSensors( PHY_RolePion_Perceiver::T_SurfaceAgentMap& surfacesAgent, PHY_RolePion_Perceiver::T_SurfaceObjectMap& surfacesObject, const MT_Vector2D& vOrigin, const MT_Vector2D& vDirection, double& rMaxAgentPerceptionDistance, double& rMaxObjectPerceptionDistance )
        : surfacesAgent_               ( surfacesAgent )
        , surfacesObject_              ( surfacesObject )
        , vOrigin_                     ( vOrigin )
        , vDirection_                  ( vDirection )
        , rMaxAgentPerceptionDistance_ ( rMaxAgentPerceptionDistance )
        , rMaxObjectPerceptionDistance_( rMaxObjectPerceptionDistance )
    {
        // NOTHING
    }

    void operator()( const PHY_Sensor& sensor )
    {
        const PHY_SensorTypeAgent* pSensorTypeAgent = sensor.GetType().GetTypeAgent();
        if( pSensorTypeAgent )
        {
            rMaxAgentPerceptionDistance_ = std::max( rMaxAgentPerceptionDistance_, pSensorTypeAgent->GetMaxDistance() );
            PHY_PerceptionSurfaceAgent& surface = surfacesAgent_[ std::make_pair( pSensorTypeAgent, sensor.GetHeight() ) ];
            if( !surface.IsInitialized() )
                surface = PHY_PerceptionSurfaceAgent( *pSensorTypeAgent, vOrigin_, sensor.GetHeight() );
            surface.AddDirection( vDirection_ );
        }
        const PHY_SensorTypeObject* pSensorTypeObject = sensor.GetType().GetTypeObject();
        if( pSensorTypeObject )
        {
            rMaxObjectPerceptionDistance_ = std::max( rMaxObjectPerceptionDistance_, pSensorTypeObject->GetMaxDistance() );
            PHY_PerceptionSurfaceObject& surface = surfacesObject_[ std::make_pair( pSensorTypeObject, sensor.GetHeight() ) ];
            if( !surface.IsInitialized() )
                surface = PHY_PerceptionSurfaceObject( *pSensorTypeObject, vOrigin_, sensor.GetHeight() );
        }
    }

private:
    PHY_RolePion_Perceiver::T_SurfaceAgentMap&  surfacesAgent_;
    PHY_RolePion_Perceiver::T_SurfaceObjectMap& surfacesObject_;
    const MT_Vector2D&                          vOrigin_;
    const MT_Vector2D&                          vDirection_;
    double&                                   rMaxAgentPerceptionDistance_;
    double&                                   rMaxObjectPerceptionDistance_;
};

// -----------------------------------------------------------------------------
class sPerceptionDataComposantes : public OnComponentFunctor_ABC
{
public:
    sPerceptionDataComposantes( PHY_RolePion_Perceiver::T_SurfaceAgentMap& surfacesAgent, PHY_RolePion_Perceiver::T_SurfaceObjectMap& surfacesObject,
                                const MT_Vector2D& position, const MT_Vector2D& direction, const MT_Vector2D& vMainPerceptionDirection, double rDirectionRotation,
                                double& rMaxAgentPerceptionDistance, double& rMaxObjectPerceptionDistance )
        : surfacesAgent_               ( surfacesAgent )
        , surfacesObject_              ( surfacesObject )
        , position_                    ( position )
        , direction_                   ( direction )
        , vMainPerceptionDirection_    ( vMainPerceptionDirection )
        , nRotationIdx_                ( vMainPerceptionDirection != direction ? -1 : 0 ) // détection lockée
        , rRotationAngle_              ( rDirectionRotation )
        , rMaxAgentPerceptionDistance_ ( rMaxAgentPerceptionDistance )
        , rMaxObjectPerceptionDistance_( rMaxObjectPerceptionDistance )
    {
        // NOTHING
    }

    void operator()( PHY_ComposantePion& composante )
    {
        if( !composante.CanPerceive() )
            return;
        MT_Vector2D vComposantePerceptionDirection( vMainPerceptionDirection_ );
        if( nRotationIdx_ == 0 )
            vComposantePerceptionDirection = direction_;
        else if( nRotationIdx_ > 0 )
            vComposantePerceptionDirection.Rotate( ( ( 2 * ( nRotationIdx_ & 0x1 ) - 1 ) * ( ( nRotationIdx_ + 1 ) >> 1 ) ) * rRotationAngle_ );
        ++nRotationIdx_;
        sPerceptionDataSensors dataFunctor( surfacesAgent_, surfacesObject_, position_, vComposantePerceptionDirection, rMaxAgentPerceptionDistance_, rMaxObjectPerceptionDistance_ );
        composante.ApplyOnSensors( dataFunctor );
    }

private:
    PHY_RolePion_Perceiver::T_SurfaceAgentMap&  surfacesAgent_;
    PHY_RolePion_Perceiver::T_SurfaceObjectMap& surfacesObject_;
    const MT_Vector2D&                          position_;
    const MT_Vector2D&                          direction_;
    const MT_Vector2D&                          vMainPerceptionDirection_;
    double&                                   rMaxAgentPerceptionDistance_;
    double&                                   rMaxObjectPerceptionDistance_;
    int                                         nRotationIdx_;
    const double                              rRotationAngle_;
};


// -----------------------------------------------------------------------------
class sRadarDataComposantes : public OnComponentFunctor_ABC
{

public:
    sRadarDataComposantes( PHY_RolePion_Perceiver::T_RadarsPerClassMap& radars )
        : radars_( radars )
    {
        // NOTHING
    }

    void operator()( PHY_ComposantePion& composante )//@TODO MGD Use same design for weaponAvailability
    {
        if( !composante.CanPerceive() )
            return;
        composante.ApplyOnRadars( *this );
    }

    void operator()( const PHY_RadarType& radarType )
    {
        radars_[ &radarType.GetClass() ].insert( &radarType );
    }

private:
    PHY_RolePion_Perceiver::T_RadarsPerClassMap& radars_;
};

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::ComputeMainPerceptionDirection
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Perceiver::ComputeMainPerceptionDirection( MT_Vector2D& vMainPerceptionDirection ) const
{
    if( nSensorMode_ == eNormal )
        vMainPerceptionDirection = *perceiverDirection_;
    else if( nSensorMode_ == eDirection )
        vMainPerceptionDirection = vSensorInfo_;
    else if( nSensorMode_ == ePoint )
    {
        const MT_Vector2D& vDirection = *perceiverDirection_;
        const MT_Vector2D& vPosition  = *perceiverPosition_;
        if( vSensorInfo_ != vPosition )
            vMainPerceptionDirection = ( vSensorInfo_ - vPosition ).Normalize();
        else
            vMainPerceptionDirection = vDirection;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::PreparePerceptionData
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::PreparePerceptionData()
{
    if( ( *perceiverPosition_ == lastPerceiverPosition_ )
        && !bExternalMustChangePerception_
        && !HasChanged() )
        return;
    MT_Vector2D vMainPerceptionDirection;
    ComputeMainPerceptionDirection( vMainPerceptionDirection );
    surfacesAgent_.clear();
    surfacesObject_.clear();
    rMaxAgentPerceptionDistance_ = 0;
    rMaxObjectPerceptionDistance_ = 0;
    sPerceptionRotation rotation;
    std::auto_ptr< OnComponentComputer_ABC > componentComputer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( rotation ) );
    pion_.Execute( *componentComputer );
    sPerceptionDataComposantes dataFunctor( surfacesAgent_, surfacesObject_, *perceiverPosition_, *perceiverDirection_ , vMainPerceptionDirection, rotation.GetAngle() , rMaxAgentPerceptionDistance_, rMaxObjectPerceptionDistance_ );
    std::auto_ptr< OnComponentComputer_ABC > dataComputer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( dataFunctor ) );
    pion_.Execute( *dataComputer );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::PrepareRadarData
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::PrepareRadarData()
{
    if( !bExternalMustChangeRadar_ )
        return;
    radars_.clear();
    sRadarDataComposantes dataFunctor( radars_ );
    std::auto_ptr< OnComponentComputer_ABC > componentComputer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( dataFunctor ) );
    pion_.Execute( *componentComputer );
}

namespace
{
    template< typename T >
    void Reset( T*& perception )
    {
        delete perception;
        perception = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableAllPerceptions
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableAllPerceptions()
{
    activePerceptions_.clear();
    activePerceptions_.push_back( pPerceptionView_ );
    Reset( pPerceptionCoupDeSonde_ );
    Reset( pPerceptionRecoPoint_ );
    Reset( pPerceptionRecoLocalisation_ );
    Reset( pPerceptionRecoUrbanBlock_ );
    Reset( pPerceptionRecoObjects_ );
    Reset( pPerceptionSurveillance_ );
    Reset( pPerceptionRadar_ );
    Reset( pPerceptionAlat_ );
    Reset( pPerceptionFlyingShell_ );
}

namespace
{
    const float maxBlockPerceptionDistance = 100.f; // Distance under which we consider urban blocks for perception.
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::ExecutePerceptions
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::ExecutePerceptions()
{
    if( CanPerceive() )
    {
        CIT_PerceptionVector itPerception;

        double maxPerceptionDistance = GetMaxAgentPerceptionDistance();

        std::vector< const urban::TerrainObject_ABC* > perceivableUrbanBlock;
        MIL_AgentServer::GetWorkspace().GetUrbanModel().GetListWithinCircle( geometry::Point2f( static_cast< float >( perceiverPosition_->rX_ ),
                                                                                                           static_cast< float >( perceiverPosition_->rY_ ) ),
                                                                                        maxBlockPerceptionDistance,
                                                                                        perceivableUrbanBlock );
        if( !perceivableUrbanBlock.empty() )
        {
            double occupation = 0.;  
            for( std::vector< const urban::TerrainObject_ABC* >::const_iterator itUrban = perceivableUrbanBlock.begin(); itUrban != perceivableUrbanBlock.end(); ++itUrban )
            {
                const UrbanObjectWrapper& wrapper = MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( **itUrban );
                NotifyPerception( wrapper, PHY_PerceptionLevel::identified_ );
                occupation += wrapper.GetOccupation();
            }
            occupation /= perceivableUrbanBlock.size();
            maxPerceptionDistance *= ( 1 - 9*occupation/10 );
        }
        
        TER_Agent_ABC::T_AgentPtrVector perceivableAgents;
        TER_Object_ABC::T_ObjectVector perceivableObjects;
        TER_PopulationConcentration_ABC::T_ConstPopulationConcentrationVector perceivableConcentrations;
        TER_PopulationFlow_ABC::T_ConstPopulationFlowVector perceivableFlows;
        GetKnowledgeGroup().AppendAddedKnowledge( perceivableAgents, perceivableObjects, perceivableConcentrations, perceivableFlows );
        
        TER_World::GetWorld().GetAgentManager().GetListWithinCircle( *perceiverPosition_, maxPerceptionDistance, perceivableAgents );
        for( itPerception = activePerceptions_.begin(); itPerception != activePerceptions_.end(); ++itPerception )
            (**itPerception).Execute( perceivableAgents, *pion_.GetAlgorithms().detectionComputerFactory_ );

        TER_World::GetWorld().GetObjectManager().GetListWithinCircle( *perceiverPosition_, maxPerceptionDistance, perceivableObjects );
        AppendUniversalObjects( perceivableObjects );
        for( itPerception = activePerceptions_.begin(); itPerception != activePerceptions_.end(); ++itPerception )
            (**itPerception).Execute( perceivableObjects );

        TER_World::GetWorld().GetPopulationManager().GetConcentrationManager().GetListWithinCircle( *perceiverPosition_, maxPerceptionDistance, perceivableConcentrations );
        for( itPerception = activePerceptions_.begin(); itPerception != activePerceptions_.end(); ++itPerception )
            (**itPerception).Execute( perceivableConcentrations );

        TER_World::GetWorld().GetPopulationManager().GetFlowManager().GetListWithinCircle( *perceiverPosition_, maxPerceptionDistance, perceivableFlows );
        for( itPerception = activePerceptions_.begin(); itPerception != activePerceptions_.end(); ++itPerception )
            (**itPerception).Execute( perceivableFlows );

        for( itPerception = activePerceptions_.begin(); itPerception != activePerceptions_.end(); ++itPerception )
            (**itPerception).FinalizePerception();
    }
    NotifyPerception( pion_, PHY_PerceptionLevel::identified_, false );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_RolePion_Perceiver::ComputePerception( const DEC_Knowledge_Agent& knowledge ) const
{
    if( !CanPerceive() || !knowledge.IsValid() )
        return PHY_PerceptionLevel::notSeen_;
    const PHY_PerceptionLevel* pBestPerceptionLevel_ = &PHY_PerceptionLevel::notSeen_;
    for( CIT_PerceptionVector itPerception = activePerceptions_.begin(); itPerception != activePerceptions_.end(); ++itPerception )
    {
        pBestPerceptionLevel_ = &(**itPerception).Compute( knowledge );
        if( pBestPerceptionLevel_->IsBestLevel() )
            return *pBestPerceptionLevel_;
    }
    return *pBestPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_RolePion_Perceiver::ComputePerception( const DEC_Knowledge_Object& knowledge ) const
{
    if( !CanPerceive() )
        return PHY_PerceptionLevel::notSeen_;
    const PHY_PerceptionLevel* pBestPerceptionLevel_ = &PHY_PerceptionLevel::notSeen_;
    for( CIT_PerceptionVector itPerception = activePerceptions_.begin(); itPerception != activePerceptions_.end(); ++itPerception )
    {
        pBestPerceptionLevel_ = &(**itPerception).Compute( knowledge );
        if( pBestPerceptionLevel_->IsBestLevel() )
            return *pBestPerceptionLevel_;
    }
    return *pBestPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::ComputePerception
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_RolePion_Perceiver::ComputePerception( const MT_Vector2D& vPoint ) const
{
    if( !CanPerceive() )
        return PHY_PerceptionLevel::notSeen_;
    const PHY_PerceptionLevel* pBestPerceptionLevel_ = &PHY_PerceptionLevel::notSeen_;
    for( CIT_PerceptionVector itPerception = activePerceptions_.begin(); itPerception != activePerceptions_.end(); ++itPerception )
    {
        pBestPerceptionLevel_ = &(**itPerception).Compute( vPoint );
        if( pBestPerceptionLevel_->IsBestLevel() )
            return *pBestPerceptionLevel_;
    }
    return *pBestPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::UpdatePeriphericalVisionState
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::UpdatePeriphericalVisionState()
{
    const unsigned int nCurrentTime = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( nNextPeriphericalVisionStep_ <= nCurrentTime )
    {
        while ( nNextPeriphericalVisionStep_ <= nCurrentTime )
            nNextPeriphericalVisionStep_ += nNbrStepsBetweenPeriphericalVision_;
        bPeriphericalVisionEnabled_ = true;
        return;
    }
    bPeriphericalVisionEnabled_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::Update
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::Update( bool /*bIsDead*/ )
{
    UpdatePeriphericalVisionState();
    PreparePerceptionData        ();
    PrepareRadarData             ();
    lastPerceiverPosition_ = *perceiverPosition_;
    if( pPerceptionRecoPoint_ )
        pPerceptionRecoPoint_->Update();
   if( pPerceptionRecoLocalisation_ )
        pPerceptionRecoLocalisation_->Update();
    if( pPerceptionRecoObjects_ )
        pPerceptionRecoObjects_->Update();
    if( HasChanged() )
    {
        if( HasRadarStateChanged() )
            pion_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
        pion_.Apply( &network::VisionConeNotificationHandler_ABC::NotifyVisionConeDataHasChanged );
    }
    // Debug - Cones de vision
    if( MIL_AgentServer::GetWorkspace().GetAgentServer().MustSendUnitVisionCones() )
    {
        if( bExternalMustUpdateVisionCones_
            || MIL_AgentServer::GetWorkspace().GetAgentServer().MustInitUnitVisionCones() )
            SendDebugState();
    }
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetKnowledgeGroup
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
const MIL_KnowledgeGroup& PHY_RolePion_Perceiver::GetKnowledgeGroup() const
{
    return pion_.GetKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsKnown
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsKnown( const MIL_Agent_ABC& agent ) const
{
    return pion_.GetKnowledgeGroup().GetKnowledge().IsKnown( agent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsIdentified
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsIdentified( const MIL_Agent_ABC& agent ) const
{
    return pion_.GetKnowledge().IsIdentified( agent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsKnown
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsKnown( const MIL_Object_ABC& object ) const
{
    return pion_.GetArmy().GetKnowledge().IsKnown( object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsIdentified
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsIdentified( const MIL_Object_ABC& object ) const
{
    return pion_.GetKnowledge().IsIdentified( object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsIdentified
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsIdentified( const MIL_PopulationConcentration& concentration ) const
{
    return pion_.GetKnowledge().IsIdentified( concentration );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsIdentified
// Created: MGD 2009-11-25
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsIdentified( const UrbanObjectWrapper& object ) const
{
    return pion_.GetKnowledge().IsIdentified( object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerception( const MIL_Effect_IndirectFire& flyingShell ) const
{
    MIL_Report::PostEvent( pion_, MIL_Report::eReport_IndirectFireObservation, flyingShell );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level, bool bPerceptionRecorded )
{
    pion_.GetKnowledge().GetKsPerception().NotifyPerception( agent, level, bPerceptionRecorded );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::NotifyPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level )
{
    return pion_.GetKnowledge().GetKsPerception().NotifyPerception( agent, level, bRecordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerception( MIL_Object_ABC& object, const PHY_PerceptionLevel& level )
{
    pion_.GetKnowledge().GetKsPerception().NotifyPerception( object, level, bRecordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerception( MIL_PopulationConcentration& concentration, const PHY_PerceptionLevel& level )
{
    pion_.GetKnowledge().GetKsPerception().NotifyPerception( concentration, level, bRecordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerception( MIL_PopulationFlow& flow, const PHY_PerceptionLevel& level, const T_PointVector& shape )
{
    pion_.GetKnowledge().GetKsPerception().NotifyPerception( flow, level, shape, bRecordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: MGD 2009-11-20
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerception( const UrbanObjectWrapper& object, const PHY_PerceptionLevel& level ) const
{
    pion_.GetKnowledge().GetKsPerception().NotifyPerception( object, level );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifytExternalPerception
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyExternalPerception( MIL_Agent_ABC& agent , const PHY_PerceptionLevel& level )
{
    pion_.GetKnowledge().GetKsPerception().NotifyExternalPerception( agent, level );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRecordMode
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::EnableRecordMode()
{
    bRecordModeEnabled_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRecordMode
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRecordMode()
{
    bRecordModeEnabled_ = false;
    pion_.GetKnowledge().GetKsPerception().MakePerceptionsAvailable();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::HasDelayedPerceptions
// Created: JVT 2004-12-03
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::HasDelayedPerceptions() const
{
    return pion_.GetKnowledge().GetKsPerception().HasDelayedPerceptions();
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::SendDebugState
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::SendDebugState() const
{
    client::UnitVisionCones message;
    message().mutable_unit()->set_id( pion_.GetID() );
    std::auto_ptr< detection::PerceptionDistanceComputer_ABC > algorithm = pion_.GetAlgorithms().detectionComputerFactory_->CreateDistanceComputer();
    message().set_elongation( static_cast< float >( pion_.Execute( *algorithm ).GetElongationFactor() ) ); //@TODO MGD share
    message().mutable_cones();
    for( CIT_SurfaceAgentMap it = surfacesAgent_.begin(); it != surfacesAgent_.end(); ++it )
        it->second.SendFullState( *message().mutable_cones()->add_elem() );
    message.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::SendFullState( client::UnitAttributes& msg ) const
{
    msg().set_radar_active( IsUsingActiveRadar() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::SendChangedState( client::UnitAttributes& msg ) const
{
    if( bRadarStateHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsPeriphericalVisionEnabled
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsPeriphericalVisionEnabled() const
{
    return bPeriphericalVisionEnabled_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetPion
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MIL_Agent_ABC& PHY_RolePion_Perceiver::GetPion() const
{
    return pion_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::SetVisionModeNormal
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::SetVisionModeNormal()
{
    nSensorMode_ = eNormal;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::SetVisionModeDirection
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::SetVisionModeDirection( const MT_Vector2D& vDirection )
{
    nSensorMode_ = eDirection;
    vSensorInfo_ = vDirection;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::SetVisionModePoint
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::SetVisionModePoint( const MT_Vector2D& vPoint )
{
    nSensorMode_ = ePoint;
    vSensorInfo_ = vPoint;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::HasChanged
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::HasChanged() const
{
    return bHasChanged_ || bRadarStateHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetMainPerceptionDirection
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::GetMainPerceptionDirection( MT_Vector2D& vDirection ) const
{
    ComputeMainPerceptionDirection( vDirection );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::HasRadarStateChanged
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::HasRadarStateChanged() const
{
    return bRadarStateHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::Clean()
{
    bHasChanged_           = false;
    bExternalMustChangePerception_ = false;
    bExternalMustChangeRadar_ = false;
    bRadarStateHasChanged_ = false;
    bExternalMustUpdateVisionCones_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetSurfacesAgent
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const PHY_RolePion_Perceiver::T_SurfaceAgentMap& PHY_RolePion_Perceiver::GetSurfacesAgent() const
{
    return surfacesAgent_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetSurfacesObject
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const PHY_RolePion_Perceiver::T_SurfaceObjectMap& PHY_RolePion_Perceiver::GetSurfacesObject() const
{
    return surfacesObject_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetRadars
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const PHY_RolePion_Perceiver::T_RadarSet& PHY_RolePion_Perceiver::GetRadars( const PHY_RadarClass& radarClass )
{
    return radars_[ &radarClass ];
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::Execute
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::Execute( detection::DetectionComputer_ABC& algorithm ) const
{
    if( algorithm.GetTarget() != pion_ && pion_.GetKnowledge().WasPerceived( algorithm.GetTarget() ) )
        algorithm.AlreadyPerceived();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyHasChanged
// Created: MGD 2009-09-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyComponentHasChanged()
{
    bExternalMustChangePerception_ = true;
    bExternalMustChangeRadar_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyCaptured
// Created: MGD 2009-10-14
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyCaptured()
{
    bExternalCanPerceive_ = false;
    bExternalMustChangePerception_ = true;
    bExternalMustChangeRadar_ = true;
}
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyReleased
// Created: MGD 2009-10-1
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyReleased()
{
    bExternalCanPerceive_ = true;
    bExternalMustChangePerception_ = true;
    bExternalMustChangeRadar_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyVisionConeDataHasChanged
// Created: MGD 2009-10-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyVisionConeDataHasChanged()
{
    bExternalMustUpdateVisionCones_ = true;
}
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyIsLoaded
// Created: MGD 2009-10-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyIsLoadedForTransport()
{
    bExternalCanPerceive_ = false;
    bExternalMustChangePerception_ = true;
    bExternalMustChangeRadar_ = true;
}
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyIsUnLoaded
// Created: MGD 2009-10-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyIsUnLoadedForTransport()
{
    bExternalCanPerceive_ = true;
    bExternalMustChangePerception_ = true;
    bExternalMustChangeRadar_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyIsLoaded
// Created: MGD 2009-10-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyIsLoadedInVab()
{
    bExternalMustChangePerception_ = true;
    bExternalMustChangeRadar_ = true;
}
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyIsUnLoaded
// Created: MGD 2009-10-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyIsUnLoadedInVab()
{
    bExternalMustChangePerception_ = true;
    bExternalMustChangeRadar_ = true;
}

namespace
{
// -----------------------------------------------------------------------------
class UniversalObjectsVisitor : public TER_ObjectVisitor_ABC
{
public:
    UniversalObjectsVisitor( TER_Object_ABC::T_ObjectVector& perceivableObjects )
        : perceivableObjects_( perceivableObjects )
    {
        // NOTHING
    }
    virtual ~UniversalObjectsVisitor()
    {
        // NOTHING
    }
    virtual void Visit( TER_Object_ABC& object )
    {
        //MIL_Object_ABC* pObjectKnown = dynamic_cast< MIL_Object_ABC* >( &object );
        if( object.IsUniversal()
            && std::find( perceivableObjects_.begin(), perceivableObjects_.end(), &object ) == perceivableObjects_.end() )
            perceivableObjects_.push_back( &object );
    }
private:
    TER_Object_ABC::T_ObjectVector& perceivableObjects_;
};
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::AppendUniversalObjects
// Created: JSR 2011-01-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::AppendUniversalObjects( TER_Object_ABC::T_ObjectVector& perceivableObjects ) const
{
    UniversalObjectsVisitor visitor( perceivableObjects );
    TER_World::GetWorld().GetObjectManager().Accept( visitor );
}
