// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.cpp $
// $Author: Jvt $
// $Modtime: 24/05/05 10:19 $
// $Revision: 33 $
// $Workfile: PHY_RolePion_Perceiver.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_RolePion_Perceiver.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Radars/PHY_RadarType.h"
#include "Entities/Agents/Units/Sensors/PHY_Sensor.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeObject.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.h"
#include "Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionView.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionCoupDeSonde.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionRecoPoint.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionRecoLocalisation.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionRecoSurveillance.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionRecoObjects.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionRadar.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionAlat.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionFlyingShell.h"
#include "Entities/RC/MIL_RC_ObservationTirIndirect.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KS_Perception.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Network/NET_AS_MOSServer.h"
#include "Network/NET_AS_MOSServerMsgMgr.h"
#include "Network/NET_AgentServer.h"
#include "Network/NET_ASN_Messages.h"

#include "TER/TER_Agent_ABC.h"
#include "TER/TER_PopulationConcentration_ABC.h"
#include "TER/TER_PopulationFlow_ABC.h"
#include "TER/TER_PopulationManager.h"
#include "TER/TER_World.h"

const uint PHY_RolePion_Perceiver::nNbrStepsBetweenPeriphericalVision_ = 12; //$$$ En dur ...

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Perceiver, "PHY_RolePion_Perceiver" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver constructor
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
PHY_RolePion_Perceiver::PHY_RolePion_Perceiver( MT_RoleContainer& role, MIL_AgentPion& pion )
    : PHY_RoleInterface_Perceiver  ( role )
    , pPion_                       ( &pion )
    , rMaxAgentPerceptionDistance_ ( 0. )
    , rMaxObjectPerceptionDistance_( 0. )
    , bPeriphericalVisionEnabled_  ( false )
    , bRecordModeEnabled_          ( false )
    , vSensorInfo_                 ( )
    , nSensorMode_                 ( eNormal )
    , bHasChanged_                 ( true )
    , bRadarStateHasChanged_       ( true )
    , pPerceptionCoupDeSonde_      ( 0 )
    , pPerceptionRecoPoint_        ( 0 )
    , pPerceptionRecoLocalisation_ ( 0 )
    , pPerceptionRadar_            ( 0 )
    , pPerceptionAlat_             ( 0 )
    , pPerceptionSurveillance_     ( 0 )
    , pPerceptionRecoObjects_      ( 0 )
    , pPerceptionFlyingShell_      ( 0 )
{
    static uint nNbr = 0;
    nNextPeriphericalVisionStep_ = ++nNbr % nNbrStepsBetweenPeriphericalVision_;

    pPerceptionView_ = new PHY_PerceptionView( *this );
    activePerceptions_.push_back( pPerceptionView_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_RolePion_Perceiver::PHY_RolePion_Perceiver()
    : PHY_RoleInterface_Perceiver  ()
    , pPion_                       ()
    , rMaxAgentPerceptionDistance_ ( 0. )
    , rMaxObjectPerceptionDistance_( 0. )
    , bPeriphericalVisionEnabled_  ( false )
    , bRecordModeEnabled_          ( false )
    , vSensorInfo_                 ( )
    , nSensorMode_                 ( eNormal )
    , bHasChanged_                 ( true )
    , bRadarStateHasChanged_       ( true )
    , pPerceptionCoupDeSonde_      ( 0 )
    , pPerceptionRecoPoint_        ( 0 )
    , pPerceptionRecoLocalisation_ ( 0 )
    , pPerceptionRadar_            ( 0 )
    , pPerceptionAlat_             ( 0 )
    , pPerceptionSurveillance_     ( 0 )
    , pPerceptionRecoObjects_      ( 0 )
    , pPerceptionFlyingShell_      ( 0 )
{
    static uint nNbr = 0;
    nNextPeriphericalVisionStep_ = ++nNbr % nNbrStepsBetweenPeriphericalVision_;

    pPerceptionView_ = new PHY_PerceptionView( *this );
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
        void serialize( Archive& file, PHY_RolePion_Perceiver::T_SurfaceAgentKeyPair& pair, const uint nVersion )
        {
            split_free( file, pair, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const PHY_RolePion_Perceiver::T_SurfaceAgentKeyPair& pair, const uint )
        {
            assert( pair.first );
            
            file << pair.first->GetType().GetID()
                 << pair.second;
        }
        
        template< typename Archive >
        void load( Archive& file, PHY_RolePion_Perceiver::T_SurfaceAgentKeyPair& pair, const uint )
        {
            uint nID;
            
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
        void serialize( Archive& file, PHY_RolePion_Perceiver::T_SurfaceObjectKeyPair& pair, const uint nVersion )
        {
            split_free( file, pair, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const PHY_RolePion_Perceiver::T_SurfaceObjectKeyPair& pair, const uint )
        {
            assert( pair.first );
            
            file << pair.first->GetType().GetID()
                 << pair.second;
        }
        
        template< typename Archive >
        void load( Archive& file, PHY_RolePion_Perceiver::T_SurfaceObjectKeyPair& pair, const uint )
        {
            uint nID;
            
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
void PHY_RolePion_Perceiver::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Perceiver >( *this )
         & pPion_
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
    if ( pPerceptionView_ )
        pPerceptionView_->Enable();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableSensors
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableSensors()
{
    if ( pPerceptionView_ )
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
    assert( pPion_ );
    pPion_->GetKnowledge().GetKsPerception().MakePerceptionsAvailableTimed();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableSurveillanceLocalisation
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
void* PHY_RolePion_Perceiver::EnableSurveillanceLocalisation( const TER_Localisation& localisation )
{
    if ( !pPerceptionSurveillance_ )
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
void PHY_RolePion_Perceiver::DisableSurveillanceLocalisation( void* pId )
{
    if ( !pPerceptionSurveillance_ )
        return;

    pPerceptionSurveillance_->RemoveLocalisation( pId );
    if ( !pPerceptionSurveillance_->HasLocalisationToHandle() )
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
void* PHY_RolePion_Perceiver::EnableRecoPoint( const MT_Vector2D& center, MT_Float rSize, MT_Float rSpeed, DIA_Variable_ABC& result )
{
    if ( !pPerceptionRecoPoint_ )
    {
        pPerceptionRecoPoint_ = new PHY_PerceptionRecoPoint( *this );
        activePerceptions_.push_back( pPerceptionRecoPoint_ );
    }

    return pPerceptionRecoPoint_->AddPoint( center, rSize, rSpeed, result );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRecoPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRecoPoint( void* pId )
{
    if ( !pPerceptionRecoPoint_ )
        return;

    pPerceptionRecoPoint_->RemovePoint( pId );
    if ( !pPerceptionRecoPoint_->HasPointToHandle() )
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
void* PHY_RolePion_Perceiver::EnableRecoObjects( const TER_Localisation& localisation, const MT_Vector2D& vCenter, MT_Float rSpeed, DIA_Variable_ABC& result )
{
    if ( !pPerceptionRecoObjects_ )
    {
        pPerceptionRecoObjects_ = new PHY_PerceptionRecoObjects( *this );
        activePerceptions_.push_back( pPerceptionRecoObjects_ );
    }
    
    return  pPerceptionRecoObjects_->AddLocalisation(localisation, vCenter, rSpeed, result );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRecoObjects
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRecoObjects( void* pId )
{
    if ( !pPerceptionRecoObjects_ )
        return;
        
    pPerceptionRecoObjects_->RemoveLocalisation( pId );
    if ( !pPerceptionRecoObjects_->HasLocalisationToHandle() )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionRecoObjects_ ) );
        delete pPerceptionRecoObjects_;
        pPerceptionRecoObjects_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRecoLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void* PHY_RolePion_Perceiver::EnableRecoLocalisation( const TER_Localisation& localisation, MT_Float rRadius )
{
    if ( !pPerceptionRecoLocalisation_ )
    {
        pPerceptionRecoLocalisation_ = new PHY_PerceptionRecoLocalisation( *this );
        activePerceptions_.push_back( pPerceptionRecoLocalisation_ );
    }

    return pPerceptionRecoLocalisation_->AddLocalisationWithRadius( localisation, rRadius );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRecoLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void* PHY_RolePion_Perceiver::EnableRecoLocalisation( const TER_Localisation& localisation )
{
    if ( !pPerceptionRecoLocalisation_ )
    {
        pPerceptionRecoLocalisation_ = new PHY_PerceptionRecoLocalisation( *this );
        activePerceptions_.push_back( pPerceptionRecoLocalisation_ );
    }

    return pPerceptionRecoLocalisation_->AddLocalisationWithDefaultRadius( localisation );    
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableControlLocalisation
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
void* PHY_RolePion_Perceiver::EnableControlLocalisation( const TER_Localisation& localisation )
{
    if ( !pPerceptionRecoLocalisation_ )
    {
        pPerceptionRecoLocalisation_ = new PHY_PerceptionRecoLocalisation( *this );
        activePerceptions_.push_back( pPerceptionRecoLocalisation_ );
    }

    return pPerceptionRecoLocalisation_->AddLocalisationWithoutRadius( localisation );
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRecoLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableRecoLocalisation( void* pId )
{
    if ( !pPerceptionRecoLocalisation_ )
        return;

    pPerceptionRecoLocalisation_->RemoveLocalisation( pId );
    if ( !pPerceptionRecoLocalisation_->HasLocalisationToHandle() )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionRecoLocalisation_ ) );
        delete pPerceptionRecoLocalisation_;
        pPerceptionRecoLocalisation_ = 0;
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
// Name: PHY_RolePion_Perceiver::EnableRadarOnLocalisation
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void* PHY_RolePion_Perceiver::EnableRadarOnLocalisation( const PHY_RadarClass& radarClass, const TER_Localisation& localisation )
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
void PHY_RolePion_Perceiver::DisableRadarOnLocalisation( const PHY_RadarClass& radarClass, void* pID )
{
   if( !pPerceptionRadar_ )
        return;

    pPerceptionRadar_->DisableRadar( radarClass, pID );
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
void* PHY_RolePion_Perceiver::EnableFlyingShellDetection( const TER_Localisation& localisation )
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
void PHY_RolePion_Perceiver::DisableFlyingShellDetection( void* pId )
{
    if ( !pPerceptionFlyingShell_ )
        return;

    pPerceptionFlyingShell_->RemoveLocalisation( pId );
    if ( !pPerceptionFlyingShell_->HasLocalisationToHandle() )
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
MT_Float PHY_RolePion_Perceiver::GetMaxAgentPerceptionDistance() const
{
    return    rMaxAgentPerceptionDistance_ 
            * GetRole< PHY_RolePion_Posture      >().GetElongationFactor() 
            * GetRole< PHY_RolePion_HumanFactors >().GetSensorDistanceModificator();;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetMaxObjectPerceptionDistance
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_Perceiver::GetMaxObjectPerceptionDistance() const
{
    return   rMaxObjectPerceptionDistance_             
           * GetRole< PHY_RolePion_Posture      >().GetElongationFactor() 
           * GetRole< PHY_RolePion_HumanFactors >().GetSensorDistanceModificator();;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::CanPerceive
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::CanPerceive() const
{
    assert( pPion_ );
    return !pPion_->IsDead() && !GetRole< PHY_RolePion_Transported >().IsTransported() && !GetRole< PHY_RolePion_Surrender >().IsSurrendered();
}

// =============================================================================
// UPDATE
// =============================================================================
struct sPerceptionRotation
{
    MT_COPYNOTALLOWED( sPerceptionRotation )

public:
    sPerceptionRotation() 
        : rAngle_ ( std::numeric_limits< MT_Float >::max()  ) 
    {
    }

    void operator () ( const PHY_ComposantePion& composante )
    {
        rAngle_ = std::min( rAngle_, composante.GetType().GetSensorRotationAngle() );
    }

    MT_Float GetAngle() const
    {
        return rAngle_ == std::numeric_limits< MT_Float >::max() ? 0. : rAngle_;
    }

private:
    MT_Float rAngle_;
};

// -----------------------------------------------------------------------------
struct sPerceptionDataSensors
{
    MT_COPYNOTALLOWED( sPerceptionDataSensors )

public:
    sPerceptionDataSensors( PHY_RolePion_Perceiver::T_SurfaceAgentMap& surfacesAgent, PHY_RolePion_Perceiver::T_SurfaceObjectMap& surfacesObject, const MT_Vector2D& vOrigin, const MT_Vector2D& vDirection, MT_Float& rMaxAgentPerceptionDistance, MT_Float& rMaxObjectPerceptionDistance )
        : surfacesAgent_               ( surfacesAgent  )
        , surfacesObject_              ( surfacesObject )
        , vOrigin_                     ( vOrigin    )
        , vDirection_                  ( vDirection )
        , rMaxAgentPerceptionDistance_ ( rMaxAgentPerceptionDistance  )
        , rMaxObjectPerceptionDistance_( rMaxObjectPerceptionDistance )
    {
    }

    void operator() ( const PHY_Sensor& sensor )
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
    MT_Float&                                   rMaxAgentPerceptionDistance_;
    MT_Float&                                   rMaxObjectPerceptionDistance_;
};

// -----------------------------------------------------------------------------
struct sPerceptionDataComposantes
{
    MT_COPYNOTALLOWED( sPerceptionDataComposantes )

public:
    sPerceptionDataComposantes( PHY_RolePion_Perceiver::T_SurfaceAgentMap& surfacesAgent, PHY_RolePion_Perceiver::T_SurfaceObjectMap& surfacesObject, const PHY_RolePion_Location& roleLocation, const MT_Vector2D& vMainPerceptionDirection, MT_Float rDirectionRotation, MT_Float& rMaxAgentPerceptionDistance, MT_Float& rMaxObjectPerceptionDistance )
        : surfacesAgent_               ( surfacesAgent  )
        , surfacesObject_              ( surfacesObject )
        , roleLocation_                ( roleLocation )
        , vMainPerceptionDirection_    ( vMainPerceptionDirection )
        , nRotationIdx_                ( vMainPerceptionDirection != roleLocation_.GetDirection() ? -1 : 0 ) // détection lockée
        , rRotationAngle_              ( rDirectionRotation )
        , rMaxAgentPerceptionDistance_ ( rMaxAgentPerceptionDistance )
        , rMaxObjectPerceptionDistance_( rMaxObjectPerceptionDistance )
    {
    }

    void operator() ( const PHY_ComposantePion& composante )
    {
        if( !composante.CanPerceive() )
            return;

        MT_Vector2D vComposantePerceptionDirection( vMainPerceptionDirection_ );

        if ( nRotationIdx_ == 0  )
            vComposantePerceptionDirection = roleLocation_.GetDirection();
        else if ( nRotationIdx_ > 0 )
            vComposantePerceptionDirection.Rotate( ( ( 2 * ( nRotationIdx_ & 0x1 ) - 1 ) * ( ( nRotationIdx_ + 1 ) >> 1 ) ) * rRotationAngle_ );
        ++nRotationIdx_;

        sPerceptionDataSensors dataFunctor( surfacesAgent_, surfacesObject_, roleLocation_.GetPosition(), vComposantePerceptionDirection, rMaxAgentPerceptionDistance_, rMaxObjectPerceptionDistance_ );
        composante.ApplyOnSensors( dataFunctor );
    }

private:
    PHY_RolePion_Perceiver::T_SurfaceAgentMap&  surfacesAgent_;
    PHY_RolePion_Perceiver::T_SurfaceObjectMap& surfacesObject_;
    const PHY_RolePion_Location&                roleLocation_;
    const MT_Vector2D&                          vMainPerceptionDirection_;
    MT_Float&                                   rMaxAgentPerceptionDistance_;
    MT_Float&                                   rMaxObjectPerceptionDistance_;
    int                                         nRotationIdx_;   
    const MT_Float                              rRotationAngle_;
};


// -----------------------------------------------------------------------------
struct sRadarDataComposantes
{
    MT_COPYNOTALLOWED( sRadarDataComposantes )

public:
    sRadarDataComposantes( PHY_RolePion_Perceiver::T_RadarsPerClassMap& radars )
        : radars_                      ( radars )
    {        
    }

    void operator() ( const PHY_ComposantePion& composante )
    {
        if( !composante.CanPerceive() )
            return;

        composante.ApplyOnRadars( *this );
    }

    void operator() ( const PHY_RadarType& radarType )
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
        vMainPerceptionDirection = GetRole< PHY_RolePion_Location >().GetDirection();
    else if( nSensorMode_ == eDirection )
        vMainPerceptionDirection = vSensorInfo_;
    else if( nSensorMode_ == ePoint )
    {
        const MT_Vector2D& vDirection = GetRole< PHY_RolePion_Location >().GetDirection();
        const MT_Vector2D& vPosition  = GetRole< PHY_RolePion_Location >().GetPosition ();
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
    PHY_RolePion_Location&    roleLocation    = GetRole< PHY_RolePion_Location    >();
    PHY_RolePion_Composantes& roleComposantes = GetRole< PHY_RolePion_Composantes >();
    if(    !roleLocation.HasLocationChanged() 
        && !roleComposantes.HasChanged()
        && !GetRole< PHY_RoleAction_Loading   >().HasChanged() 
        && !GetRole< PHY_RolePion_Transported >().HasChanged() 
        && !GetRole< PHY_RolePion_Surrender   >().HasChanged()
        && !HasChanged() )
        return;

    MT_Vector2D vMainPerceptionDirection;
    ComputeMainPerceptionDirection( vMainPerceptionDirection );
    
    surfacesAgent_ .clear();
    surfacesObject_.clear();
    rMaxAgentPerceptionDistance_  = 0; 
    rMaxObjectPerceptionDistance_ = 0;

    sPerceptionRotation rotation;
    roleComposantes.Apply( rotation );

    sPerceptionDataComposantes dataFunctor( surfacesAgent_, surfacesObject_, roleLocation, vMainPerceptionDirection, rotation.GetAngle() , rMaxAgentPerceptionDistance_, rMaxObjectPerceptionDistance_ );
    roleComposantes.Apply( dataFunctor );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::PrepareRadarData
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::PrepareRadarData()
{
    PHY_RolePion_Composantes& roleComposantes = GetRole< PHY_RolePion_Composantes >();
    if(    !roleComposantes.HasChanged()
        && !GetRole< PHY_RoleAction_Loading   >().HasChanged() 
        && !GetRole< PHY_RolePion_Transported >().HasChanged() 
        && !GetRole< PHY_RolePion_Surrender   >().HasChanged() )
        return;

    radars_.clear();

    sRadarDataComposantes dataFunctor( radars_ );
    roleComposantes.Apply( dataFunctor );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableAllPerceptions
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::DisableAllPerceptions()
{
    if( pPerceptionCoupDeSonde_ )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionCoupDeSonde_ ) );
        delete pPerceptionCoupDeSonde_;
        pPerceptionCoupDeSonde_ = 0;
    }

    if( pPerceptionRecoPoint_ )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionRecoPoint_ ) );
        delete pPerceptionRecoPoint_;
        pPerceptionRecoPoint_ = 0;
    }

    if( pPerceptionRecoLocalisation_ )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionRecoLocalisation_ ) );
        delete pPerceptionRecoLocalisation_;
        pPerceptionRecoLocalisation_ = 0;
    }

    if( pPerceptionRecoObjects_ )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionRecoObjects_ ) );
        delete pPerceptionRecoObjects_;
        pPerceptionRecoObjects_ = 0;
    }

    if( pPerceptionSurveillance_ )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionSurveillance_ ) );
        delete pPerceptionSurveillance_;
        pPerceptionSurveillance_ = 0;
    }

    if( pPerceptionRadar_ )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionRadar_ ) );
        delete pPerceptionRadar_;
        pPerceptionRadar_ = 0;
    }

    if( pPerceptionAlat_ )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionAlat_ ) );
        delete pPerceptionAlat_;
        pPerceptionAlat_ = 0;
    }

    if( pPerceptionFlyingShell_ )
    {
        activePerceptions_.erase( std::find( activePerceptions_.begin(), activePerceptions_.end(), pPerceptionFlyingShell_ ) );
        delete pPerceptionFlyingShell_;
        pPerceptionFlyingShell_ = 0;
    }
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::ExecutePerceptions
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::ExecutePerceptions() 
{
    if( CanPerceive() )
    {
        CIT_PerceptionVector itPerception;

        TER_Agent_ABC::T_AgentPtrVector perceivableAgents;
        TER_World::GetWorld().GetAgentManager().GetListWithinCircle( GetRole< PHY_RolePion_Location >().GetPosition(), GetMaxAgentPerceptionDistance(), perceivableAgents );
        for( itPerception = activePerceptions_.begin(); itPerception != activePerceptions_.end(); ++itPerception )
            (**itPerception).Execute( perceivableAgents );
        
        TER_Object_ABC::T_ObjectVector perceivableObjects;
        TER_World::GetWorld().GetObjectManager().GetListWithinCircle( GetRole< PHY_RolePion_Location >().GetPosition(), GetMaxObjectPerceptionDistance(), perceivableObjects );
        for( itPerception = activePerceptions_.begin(); itPerception != activePerceptions_.end(); ++itPerception )
            (**itPerception).Execute( perceivableObjects );

        TER_PopulationConcentration_ABC::T_PopulationConcentrationVector perceivableConcentrations;
        TER_World::GetWorld().GetPopulationManager().GetConcentrationManager().GetListWithinCircle( GetRole< PHY_RolePion_Location >().GetPosition(), GetMaxAgentPerceptionDistance(), perceivableConcentrations );
        for( itPerception = activePerceptions_.begin(); itPerception != activePerceptions_.end(); ++itPerception )
            (**itPerception).Execute( perceivableConcentrations );

        TER_PopulationFlow_ABC::T_PopulationFlowVector perceivableFlows;
        TER_World::GetWorld().GetPopulationManager().GetFlowManager().GetListWithinCircle( GetRole< PHY_RolePion_Location >().GetPosition(), GetMaxAgentPerceptionDistance(), perceivableFlows );
        for( itPerception = activePerceptions_.begin(); itPerception != activePerceptions_.end(); ++itPerception )
            (**itPerception).Execute( perceivableFlows );
    }

    assert( pPion_ );
    NotifyPerception( *pPion_, PHY_PerceptionLevel::identified_, false );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_RolePion_Perceiver::ComputePerception( const DEC_Knowledge_Agent& knowledge ) const
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
    const uint nCurrentTime = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
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

    if ( pPerceptionRecoPoint_ )
        pPerceptionRecoPoint_->Update();
        
    if ( pPerceptionRecoObjects_ )
        pPerceptionRecoObjects_->Update();
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetArmy
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
const MIL_Army& PHY_RolePion_Perceiver::GetArmy() const
{
    assert( pPion_ );
    return pPion_->GetArmy();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetKnowledgeGroup
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
const MIL_KnowledgeGroup& PHY_RolePion_Perceiver::GetKnowledgeGroup() const
{
    assert( pPion_ );
    return pPion_->GetKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsKnown
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsKnown( const MIL_Agent_ABC& agent ) const
{
    assert( pPion_ );
    return pPion_->GetKnowledgeGroup().GetKnowledge().IsKnown( agent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsIdentified
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsIdentified( const MIL_Agent_ABC& agent ) const
{
    assert( pPion_ );
    return pPion_->GetKnowledge().IsIdentified( agent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::WasPerceived
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::WasPerceived( const MIL_Agent_ABC& agent  ) const
{
    assert( pPion_ );
    return pPion_->GetKnowledge().WasPerceived( agent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsKnown
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsKnown( const MIL_RealObject_ABC& object ) const
{
    assert( pPion_ );
    return pPion_->GetArmy().GetKnowledge().IsKnown( object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsIdentified
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsIdentified( const MIL_RealObject_ABC& object ) const
{
    assert( pPion_ );
    return pPion_->GetKnowledge().IsIdentified( object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsIdentified
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::IsIdentified( const MIL_PopulationConcentration& concentration ) const
{
    assert( pPion_ );
    return pPion_->GetKnowledge().IsIdentified( concentration );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerception( const MIL_Effect_IndirectFire& flyingShell ) const
{
    assert( pPion_ );
    MIL_RC::pRcObservationTirIndirect_->Send( *pPion_, MIL_RC::eRcTypeOperational, flyingShell );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level, bool bPerceptionRecorded )
{
    assert( pPion_ );
    pPion_->GetKnowledge().GetKsPerception().NotifyPerception( agent, level, bPerceptionRecorded );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level )
{
    assert( pPion_ );
    pPion_->GetKnowledge().GetKsPerception().NotifyPerception( agent, level, bRecordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerception( MIL_RealObject_ABC& object, const PHY_PerceptionLevel& level )
{
    assert( pPion_ );
    pPion_->GetKnowledge().GetKsPerception().NotifyPerception( object, level, bRecordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerception( MIL_PopulationConcentration& concentration, const PHY_PerceptionLevel& level )
{
    assert( pPion_ );
    pPion_->GetKnowledge().GetKsPerception().NotifyPerception( concentration, level, bRecordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyPerception( MIL_PopulationFlow& flow, const PHY_PerceptionLevel& level, const T_PointVector& shape )
{
    assert( pPion_ );
    pPion_->GetKnowledge().GetKsPerception().NotifyPerception( flow, level, shape, bRecordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifytExternalPerception
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::NotifyExternalPerception( MIL_Agent_ABC& agent , const PHY_PerceptionLevel& level )
{
    assert( pPion_ );
    pPion_->GetKnowledge().GetKsPerception().NotifyExternalPerception( agent, level );
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
    
    assert( pPion_ );
    pPion_->GetKnowledge().GetKsPerception().MakePerceptionsAvailable();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::HasDelayedPerceptions
// Created: JVT 2004-12-03
// -----------------------------------------------------------------------------
bool PHY_RolePion_Perceiver::HasDelayedPerceptions() const
{
    assert( pPion_ );
    return pPion_->GetKnowledge().GetKsPerception().HasDelayedPerceptions();
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
    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN::DIN_BufferedMessage msg = msgMgr.BuildMessage();
    
    assert( pPion_ );
    msg << (uint32)pPion_->GetID();
    msg << (uint32)surfacesAgent_.size();
    for( CIT_SurfaceAgentMap it = surfacesAgent_.begin(); it != surfacesAgent_.end(); ++it )
        it->second.SendFullState( msg );
    msg << GetRole< PHY_RolePion_Posture >().GetElongationFactor();
    msgMgr.SendMsgUnitVisionCones( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg.GetAsnMsg().m.radar_actifPresent = 1;
    msg.GetAsnMsg().radar_actif          = IsUsingActiveRadar();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Perceiver::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
{
    if( bRadarStateHasChanged_ )
        SendFullState( msg );
}
