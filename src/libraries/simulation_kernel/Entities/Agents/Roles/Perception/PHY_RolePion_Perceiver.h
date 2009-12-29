// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h $
// $Author: Jvt $
// $Modtime: 24/05/05 10:18 $
// $Revision: 24 $
// $Workfile: PHY_RolePion_Perceiver.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Perceiver_h_
#define __PHY_RolePion_Perceiver_h_

#include "PHY_RoleInterface_Perceiver.h"

#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "ComponentsChangedNotificationHandler_ABC.h"
#include "simulation_kernel/SurrenderNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkUnitMessageNotificationHandler_ABC.h"
#include "simulation_kernel/VisionConeNotificationHandler_ABC.h"
#include "simulation_kernel/TransportChangeNotificationHandler_ABC.h"
#include "simulation_kernel/LoadingChangeNotificationHandler_ABC.h"

class PHY_Perception_ABC;
class PHY_PerceptionView;
class PHY_PerceptionCoupDeSonde;
class PHY_PerceptionRecoPoint;
class PHY_PerceptionRecoLocalisation;
class PHY_PerceptionRecoObjects;
class PHY_PerceptionRecoSurveillance;
class PHY_PerceptionRadar;
class PHY_PerceptionAlat;
class PHY_PerceptionFlyingShell;

