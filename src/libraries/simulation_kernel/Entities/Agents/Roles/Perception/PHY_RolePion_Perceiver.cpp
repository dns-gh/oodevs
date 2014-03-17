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
#include "AlgorithmsFactories.h"
#include "ConsumptionComputer_ABC.h"
#include "DetectionComputer.h"
#include "NetworkNotificationHandler_ABC.h"
#include "OnComponentFunctor_ABC.h"
#include "OnComponentFunctorComputer_ABC.h"
#include "OnComponentFunctorComputerFactory_ABC.h"
#include "PerceptionDistanceComputer.h"
#include "VisionConeNotificationHandler_ABC.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Actions/Underground/PHY_RoleAction_MovingUnderground.h"
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
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Units/Radars/PHY_RadarType.h"
#include "Entities/Agents/Units/Sensors/PHY_Sensor.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeObject.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KS_Perception.h"
#include "Knowledge/DEC_KS_ObjectInteraction.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KS_IndirectFire.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "MIL_AgentServer.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "Urban/MIL_UrbanCache.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Urban/UrbanPhysicalCapacity.h"
#include "simulation_terrain/TER_Agent_ABC.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "simulation_terrain/TER_AgentVisitor_ABC.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "simulation_terrain/TER_ObjectVisitor_ABC.h"
#include "simulation_terrain/TER_PopulationConcentration_ABC.h"
#include "simulation_terrain/TER_PopulationConcentrationVisitor_ABC.h"
#include "simulation_terrain/TER_PopulationFlow_ABC.h"
#include "simulation_terrain/TER_PopulationFlowVisitor_ABC.h"
#include "simulation_terrain/TER_PopulationManager.h"
#include "simulation_terrain/TER_World.h"
#include <boost/range/algorithm_ext/erase.hpp>

using namespace detection;


BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePion_Perceiver )

