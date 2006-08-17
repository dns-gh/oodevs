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

#include "MIL.h"

#include "PHY_RoleInterface_Perceiver.h"

#include "Entities/Agents/Perceptions/PHY_PerceptionSurfaceAgent.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionSurfaceObject.h"

class DEC_Knowledge_Object;
class DEC_Knowledge_Agent;
class PHY_PerceptionLevel;
class PHY_PerceptionView;
class PHY_Perception_ABC;
class PHY_PerceptionCoupDeSonde;
class PHY_PerceptionRecoPoint;
class PHY_PerceptionRecoLocalisation;
class PHY_PerceptionRecoSurveillance;
class PHY_PerceptionRecoObjects;
class PHY_PerceptionAlat;
class PHY_PerceptionFlyingShell;
class PHY_PerceptionRadar;
class PHY_RadarClass;
class PHY_RadarType;
class MIL_Effect_IndirectFire;
class MIL_Agent_ABC;
class MIL_Army;
class MIL_KnowledgeGroup;
class MIL_AgentPion;
class NET_ASN_MsgUnitAttributes;

// =============================================================================
// @class  PHY_RolePion_Perceiver
// Created: JVT 2004-08-03
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_RolePion_Perceiver : public PHY_RoleInterface_Perceiver
{
    MT_COPYNOTALLOWED( PHY_RolePion_Perceiver )

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
             PHY_RolePion_Perceiver( MT_RoleContainer& role, MIL_AgentPion& pion );
             PHY_RolePion_Perceiver();
    virtual ~PHY_RolePion_Perceiver();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //@{
    void NotifyExternalPerception( MIL_Agent_ABC&               agent        , const PHY_PerceptionLevel& level );
    void NotifyPerception        ( MIL_Agent_ABC&               agent        , const PHY_PerceptionLevel& level, bool bPerceptionRecorded );
    void NotifyPerception        ( MIL_Agent_ABC&               agent        , const PHY_PerceptionLevel& level );
    void NotifyPerception        ( MIL_RealObject_ABC&          object       , const PHY_PerceptionLevel& level );
    void NotifyPerception        ( MIL_PopulationConcentration& concentration, const PHY_PerceptionLevel& level );
    void NotifyPerception        ( MIL_PopulationFlow&          flow         , const PHY_PerceptionLevel& level, const T_PointVector& shape );
    void NotifyPerception        ( const MIL_Effect_IndirectFire& flyingShell ) const;    
    //@}

    //! @name Operations
    //@{    
    bool                       HasChanged        () const;
    void                       Update            ( bool bIsDead );
    void                       Clean             ();
    void                       ExecutePerceptions();
    const PHY_PerceptionLevel& ComputePerception ( const MT_Vector2D& vPoint             ) const;
    const PHY_PerceptionLevel& ComputePerception ( const DEC_Knowledge_Object& knowledge ) const;
    const PHY_PerceptionLevel& ComputePerception ( const DEC_Knowledge_Agent & knowledge ) const;

    void SetVisionModeNormal   ();
    void SetVisionModeDirection( const MT_Vector2D& vDirection );
    void SetVisionModePoint    ( const MT_Vector2D& vPoint );

            bool HasRadarStateChanged() const;
    virtual bool IsUsingActiveRadar  () const;
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
    void* EnableRecoPoint                ( const MT_Vector2D& center, MT_Float rSize, MT_Float rSpeed, DIA_Variable_ABC& result );
    void  DisableRecoPoint               ( void* );
    void* EnableRecoLocalisation         ( const TER_Localisation& localisation, MT_Float rRadius );
    void* EnableRecoLocalisation         ( const TER_Localisation& localisation );
    void* EnableControlLocalisation      ( const TER_Localisation& localisation );
    void  DisableRecoLocalisation        ( void* );
    void* EnableSurveillanceLocalisation ( const TER_Localisation& localisation );
    void  DisableSurveillanceLocalisation( void* );
    void* EnableRecoObjects              ( const TER_Localisation& localisation, const MT_Vector2D& vCenter, MT_Float rSpeed, DIA_Variable_ABC& result );
    void  DisableRecoObjects             ( void* );

    // Radars
    void* EnableRadarOnLocalisation      ( const PHY_RadarClass& radarClass, const TER_Localisation& localisation );
    void  DisableRadarOnLocalisation     ( const PHY_RadarClass& radarClass, void* );
    void  EnableRadar                    ( const PHY_RadarClass& radarClass );
    void  DisableRadar                   ( const PHY_RadarClass& radarClass );

    void* EnableFlyingShellDetection     ( const TER_Localisation& localisation );
    void  DisableFlyingShellDetection    ( void* );

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
    const MIL_Army&           GetArmy                      () const;
    const MIL_KnowledgeGroup& GetKnowledgeGroup            () const;
    const MIL_AgentPion&      GetPion                      () const;
          MT_Float            GetMaxAgentPerceptionDistance() const;
          void                GetMainPerceptionDirection   ( MT_Vector2D& vDirection ) const;

    bool IsPeriphericalVisionEnabled() const;
    bool IsKnown                    ( const MIL_Agent_ABC&               agent         ) const;
    bool IsIdentified               ( const MIL_Agent_ABC&               agent         ) const;
    bool WasPerceived               ( const MIL_Agent_ABC&               agent         ) const;
    bool IsKnown                    ( const MIL_RealObject_ABC&          object        ) const;
    bool IsIdentified               ( const MIL_RealObject_ABC&          object        ) const;
    bool IsIdentified               ( const MIL_PopulationConcentration& concentration ) const;
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
    bool CanPerceive                   () const;
    void PreparePerceptionData         ();
    void PrepareRadarData              ();
    void UpdatePeriphericalVisionState ();
    void ComputeMainPerceptionDirection( MT_Vector2D& vMainPerceptionDirection ) const;

    MT_Float    GetMaxObjectPerceptionDistance() const;
    //@}

private:
    MIL_AgentPion* pPion_;
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
    bool bRadarStateHasChanged_;

private:
    static const uint nNbrStepsBetweenPeriphericalVision_;
};

#include "PHY_RolePion_Perceiver.inl"

#endif // __PHY_RolePion_Perceiver_h_