namespace detection
{
    class DetectionComputer_ABC;
}
// =============================================================================
// @class  PHY_RolePion_Perceiver
// Created: JVT 2004-08-03
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_RolePion_Perceiver : public PHY_RoleInterface_Perceiver
                             , public tools::AlgorithmModifier_ABC< detection::DetectionComputer_ABC >
                             , public component::ComponentsChangedNotificationHandler_ABC
                             , public surrender::SurrenderNotificationHandler_ABC
                             , public network::NetworkUnitMessageNotificationHandler_ABC
                             , public network::VisionConeNotificationHandler_ABC
                             , public transport::TransportChangeNotificationHandler_ABC
                             , public transport::LoadingChangeNotificationHandler_ABC
{

public:
    //! @name Types
    //@{
    typedef std::pair< const PHY_SensorTypeAgent* , MT_Float /*height*/ > T_SurfaceAgentKeyPair; 
    typedef std::map< T_SurfaceAgentKeyPair, PHY_PerceptionSurfaceAgent > T_SurfaceAgentMap;
    typedef T_SurfaceAgentMap::const_iterator                             CIT_SurfaceAgentMap;

    typedef std::pair< const PHY_SensorTypeObject* , MT_Float /*height*/ >     T_SurfaceObjectKeyPair; 
    typedef std::map< T_SurfaceObjectKeyPair, PHY_PerceptionSurfaceObject >    T_SurfaceObjectMap;
    typedef T_SurfaceObjectMap::const_iterator                                 CIT_SurfaceObjectMap;

    typedef std::set< const PHY_RadarType* > T_RadarSet;
    typedef T_RadarSet::const_iterator       CIT_RadarSet;

    typedef std::map< const PHY_RadarClass*, T_RadarSet > T_RadarsPerClassMap;
    typedef T_RadarsPerClassMap::const_iterator           CIT_RadarsPerClassMap;
    //@}

public:
    explicit PHY_RolePion_Perceiver( MIL_AgentPion& pion );
    virtual ~PHY_RolePion_Perceiver();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    void Initialization( const MT_Vector2D& perceiverPosition, const MT_Vector2D& perceiverDirection );

    //@{
    virtual void NotifyExternalPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level );
    void NotifyPerception( MIL_Agent_ABC&               agent        , const PHY_PerceptionLevel& level, bool bPerceptionRecorded );
    void NotifyPerception( MIL_Agent_ABC&               agent        , const PHY_PerceptionLevel& level );
    void NotifyPerception( MIL_Object_ABC&              object       , const PHY_PerceptionLevel& level );
    void NotifyPerception( MIL_PopulationConcentration& concentration, const PHY_PerceptionLevel& level );
    void NotifyPerception( MIL_PopulationFlow&          flow         , const PHY_PerceptionLevel& level, const T_PointVector& shape );
    void NotifyPerception( const MIL_Effect_IndirectFire& flyingShell ) const;
    void NotifyPerception( const urban::TerrainObject_ABC& block, const PHY_PerceptionLevel& level ) const;    
    //@}

    //! @name Operations
    //@{    
    void                       Update            ( bool bIsDead );
    void                       Clean             ();
    void                       ExecutePerceptions();
    const PHY_PerceptionLevel& ComputePerception ( const MT_Vector2D& vPoint             ) const;
    const PHY_PerceptionLevel& ComputePerception ( const DEC_Knowledge_Object& knowledge ) const;
    const PHY_PerceptionLevel& ComputePerception ( const DEC_Knowledge_Agent & knowledge ) const;
    const PHY_PerceptionLevel& ComputePerception ( const urban::Block& block ) const;

    void SetVisionModeNormal   ();
    void SetVisionModeDirection( const MT_Vector2D& vDirection );
    void SetVisionModePoint    ( const MT_Vector2D& vPoint );
    virtual bool IsUsingActiveRadar  () const;

    virtual void Execute( detection::DetectionComputer_ABC& algorithm ) const;
    virtual void NotifyComponentHasChanged();
    virtual void NotifyCaptured();
    virtual void NotifyReleased();
    virtual void NotifyVisionConeDataHasChanged();
    virtual void NotifyIsLoadedForTransport();
    virtual void NotifyIsUnLoadedForTransport();
    virtual void NotifyIsLoadedInVab();
    virtual void NotifyIsUnLoadedInVab();
    //@}

    //! @name Perceptions
    //@{
    void EnableSensors ();
    void DisableSensors();

    bool  HasDelayedPerceptions          () const;
    void  EnableRecoAlat                 ( const TER_Localisation& localisation );
    void  DisableRecoAlat                ();
    void  EnableCoupDeSonde              ();
    void  DisableCoupDeSonde             ();
    int   EnableRecoPoint                ( const MT_Vector2D& center, MT_Float rSize, MT_Float rSpeed, DEC_Decision_ABC& callerAgent );
    void  DisableRecoPoint               ( int );
    int   EnableRecoLocalisation         ( const TER_Localisation& localisation, MT_Float rRadius );
    int   EnableRecoLocalisation         ( const TER_Localisation& localisation );
    int   EnableControlLocalisation      ( const TER_Localisation& localisation );
    void  DisableRecoLocalisation        ( int );
    int   EnableSurveillanceLocalisation ( const TER_Localisation& localisation );
    void  DisableSurveillanceLocalisation( int );
    int   EnableRecoObjects              ( const TER_Localisation& localisation, const MT_Vector2D& vCenter, MT_Float rSpeed, DEC_Decision_ABC& callerAgent );
    void  DisableRecoObjects             ( int );

    // Radars
    int   EnableRadarOnLocalisation      ( const PHY_RadarClass& radarClass, const TER_Localisation& localisation );
    void  DisableRadarOnLocalisation     ( const PHY_RadarClass& radarClass, int );
    void  EnableRadar                    ( const PHY_RadarClass& radarClass );
    void  DisableRadar                   ( const PHY_RadarClass& radarClass );

    int   EnableFlyingShellDetection     ( const TER_Localisation& localisation );
    void  DisableFlyingShellDetection    ( int );

    // Rens
    void  EnableRecordMode ();
    void  DisableRecordMode();

    void DisableAllPerceptions();
    //@}

    //! @name Accessors
    //@{
    const T_SurfaceAgentMap&  GetSurfacesAgent () const;
    const T_SurfaceObjectMap& GetSurfacesObject() const;
    const T_RadarSet&         GetRadars        ( const PHY_RadarClass& radarClass );
    //@}

    //! @name Tools
    //@{
    const MIL_KnowledgeGroup& GetKnowledgeGroup            () const;
          MIL_AgentPion&      GetPion                      () const;
          MT_Float            GetMaxAgentPerceptionDistance() const;
          void                GetMainPerceptionDirection   ( MT_Vector2D& vDirection ) const;

    bool IsPeriphericalVisionEnabled() const;
    bool IsKnown                    ( const MIL_Agent_ABC&               agent         ) const;
    bool IsIdentified               ( const MIL_Agent_ABC&               agent         ) const;
    bool IsKnown                    ( const MIL_Object_ABC&              object        ) const;
    bool IsIdentified               ( const MIL_Object_ABC&              object        ) const;
    bool IsIdentified               ( const MIL_PopulationConcentration& concentration ) const;
    bool IsIdentified               ( const urban::TerrainObject_ABC&    object        ) const;
    //@}

    //! @name Network
    //@{
    void SendDebugState  () const;
    void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const;
    void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_SensorMode
    {
        eNormal,    // la direction de détection est déterminée en fonction de la direction de déplacement
        eDirection, // la direction de détection est fixe
        ePoint      // la détection est fixées sur un point précis
    };

    typedef std::vector< PHY_Perception_ABC* > T_PerceptionVector;
    typedef T_PerceptionVector::const_iterator IT_PerceptionVector;
    typedef T_PerceptionVector::const_iterator CIT_PerceptionVector;
    //@}