namespace
{
    const unsigned int nNbrStepsBetweenPeriphericalVision = 12; //$$$ En dur ...
    unsigned int nNbr = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver constructor
// Created: JSR 2013-01-09
// -----------------------------------------------------------------------------
PHY_RolePion_Perceiver::PHY_RolePion_Perceiver()
    : owner_                         ( 0 )
    , rMaxAgentPerceptionDistance_   ( 0 )
    , nSensorMode_                   ( eNormal )
    , bPeriphericalVisionEnabled_    ( false )
    , bRecordModeEnabled_            ( false )
    , bHasChanged_                   ( true )
    , bExternalMustChangePerception_ ( false )
    , bExternalMustChangeRadar_      ( false )
    , bExternalCanPerceive_          ( true )
    , bExternalMustUpdateVisionCones_( false )
    , bRadarStateHasChanged_         ( true )
    , bFireObserver_                 ( false )
    , pPerceptionView_               ( new PHY_PerceptionView( *this, false ) ) // false is arbitrary: Doesn't matter on first tick.
{
    ++nNbr;
    DisableAllPerceptions();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver constructor
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
PHY_RolePion_Perceiver::PHY_RolePion_Perceiver( MIL_Agent_ABC& pion )
    : owner_                         ( &pion )
    , nNextPeriphericalVisionStep_   ( ++nNbr % nNbrStepsBetweenPeriphericalVision )
    , rMaxAgentPerceptionDistance_   ( 0 )
    , nSensorMode_                   ( eNormal )
    , bPeriphericalVisionEnabled_    ( false )
    , bRecordModeEnabled_            ( false )
    , bHasChanged_                   ( true )
    , bExternalMustChangePerception_ ( false )
    , bExternalMustChangeRadar_      ( false )
    , bExternalCanPerceive_          ( true )
    , bExternalMustUpdateVisionCones_( false )
    , bRadarStateHasChanged_         ( true )
    , bFireObserver_                 ( false )
    , bPerceptionUponRequest_        ( false )
    , pPerceptionView_               ( new PHY_PerceptionView( *this, pion.GetRole< PHY_RoleInterface_UrbanLocation >().IsInCity() ) )
{
    DisableAllPerceptions();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_RolePion_Perceiver::~PHY_RolePion_Perceiver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Perceiver::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Perceiver >( *this );
    file & owner_;
    file & bPeriphericalVisionEnabled_;
    file & nNextPeriphericalVisionStep_;
    file & surfacesAgent_;
    file & surfacesObject_;
    file & rMaxAgentPerceptionDistance_;
    file & bFireObserver_;
    file & bPerceptionUponRequest_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableSensors
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::EnableSensors()
{
    pPerceptionView_->Enable();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableSensors
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableSensors()
{
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
    pPerceptionCoupDeSonde_.reset( new PHY_PerceptionCoupDeSonde( *this ) );
    perceptions_.push_back( pPerceptionCoupDeSonde_.get() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableCoupDeSonde
// Created: NLD 2004-10-14
// Modified: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableCoupDeSonde()
{
    boost::remove_erase( perceptions_, pPerceptionCoupDeSonde_.get() );
    pPerceptionCoupDeSonde_.reset();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRecoAlat
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::EnableRecoAlat( const TER_Localisation& localisation )
{
    if( pPerceptionAlat_ )
        return;
    pPerceptionAlat_.reset( new PHY_PerceptionAlat( *this, localisation ) );
    perceptions_.push_back( pPerceptionAlat_.get() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRecoAlat
// Created: NLD 2004-10-14
// Modified: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRecoAlat()
{
    boost::remove_erase( perceptions_, pPerceptionAlat_.get() );
    pPerceptionAlat_.reset();
    owner_->GetKnowledge().GetKsPerception().MakePerceptionsAvailableTimed();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableSurveillanceLocalisation
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableSurveillanceLocalisation( const TER_Localisation& localisation )
{
    return pPerceptionSurveillance_->AddLocalisation( localisation );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableSurveillanceLocalisation
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableSurveillanceLocalisation( int id )
{
    pPerceptionSurveillance_->RemoveLocalisation( id );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRecoPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableRecoPoint( const MT_Vector2D& center, double rSize, double rSpeed, DEC_Decision_ABC& callerAgent )
{
    return pPerceptionRecoPoint_->AddPoint( center, rSize, rSpeed, callerAgent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRecoPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRecoPoint( int id )
{
    pPerceptionRecoPoint_->RemovePoint( id );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRecoObjects
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableRecoObjects( const TER_Localisation& localisation, const MT_Vector2D& vCenter, double rSpeed, DEC_Decision_ABC& callerAgent )
{
    return  pPerceptionRecoObjects_->AddLocalisation( localisation, vCenter, rSpeed, callerAgent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRecoObjects
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRecoObjects( int id )
{
    pPerceptionRecoObjects_->RemoveLocalisation( id );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRecoLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableRecoLocalisation( const TER_Localisation& localisation, float rGrowthSpeed, DEC_Decision_ABC& callerAgent )
{
    return pPerceptionRecoLocalisation_->AddLocalisationWithGrowthSpeed( localisation, rGrowthSpeed, callerAgent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRecoLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableRecoLocalisation( const TER_Localisation& localisation, DEC_Decision_ABC& callerAgent )
{
    return pPerceptionRecoLocalisation_->AddLocalisationWithDefaultGrowthSpeed( localisation, callerAgent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableControlLocalisation
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableControlLocalisation( const TER_Localisation& localisation, DEC_Decision_ABC& callerAgent )
{
    return pPerceptionRecoLocalisation_->AddLocalisationWithDefaultGrowthSpeed( localisation, callerAgent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRecoLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRecoLocalisation( int id )
{
    pPerceptionRecoLocalisation_->RemoveLocalisation( id );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRecoLocalisation
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableRecoUrbanBlock( MIL_UrbanObject_ABC* pUrbanBlock )
{
    return pPerceptionRecoUrbanBlock_->AddUrbanBlock( pUrbanBlock );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRecoUrbanBlock
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRecoUrbanBlock( int id )
{
    pPerceptionRecoUrbanBlock_->RemoveUrbanBlock( id );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsUsingActiveRadar
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsUsingActiveRadar() const
{
    return pPerceptionRadar_->IsUsingActiveRadar()
        || pPerceptionFlyingShell_->HasLocalisationToHandle();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsUsingActiveRadar
// Created: JSR 2010-03-18
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsUsingActiveRadar( const PHY_RadarClass& radarClass ) const
{
    return pPerceptionRadar_->IsUsingActiveRadar( radarClass )
        || pPerceptionFlyingShell_->HasLocalisationToHandle();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRadarOnLocalisation
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableRadarOnLocalisation( const PHY_RadarClass& radarClass, const TER_Localisation& localisation )
{
    if( !pPerceptionRadar_->HasRadarToHandle() )
        bRadarStateHasChanged_ = true;
    return pPerceptionRadar_->EnableRadar( radarClass, localisation );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRadarOnLocalisation
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRadarOnLocalisation( const PHY_RadarClass& radarClass, int id )
{
    pPerceptionRadar_->DisableRadar( radarClass, id );
    if( !pPerceptionRadar_->HasRadarToHandle() )
        bRadarStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRadar
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::EnableRadar( const PHY_RadarClass& radarClass )
{
    if( !pPerceptionRadar_->HasRadarToHandle() )
        bRadarStateHasChanged_ = true;
    return pPerceptionRadar_->EnableRadar( radarClass );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRadar
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRadar( const PHY_RadarClass& radarClass )
{
    pPerceptionRadar_->DisableRadar( radarClass );
    if( !pPerceptionRadar_->HasRadarToHandle() )
        bRadarStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableFlyingShellDetection
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
int PHY_RolePion_Perceiver::EnableFlyingShellDetection( const TER_Localisation& localisation )
{
    if( !pPerceptionFlyingShell_->HasLocalisationToHandle() )
        bRadarStateHasChanged_ = true;
    return pPerceptionFlyingShell_->AddLocalisation( localisation );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableFlyingShellDetection
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableFlyingShellDetection( int id )
{
    pPerceptionFlyingShell_->RemoveLocalisation( id );
    if( !pPerceptionFlyingShell_->HasLocalisationToHandle() )
        bRadarStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetMaxAgentPerceptionDistance
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
double PHY_RolePion_Perceiver::GetMaxAgentPerceptionDistance() const
{
    detection::PerceptionDistanceComputer computer;
    return rMaxAgentPerceptionDistance_ * owner_->Execute( computer ).GetFactor();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::PHY_RolePion_Perceiver::GetMaxTheoreticalAgentPerceptionDistance
// Created: MMC 2011-05-11
// -----------------------------------------------------------------------------
double PHY_RolePion_Perceiver::GetMaxTheoreticalAgentPerceptionDistance() const
{
    return rMaxAgentPerceptionDistance_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::CanPerceive
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::CanPerceive() const
{
    return !owner_->IsDead() && !owner_->GetRole< PHY_RoleAction_MovingUnderground >().IsUnderground() && bExternalCanPerceive_;
}

namespace
{

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

struct sPerceptionDataSensors : private boost::noncopyable
{
public:
    sPerceptionDataSensors( PHY_RolePion_Perceiver::T_SurfaceAgentMap& surfacesAgent, PHY_RolePion_Perceiver::T_SurfaceObjectMap& surfacesObject,
                            PHY_RolePion_Perceiver::T_DisasterDetectors& disasterDetectors, const MT_Vector2D& vOrigin, const MT_Vector2D& vDirection,
                            double& rMaxAgentPerceptionDistance, bool bPerceptionUponRequest )
        : surfacesAgent_               ( surfacesAgent )
        , surfacesObject_              ( surfacesObject )
        , disasterDetectors_           ( disasterDetectors )
        , vOrigin_                     ( vOrigin )
        , vDirection_                  ( vDirection )
        , rMaxAgentPerceptionDistance_ ( rMaxAgentPerceptionDistance )
        , bPerceptionUponRequest_      ( bPerceptionUponRequest )
    {
        // NOTHING
    }

    void operator()( const PHY_Sensor& sensor )
    {
        bool isActivatedOnRequest = sensor.GetType().IsActivatedOnRequest();
        if( !isActivatedOnRequest || ( isActivatedOnRequest && bPerceptionUponRequest_ ) )
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
                PHY_PerceptionSurfaceObject& surface = surfacesObject_[ std::make_pair( pSensorTypeObject, sensor.GetHeight() ) ];
                if( !surface.IsInitialized() )
                    surface = PHY_PerceptionSurfaceObject( *pSensorTypeObject, vOrigin_, sensor.GetHeight() );
            }
            const PHY_SensorTypeDisaster* pSensorTypeDisaster = sensor.GetType().GetTypeDisaster();
            if( pSensorTypeDisaster )
                if( std::find( disasterDetectors_.begin(), disasterDetectors_.end(), pSensorTypeDisaster ) == disasterDetectors_.end() )
                    disasterDetectors_.push_back( pSensorTypeDisaster );
        }
    }

private:
    PHY_RolePion_Perceiver::T_SurfaceAgentMap&   surfacesAgent_;
    PHY_RolePion_Perceiver::T_SurfaceObjectMap&  surfacesObject_;
    PHY_RolePion_Perceiver::T_DisasterDetectors& disasterDetectors_;
    const MT_Vector2D&                           vOrigin_;
    const MT_Vector2D&                           vDirection_;
    double&                                      rMaxAgentPerceptionDistance_;
    bool                                         bPerceptionUponRequest_;
};

class sPerceptionDataComposantes : public OnComponentFunctor_ABC
{
public:
    sPerceptionDataComposantes( MIL_Agent_ABC& pion, PHY_RolePion_Perceiver::T_SurfaceAgentMap& surfacesAgent, PHY_RolePion_Perceiver::T_SurfaceObjectMap& surfacesObject,
                                PHY_RolePion_Perceiver::T_DisasterDetectors& disasterDetectors, const MT_Vector2D& vMainPerceptionDirection, double rDirectionRotation,
                                double& rMaxAgentPerceptionDistance, bool bPerceptionUponRequest )
        : surfacesAgent_               ( surfacesAgent )
        , surfacesObject_              ( surfacesObject )
        , disasterDetectors_           ( disasterDetectors )
        , position_                    ( pion.GetRole< PHY_RoleInterface_Location >().GetPosition() )
        , direction_                   ( pion.GetRole< PHY_RoleInterface_Location >().GetDirection() )
        , vMainPerceptionDirection_    ( vMainPerceptionDirection )
        , nRotationIdx_                ( vMainPerceptionDirection != direction_ ? -1 : 0 ) // détection lockée
        , rRotationAngle_              ( rDirectionRotation )
        , rMaxAgentPerceptionDistance_ ( rMaxAgentPerceptionDistance )
        , transport_                   ( pion.RetrieveRole< transport::PHY_RoleAction_Loading >() )
        , bPerceptionUponRequest_      ( bPerceptionUponRequest )
    {
        // NOTHING
    }

    void operator()( PHY_ComposantePion& composante )
    {
        if( !composante.CanPerceive( transport_ ) )
            return;
        MT_Vector2D vComposantePerceptionDirection( vMainPerceptionDirection_ );
        if( nRotationIdx_ == 0 )
            vComposantePerceptionDirection = direction_;
        else if( nRotationIdx_ > 0 )
            vComposantePerceptionDirection.Rotate( ( ( 2 * ( nRotationIdx_ & 0x1 ) - 1 ) * ( ( nRotationIdx_ + 1 ) >> 1 ) ) * rRotationAngle_ );
        ++nRotationIdx_;
        sPerceptionDataSensors dataFunctor( surfacesAgent_, surfacesObject_, disasterDetectors_, position_, vComposantePerceptionDirection, rMaxAgentPerceptionDistance_, bPerceptionUponRequest_ );
        composante.ApplyOnSensors( dataFunctor );
    }

private:
    PHY_RolePion_Perceiver::T_SurfaceAgentMap&   surfacesAgent_;
    PHY_RolePion_Perceiver::T_SurfaceObjectMap&  surfacesObject_;
    PHY_RolePion_Perceiver::T_DisasterDetectors& disasterDetectors_;
    const MT_Vector2D&                           position_;
    const MT_Vector2D&                           direction_;
    const MT_Vector2D&                           vMainPerceptionDirection_;
    double&                                      rMaxAgentPerceptionDistance_;
    int                                          nRotationIdx_;
    const double                                 rRotationAngle_;
    const transport::PHY_RoleAction_Loading*     transport_;
    bool                                         bPerceptionUponRequest_;
};

class sRadarDataComposantes : public OnComponentFunctor_ABC
{

public:
    sRadarDataComposantes( MIL_Agent_ABC& pion, PHY_RolePion_Perceiver::T_RadarsPerClassMap& radars )
        : radars_( radars )
        , transport_( pion.RetrieveRole< transport::PHY_RoleAction_Loading >() )
    {
        // NOTHING
    }

    void operator()( PHY_ComposantePion& composante )//@TODO MGD Use same design for weaponAvailability
    {
        if( !composante.CanPerceive( transport_ ) )
            return;
        composante.ApplyOnRadars( *this );
    }

    void operator()( const PHY_RadarType& radarType, double height )
    {
        radars_[ &radarType.GetClass() ][ &radarType ] = height;
    }

private:
    PHY_RolePion_Perceiver::T_RadarsPerClassMap& radars_;
    const transport::PHY_RoleAction_Loading* transport_;
};

}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::ComputeMainPerceptionDirection
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::ComputeMainPerceptionDirection( MT_Vector2D& vMainPerceptionDirection ) const
{
    if( nSensorMode_ == eNormal )
        vMainPerceptionDirection = owner_->GetRole< PHY_RoleInterface_Location >().GetDirection();
    else if( nSensorMode_ == eDirection )
        vMainPerceptionDirection = vSensorInfo_;
    else if( nSensorMode_ == ePoint )
    {
        const MT_Vector2D& vDirection = owner_->GetRole< PHY_RoleInterface_Location >().GetDirection();
        const MT_Vector2D& vPosition  = owner_->GetRole< PHY_RoleInterface_Location >().GetPosition();
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
    if( ( lastPerceiverPosition_ == owner_->GetRole< PHY_RoleInterface_Location >().GetPosition() )
        && !bExternalMustChangePerception_
        && !HasChanged() )
        return;
    MT_Vector2D vMainPerceptionDirection;
    ComputeMainPerceptionDirection( vMainPerceptionDirection );
    surfacesAgent_.clear();
    surfacesObject_.clear();
    rMaxAgentPerceptionDistance_ = 0;
    sPerceptionRotation rotation;
    std::auto_ptr< OnComponentComputer_ABC > componentComputer( owner_->GetAlgorithms().onComponentFunctorComputerFactory_->Create( rotation ) );
    owner_->Execute( *componentComputer );
    sPerceptionDataComposantes dataFunctor( *owner_, surfacesAgent_, surfacesObject_, disasterDetectors_, vMainPerceptionDirection,
                                            rotation.GetAngle() , rMaxAgentPerceptionDistance_, bPerceptionUponRequest_ );
    std::auto_ptr< OnComponentComputer_ABC > dataComputer( owner_->GetAlgorithms().onComponentFunctorComputerFactory_->Create( dataFunctor ) );
    owner_->Execute( *dataComputer );
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
    sRadarDataComposantes dataFunctor( *owner_, radars_ );
    std::auto_ptr< OnComponentComputer_ABC > componentComputer( owner_->GetAlgorithms().onComponentFunctorComputerFactory_->Create( dataFunctor ) );
    owner_->Execute( *componentComputer );
}

template< typename T >
void PHY_RolePion_Perceiver::Reset( std::unique_ptr< T >& pointer )
{
    pointer.reset( new T( *this ) );
    perceptions_.push_back( pointer.get() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableAllPerceptions
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableAllPerceptions()
{
    perceptions_.clear();
    perceptions_.push_back( pPerceptionView_.get() );
    Reset( pPerceptionRecoPoint_ );
    Reset( pPerceptionRecoLocalisation_ );
    Reset( pPerceptionRecoUrbanBlock_ );
    Reset( pPerceptionRecoObjects_ );
    Reset( pPerceptionSurveillance_ );
    Reset( pPerceptionRadar_ );
    Reset( pPerceptionFlyingShell_ );
    pPerceptionCoupDeSonde_.reset();
    pPerceptionAlat_.reset();
}

namespace
{
    const float maxBlockPerceptionDistance = 100.f; // Distance under which we consider urban blocks for pPerception.
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::ExecutePerceptions
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::ExecutePerceptions()
{
    if( CanPerceive() )
    {
        double maxPerceptionDistance = GetMaxAgentPerceptionDistance();

        std::vector< const MIL_UrbanObject_ABC* > perceivableUrbanBlock;
        const MT_Vector2D& position = owner_->GetRole< PHY_RoleInterface_Location >().GetPosition();
        MIL_AgentServer::GetWorkspace().GetUrbanCache().GetListWithinCircle( position, maxBlockPerceptionDistance, perceivableUrbanBlock );
        if( !perceivableUrbanBlock.empty() )
        {
            double occupation = 0.;
            double maxBlockLength = 0.;
            for( auto it = perceivableUrbanBlock.cbegin(); it != perceivableUrbanBlock.end(); ++it )
            {
                NotifyPerceptionUrban( **it, PHY_PerceptionLevel::identified_ );
                if( const UrbanPhysicalCapacity* physical = (*it)->Retrieve< UrbanPhysicalCapacity >() )
                    occupation += physical->GetOccupation();
                maxBlockLength = std::max( maxBlockLength, (*it)->GetLocalisation().GetLength() );
            }
            occupation /= perceivableUrbanBlock.size();
            maxPerceptionDistance = maxPerceptionDistance * ( 1 - 0.9 * occupation ) + maxBlockLength;
        }

        TER_Agent_ABC::T_AgentPtrVector perceivableAgents;
        TER_Object_ABC::T_ObjectVector perceivableObjects;
        TER_PopulationConcentration_ABC::T_ConstPopulationConcentrationVector perceivableConcentrations;
        TER_PopulationFlow_ABC::T_ConstPopulationFlowVector perceivableFlows;
        GetKnowledgeGroup()->AppendAddedKnowledge( perceivableAgents, perceivableObjects, perceivableConcentrations, perceivableFlows );

        TER_World::GetWorld().GetAgentManager().GetListWithinCircle( position, maxPerceptionDistance, perceivableAgents );
        owner_->InteractWithTraffic( perceivableAgents );

        for( auto it = perceptions_.begin(); it != perceptions_.end(); ++it )
            (**it).Execute( perceivableAgents );

        TER_World::GetWorld().GetObjectManager().GetListWithinCircle( position, maxPerceptionDistance, perceivableObjects );
        for( auto it = perceptions_.begin(); it != perceptions_.end(); ++it )
            (**it).Execute( perceivableObjects );

        TER_World::GetWorld().GetPopulationManager().GetConcentrationManager().GetListWithinCircle( position, maxPerceptionDistance, perceivableConcentrations );
        for( auto it = perceptions_.begin(); it != perceptions_.end(); ++it )
            (**it).Execute( perceivableConcentrations );

        TER_World::GetWorld().GetPopulationManager().GetFlowManager().GetListWithinCircle( position, maxPerceptionDistance, perceivableFlows );
        for( auto it = perceptions_.begin(); it != perceptions_.end(); ++it )
            (**it).Execute( perceivableFlows );

        for( auto it = perceptions_.begin(); it != perceptions_.end(); ++it )
            (**it).FinalizePerception();
    }
    NotifyPerception( *owner_, PHY_PerceptionLevel::identified_, false );
    const MIL_Agent_ABC* transporter = owner_->GetRole< transport::PHY_RoleInterface_Transported >().GetTransporter();
    if( transporter )
        NotifyPerception( const_cast< MIL_Agent_ABC& >( *transporter ), PHY_PerceptionLevel::identified_, false );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::ExecuteCollisions
// Created: LGY 2013-01-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::ExecuteCollisions()
{
    if( CanPerceive() )
    {
        const MT_Vector2D& position = owner_->GetRole< PHY_RoleInterface_Location >().GetPosition();
        TER_Object_ABC::T_ObjectVector perceivableObjects;
        TER_World::GetWorld().GetObjectManager().GetListWithinCircle( position, 1., perceivableObjects );
        for( auto it = perceptions_.begin(); it != perceptions_.end(); ++it )
            (**it).ExecuteCollisions( perceivableObjects );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_RolePion_Perceiver::ComputePerception( const DEC_Knowledge_Object& knowledge ) const
{
    if( !CanPerceive() )
        return PHY_PerceptionLevel::notSeen_;
    if( knowledge.GetLocalisation().IsInside( owner_->GetRole< PHY_RoleInterface_Location >().GetPosition() ) )
        return PHY_PerceptionLevel::identified_;
    const PHY_PerceptionLevel* pBestPerceptionLevel_ = &PHY_PerceptionLevel::notSeen_;
    for( auto it = perceptions_.begin(); it != perceptions_.end(); ++it )
    {
        pBestPerceptionLevel_ = &(**it).Compute( knowledge );
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
    for( auto it = perceptions_.begin(); it != perceptions_.end(); ++it )
    {
        pBestPerceptionLevel_ = &(**it).Compute( vPoint );
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
    const unsigned int nCurrentTime = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    if( nNextPeriphericalVisionStep_ <= nCurrentTime )
    {
        while( nNextPeriphericalVisionStep_ <= nCurrentTime )
            nNextPeriphericalVisionStep_ += nNbrStepsBetweenPeriphericalVision;
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
    lastPerceiverPosition_ = owner_->GetRole< PHY_RoleInterface_Location >().GetPosition();
    pPerceptionRecoPoint_->Update();
    pPerceptionRecoLocalisation_->Update();
    pPerceptionRecoObjects_->Update();
    if( HasChanged() )
    {
        if( bRadarStateHasChanged_ )
            owner_->Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
        owner_->Apply( &network::VisionConeNotificationHandler_ABC::NotifyVisionConeDataHasChanged );
    }
    if( bExternalMustUpdateVisionCones_ && MIL_AgentServer::GetWorkspace().GetEntityManager().SendVisionCones() )
        SendVisionCones();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetKnowledgeGroup
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > PHY_RolePion_Perceiver::GetKnowledgeGroup() const
{
    return owner_->GetKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsKnown
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsKnown( const MIL_Agent_ABC& agent ) const
{
    auto bbKg = owner_->GetKnowledgeGroup()->GetKnowledge();
    return bbKg ? bbKg->IsKnown( agent ) : false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsIdentified
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsIdentified( const MIL_Agent_ABC& agent ) const
{
    return owner_->GetKnowledge().IsIdentified( agent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsKnown
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsKnown( const MIL_Object_ABC& object ) const
{
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = owner_->GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        return container->HasKnowledgeObject( object );
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsIdentified
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsIdentified( const MIL_Object_ABC& object ) const
{
    return owner_->GetKnowledge().IsIdentified( object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsIdentified
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsIdentified( const MIL_PopulationConcentration& concentration ) const
{
    return owner_->GetKnowledge().IsIdentified( concentration );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsIdentified
// Created: MGD 2009-11-25
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsIdentified( const MIL_UrbanObject_ABC& object ) const
{
    return owner_->GetKnowledge().IsIdentified( object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerception( const MIL_Effect_IndirectFire& flyingShell ) const
{
    owner_->GetKnowledge().GetKsIndirectFire().NotifyAttackedBy( flyingShell.GetFirer() );
    MIL_Report::PostEvent( *owner_, report::eRC_ObservationTirIndirect, flyingShell );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level, bool bPerceptionRecorded )
{
    owner_->GetKnowledge().GetKsPerception().NotifyPerception( agent, level, bPerceptionRecorded );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::NotifyPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level )
{
    return owner_->GetKnowledge().GetKsPerception().NotifyPerception( agent, level, bRecordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerception( MIL_Object_ABC& object, const PHY_PerceptionLevel& level )
{
    owner_->GetKnowledge().GetKsPerception().NotifyPerception( object, level, bRecordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::NotifyPerception( MIL_PopulationConcentration& concentration, const PHY_PerceptionLevel& level )
{
    return owner_->GetKnowledge().GetKsPerception().NotifyPerception( concentration, level, bRecordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::NotifyPerception( MIL_PopulationFlow& flow, const PHY_PerceptionLevel& level, const T_PointVector& shape )
{
    return owner_->GetKnowledge().GetKsPerception().NotifyPerception( flow, level, shape, bRecordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: MGD 2009-11-20
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerceptionUrban( const MIL_UrbanObject_ABC& object, const PHY_PerceptionLevel& level ) const
{
    owner_->GetKnowledge().GetKsPerception().NotifyPerception( object, level );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifytExternalPerception
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyExternalPerception( MIL_Agent_ABC& agent , const PHY_PerceptionLevel& level )
{
    owner_->GetKnowledge().GetKsPerception().NotifyExternalPerception( agent, level );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerception( MIL_Object_ABC& object, const MT_Vector2D& vPosition, const MT_Vector2D& vDirection )
{
    owner_->GetKnowledge().GetKsObjectInteraction().NotifyDisasterCollision( object, vPosition, vDirection );
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
    owner_->GetKnowledge().GetKsPerception().MakePerceptionsAvailable();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::HasDelayedPerceptions
// Created: JVT 2004-12-03
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::HasDelayedPerceptions() const
{
    return owner_->GetKnowledge().GetKsPerception().HasDelayedPerceptions();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::SendVisionCones
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::SendVisionCones() const
{
    client::UnitVisionCones message;
    message().mutable_unit()->set_id( owner_->GetID() );
    // Elongation factor is deprecated, its value was 1.0 for relevant cases
    message().set_elongation( 1.0 );
    message().mutable_cones();
    for( auto it = surfacesAgent_.begin(); it != surfacesAgent_.end(); ++it )
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
    MT_Vector2D direction;
    GetMainPerceptionDirection( direction );
    NET_ASN_Tools::WriteDirection( direction, *msg().mutable_sensors_direction() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::SendChangedState( client::UnitAttributes& msg ) const
{
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
    return *owner_;
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
// Name: PHY_RolePion_Perceiver::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::Clean()
{
    bHasChanged_ = false;
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
// Name: PHY_RolePion_Perceiver::GetDisasterDetectors
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
const PHY_RolePion_Perceiver::T_DisasterDetectors& PHY_RolePion_Perceiver::GetDisasterDetectors() const
{
    return disasterDetectors_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetRadars
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const PHY_RolePion_Perceiver::T_RadarMap& PHY_RolePion_Perceiver::GetRadars( const PHY_RadarClass& radarClass )
{
    return radars_[ &radarClass ];
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::Execute
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::Execute( detection::DetectionComputer& algorithm ) const
{
    if( algorithm.GetTarget() != *owner_ && owner_->GetKnowledge().WasPerceived( algorithm.GetTarget() ) )
        algorithm.AlreadyPerceived();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::Execute
// Created: LDC 2012-06-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::Execute( dotation::ConsumptionComputer_ABC& algorithm ) const
{
    if( pPerceptionRecoPoint_->HasPointToHandle()
        || pPerceptionRecoLocalisation_->HasLocalisationToHandle()
        || pPerceptionRecoUrbanBlock_->HasLocalisationToHandle()
        || pPerceptionRecoObjects_->HasLocalisationToHandle() )
        algorithm.SetConsumptionMode( PHY_ConsumptionType::moving_ );
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
// Name: PHY_RolePion_Perceiver::NotifySurrendered
// Created: LDC 2012-08-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifySurrendered()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifySurrenderCanceled
// Created: LDC 2012-08-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifySurrenderCanceled()
{
    // NOTHING
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableFireObserver
// Created: LGY 2012-06-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::EnableFireObserver()
{
    bFireObserver_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableFireObserver
// Created: LGY 2012-06-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableFireObserver()
{
    bFireObserver_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsFireObserver
// Created: LGY 2012-06-28
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsFireObserver() const
{
    return bFireObserver_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnablePerceptionUponRequest
// Created: LGY 2012-12-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::EnablePerceptionUponRequest()
{
    bPerceptionUponRequest_ = true;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisablePerceptionUponRequest
// Created: LGY 2012-12-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisablePerceptionUponRequest()
{
    bPerceptionUponRequest_ = false;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::CanPerceive
// Created: MMC 2013-01-11
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::CanPerceive( const MIL_ObjectType_ABC& objectType ) const
{
    if( !owner_ )
        return false;
    const PHY_RolePion_Composantes* pComposantes = owner_->RetrieveRole< PHY_RolePion_Composantes >();
    return pComposantes && pComposantes->CanPerceive( objectType );
}

namespace
{
    class SensorFunctor : private boost::noncopyable
    {
    public:
        SensorFunctor( const MIL_Agent_ABC& perceiver, const MT_Vector2D& point, const MT_Vector2D& target )
            : perceiver_( perceiver )
            , point_    ( point )
            , target_   ( target )
            , energy_   ( 0 )
            , posted_   ( IsPosted( perceiver ) )
        {
        }
        ~SensorFunctor()
        {}
        void operator()( const PHY_Sensor& sensor )
        {
            const PHY_SensorTypeAgent* sensorTypeAgent = sensor.GetType().GetTypeAgent();
            if( sensorTypeAgent )
                energy_ = std::max( energy_, sensorTypeAgent->RayTrace( point_, target_, sensor.GetHeight(), posted_ ) );
        }
        double GetEnergy() const
        {
            return energy_;
        }
    private:
        const MIL_Agent_ABC& perceiver_;
        const MT_Vector2D& point_;
        const MT_Vector2D& target_;
        double energy_;
        const bool posted_;
    };

    class Functor : public OnComponentFunctor_ABC
    {
    public:
        Functor( const MIL_Agent_ABC& perceiver, const MT_Vector2D& point, const MT_Vector2D& target )
            : perceiver_( perceiver )
            , transport_( perceiver.RetrieveRole< transport::PHY_RoleAction_Loading >() )
            , point_    ( point )
            , target_   ( target )
            , energy_   ( 0 )
        {}
        void operator()( PHY_ComposantePion& composante )
        {
            if( !composante.CanPerceive( transport_ ) )
                return;
            SensorFunctor dataFunctor( perceiver_, point_, target_ );
            composante.ApplyOnSensors( dataFunctor );
            energy_ = std::max( energy_, dataFunctor.GetEnergy() );
        }
        double GetEnergy() const
        {
            return energy_;
        }
    private:
        const MIL_Agent_ABC& perceiver_;
        const transport::PHY_RoleAction_Loading* transport_;
        const MT_Vector2D& point_;
        const MT_Vector2D& target_;
        double energy_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetPerception
// Created: LDC 2013-02-26
// -----------------------------------------------------------------------------
double PHY_RolePion_Perceiver::GetPerception( const MT_Vector2D& from, const MT_Vector2D& to ) const
{
    Functor dataFunctor( *owner_, from, to );
    std::auto_ptr< OnComponentComputer_ABC > dataComputer( owner_->GetAlgorithms().onComponentFunctorComputerFactory_->Create( dataFunctor ) );
    const_cast< MIL_Agent_ABC& >( *owner_ ).Execute( *dataComputer );
    return dataFunctor.GetEnergy();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsReconnoitering
// Created: LDC 2013-10-07
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsReconnoitering( MIL_UrbanObject_ABC* pUrbanBlock ) const
{
    return pPerceptionRecoUrbanBlock_->IsReconnoitering( pUrbanBlock );
}
