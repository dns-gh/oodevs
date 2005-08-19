// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Radars/PHY_RadarType.cpp $
// $Author: Nld $
// $Modtime: 3/06/05 18:20 $
// $Revision: 2 $
// $Workfile: PHY_RadarType.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_RadarType.h"

#include "PHY_RadarClass.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Tools/MIL_Tools.h"

PHY_RadarType::T_RadarTypeMap PHY_RadarType::radarTypes_;
uint                          PHY_RadarType::nNextID_ = 0;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::Initialize
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_RadarType::Initialize( MIL_InputArchive& archive )
{
    archive.BeginList( "Radars" );

    while ( archive.NextListElement() )
    {
        archive.Section( "Radar" );

        std::string strRadarName;
        archive.ReadAttribute( "nom", strRadarName );

        std::string strType;
        archive.ReadAttribute( "type", strType );
        const PHY_RadarClass* pType = PHY_RadarClass::Find( strType );
        if( !pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown radar type", archive.GetContext() );

        const PHY_RadarType*& pRadarType = radarTypes_[ strRadarName ];
        if( pRadarType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Radar already exists", archive.GetContext() );
        pRadarType = new PHY_RadarType( strRadarName, *pType, archive );

        archive.EndSection(); // Senseur
    }

    archive.EndList(); // Senseurs
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::Terminate
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_RadarType::Terminate()
{
    for( CIT_RadarTypeMap it = radarTypes_.begin(); it != radarTypes_.end(); ++it )
        delete it->second;
    radarTypes_.clear();
}

// =============================================================================
// INIT
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RadarType constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_RadarType::PHY_RadarType( const std::string& strName, const PHY_RadarClass& radarClass, MIL_InputArchive& archive )
    : nID_                  ( nNextID_++ )
    , strName_              ( strName )
    , class_                ( radarClass )
    , rRadius_              ( 0. )
    , rMinHeight_           ( -std::numeric_limits< MT_Float >::max() )
    , rMaxHeight_           (  std::numeric_limits< MT_Float >::max() )
    , detectableActivities_ ( PHY_ConsumptionType::GetConsumptionTypes().size(), false )
    , rDetectionTime_       ( std::numeric_limits< MT_Float >::max() )
    , rRecognitionTime_     ( std::numeric_limits< MT_Float >::max() )
    , rIdentificationTime_  ( std::numeric_limits< MT_Float >::max() )
    , rPcDetectionTime_     ( std::numeric_limits< MT_Float >::max() )
    , rPcRecognitionTime_   ( std::numeric_limits< MT_Float >::max() )
    , rPcIdentificationTime_( std::numeric_limits< MT_Float >::max() )
{
    InitializeRange           ( archive );
    InitializeActivities      ( archive );
    InitializeAcquisitionTimes( archive );
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType destructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_RadarType::~PHY_RadarType()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::InitializeRange
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_RadarType::InitializeRange( MIL_InputArchive& archive )
{
    archive.Section( "Portee" );

    archive.ReadField( "RayonAction", rRadius_, CheckValueGreaterOrEqual( 0. ) );
    rRadius_ = MIL_Tools::ConvertMeterToSim( rRadius_ );

    archive.ReadField( "HauteurMinimale", rMinHeight_, CheckValueGreaterOrEqual( 0.          ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing );
    archive.ReadField( "HauteurMaximale", rMaxHeight_, CheckValueGreaterOrEqual( rMinHeight_ ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing );

    archive.EndSection(); // Portee
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::InitializeActivities
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_RadarType::InitializeActivities( MIL_InputArchive& archive )
{
    if( !archive.Section( "ActivitePionsDetectables", MIL_InputArchive::eNothing ) )
    {
        std::fill( detectableActivities_.begin(), detectableActivities_.end(), true );
        return;
    }

    std::fill( detectableActivities_.begin(), detectableActivities_.end(), false );

    const PHY_ConsumptionType::T_ConsumptionTypeMap& consumptionTypes = PHY_ConsumptionType::GetConsumptionTypes();
    for( PHY_ConsumptionType::CIT_ConsumptionTypeMap it = consumptionTypes.begin(); it != consumptionTypes.end(); ++it )
    {
        const PHY_ConsumptionType& conso = *it->second;
        if ( archive.Section( conso.GetName(), MIL_InputArchive::eNothing ) )
        {
            detectableActivities_[ conso.GetID() ] = true;
            archive.EndSection();
        }
    }
    archive.EndSection(); // ActivitePionsDetectables
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::InitializeAcquisitionTimes
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_RadarType::InitializeAcquisitionTimes( MIL_InputArchive& archive )
{
    if( !archive.Section( "DureesAcquisition", MIL_InputArchive::eNothing ) )
    {
        rPcDetectionTime_       = rDetectionTime_       = std::numeric_limits< MT_Float >::max();
        rPcRecognitionTime_     = rRecognitionTime_     = 0;
        rPcIdentificationTime_  = rIdentificationTime_  = std::numeric_limits< MT_Float >::max();
        return;
    }
   
    MT_Float rMinBound = 0.;
    if( archive.ReadTimeField( "DureeDetection", rDetectionTime_, CheckValueGreaterOrEqual( 0. ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing ) ) 
        rMinBound = rDetectionTime_;   
    if( archive.ReadTimeField( "DureeReconnaissance", rRecognitionTime_, CheckValueGreaterOrEqual( rMinBound ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing ) )
        rMinBound = rRecognitionTime_;
    archive.ReadTimeField( "DureeIdentification", rIdentificationTime_, CheckValueGreaterOrEqual( rMinBound ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing );

    if( rDetectionTime_ != std::numeric_limits< MT_Float >::max() )
        rDetectionTime_ = MIL_Tools::ConvertSecondsToSim( rDetectionTime_ );
    if( rRecognitionTime_ != std::numeric_limits< MT_Float >::max() )
        rRecognitionTime_ = MIL_Tools::ConvertSecondsToSim( rRecognitionTime_ );
    if( rIdentificationTime_ != std::numeric_limits< MT_Float >::max() )
        rIdentificationTime_ = MIL_Tools::ConvertSecondsToSim( rIdentificationTime_ );

    if( !archive.Section( "DureesSpecifiquesPionPC", MIL_InputArchive::eNothing ) )
    {
        rPcDetectionTime_       = rDetectionTime_;
        rPcRecognitionTime_     = rRecognitionTime_;
        rPcIdentificationTime_  = rIdentificationTime_;
    }
    else
    {
        rPcDetectionTime_       = std::numeric_limits< MT_Float >::max();
        rPcRecognitionTime_     = std::numeric_limits< MT_Float >::max();
        rPcIdentificationTime_  = std::numeric_limits< MT_Float >::max();

        MT_Float rMinBound = 0.;
        if( archive.ReadTimeField( "DureeDetection", rPcDetectionTime_, CheckValueGreaterOrEqual( 0. ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing ) ) 
            rMinBound = rPcDetectionTime_;   
        if( archive.ReadTimeField( "DureeReconnaissance", rPcRecognitionTime_, CheckValueGreaterOrEqual( rMinBound ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing ) )
            rMinBound = rPcRecognitionTime_;
        archive.ReadTimeField( "DureeIdentification", rPcIdentificationTime_, CheckValueGreaterOrEqual( rMinBound ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing );

        if( rPcDetectionTime_ != std::numeric_limits< MT_Float >::max() )
            rPcDetectionTime_ = MIL_Tools::ConvertSecondsToSim( rPcDetectionTime_ );
        if( rPcRecognitionTime_ != std::numeric_limits< MT_Float >::max() )
            rPcRecognitionTime_ = MIL_Tools::ConvertSecondsToSim( rPcRecognitionTime_ );
        if( rPcIdentificationTime_ != std::numeric_limits< MT_Float >::max() )
            rPcIdentificationTime_ = MIL_Tools::ConvertSecondsToSim( rPcIdentificationTime_ );
        archive.EndSection(); // DureesSpecifiquesPionPC
    }

    archive.EndSection(); // DureesAcquisition
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::CanAcquire
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
bool PHY_RadarType::CanAcquire( const MIL_AgentPion& perceiver, const MIL_Agent_ABC& target ) const
{
    const PHY_RolePion_Location&      roleLocationPerceiver = perceiver.GetRole< PHY_RolePion_Location      >();
    const PHY_RoleInterface_Location& roleLocationTarget    = target   .GetRole< PHY_RoleInterface_Location >();

    if( roleLocationPerceiver.GetPosition().Distance( roleLocationTarget.GetPosition() ) > rRadius_ )
        return false;

    if( !target.GetRole< PHY_RoleInterface_Posture >().CanBePerceived( perceiver ) )
        return false;

    if ( !detectableActivities_[ target.GetRole< PHY_RoleInterface_Dotations >().GetConsumptionMode().GetID() ] )
        return false;

    const MT_Float rTargetHeight = target.GetRole< PHY_RoleInterface_Location >().GetHeight();
    if( rTargetHeight > rMaxHeight_ || rTargetHeight < rMinHeight_ )
        return false;

    if( class_ == PHY_RadarClass::tapping_ && !target.GetRole< PHY_RoleInterface_Communications >().CanCommunicate() )
        return false;

    if( class_ == PHY_RadarClass::tappingRadar_ && !target.GetRole< PHY_RoleInterface_Perceiver >().IsUsingActiveRadar() )
        return false;

    return true;
}
 
// -----------------------------------------------------------------------------
// Name: PHY_RadarType::ComputeAcquisitionLevel
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_RadarType::ComputeAcquisitionLevel( const MIL_Agent_ABC& target, uint nFirstAcquisitionTimeStep ) const
{
    const uint nTimePerceived = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() - nFirstAcquisitionTimeStep;
    if( target.IsPC() )
    {
        if( nTimePerceived >= rPcIdentificationTime_ )
            return PHY_PerceptionLevel::identified_;
        if( nTimePerceived >= rPcRecognitionTime_ )
            return PHY_PerceptionLevel::recognized_;
        if( nTimePerceived >= rPcDetectionTime_ )
            return PHY_PerceptionLevel::detected_;
        return PHY_PerceptionLevel::notSeen_;
    }
    else 
    {
        if( nTimePerceived >= rIdentificationTime_ )
            return PHY_PerceptionLevel::identified_;
        if( nTimePerceived >= rRecognitionTime_ )
            return PHY_PerceptionLevel::recognized_;
        if( nTimePerceived >= rDetectionTime_ )
            return PHY_PerceptionLevel::detected_;
        return PHY_PerceptionLevel::notSeen_;
    }
}