private:
    //! @name Tools
    //@{
    bool HasChanged() const;
    bool HasRadarStateChanged() const;
    bool CanPerceive                   () const;
    void PreparePerceptionData         ();
    void PrepareRadarData              ();
    void UpdatePeriphericalVisionState ();
    void ComputeMainPerceptionDirection( MT_Vector2D& vMainPerceptionDirection ) const;

    MT_Float    GetMaxObjectPerceptionDistance() const;
    
    void EnsurePerceptionRecoLocalisation();
    //@}

private:
    MIL_AgentPion& pion_;
    const MT_Vector2D* perceiverPosition_;
    const MT_Vector2D* perceiverDirection_;
    
    MT_Vector2D lastPerceiverPosition_;

    bool           bPeriphericalVisionEnabled_;
    uint           nNextPeriphericalVisionStep_;
    bool           bRecordModeEnabled_;
    
    // Perceptions
    T_RadarsPerClassMap radars_;
    T_SurfaceAgentMap   surfacesAgent_;
    T_SurfaceObjectMap  surfacesObject_;
    MT_Float            rMaxAgentPerceptionDistance_;
    MT_Float            rMaxObjectPerceptionDistance_;   

    PHY_PerceptionView*             pPerceptionView_;
    PHY_PerceptionCoupDeSonde*      pPerceptionCoupDeSonde_;
    PHY_PerceptionRecoPoint*        pPerceptionRecoPoint_;
    PHY_PerceptionRecoLocalisation* pPerceptionRecoLocalisation_;
    PHY_PerceptionRecoObjects*      pPerceptionRecoObjects_;
    PHY_PerceptionRecoSurveillance* pPerceptionSurveillance_;
    PHY_PerceptionRadar*            pPerceptionRadar_;
    PHY_PerceptionAlat*             pPerceptionAlat_;
    PHY_PerceptionFlyingShell*      pPerceptionFlyingShell_;
    T_PerceptionVector              activePerceptions_;

    E_SensorMode nSensorMode_;
    MT_Vector2D  vSensorInfo_;

    bool bHasChanged_;
    bool bExternalMustChangePerception_;
    bool bExternalMustChangeRadar_;
    bool bExternalCanPerceive_;
    bool bExternalMustUpdateVisionCones_;
    bool bRadarStateHasChanged_;

private:
    static const uint nNbrStepsBetweenPeriphericalVision_;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_Perceiver* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_Perceiver* role, const unsigned int /*version*/ );
};

#endif // __PHY_RolePion_Perceiver_h_
