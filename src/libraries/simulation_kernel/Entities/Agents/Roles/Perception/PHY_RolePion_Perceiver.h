// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RolePion_Perceiver_h_
#define __PHY_RolePion_Perceiver_h_

#include "PHY_RoleInterface_Perceiver.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "ComponentsChangedNotificationHandler_ABC.h"
#include "simulation_kernel/SurrenderNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkUnitAttributesMessageSender_ABC.h"
#include "simulation_kernel/VisionConeNotificationHandler_ABC.h"
#include "simulation_kernel/TransportChangeNotificationHandler_ABC.h"
#include "simulation_kernel/LoadingChangeNotificationHandler_ABC.h"

class MIL_UrbanObject_ABC;
class PHY_Perception_ABC;
class PHY_PerceptionView;
class PHY_PerceptionCoupDeSonde;
class PHY_PerceptionRecoPoint;
class PHY_PerceptionRecoLocalisation;
class PHY_PerceptionRecoObjects;
class PHY_PerceptionRecoSurveillance;
class PHY_PerceptionRecoUrbanBlock;
class PHY_PerceptionRadar;
class PHY_PerceptionAlat;
class PHY_PerceptionFlyingShell;
class PHY_SensorTypeDisaster;
class TER_Agent_ABC;
class TER_Object_ABC;
class TER_PopulationConcentration_ABC;
class TER_PopulationFlow_ABC;

namespace detection
{
    class DetectionComputer;
}

namespace dotation
{
    class ConsumptionComputer_ABC;
}

// =============================================================================
// @class  PHY_RolePion_Perceiver
// Created: JVT 2004-08-03
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_RolePion_Perceiver : public PHY_RoleInterface_Perceiver
                             , public tools::AlgorithmModifier_ABC< detection::DetectionComputer >
                             , public tools::AlgorithmModifier_ABC< dotation::ConsumptionComputer_ABC >
                             , public component::ComponentsChangedNotificationHandler_ABC
                             , public surrender::SurrenderNotificationHandler_ABC
                             , public network::NetworkUnitAttributesMessageSender_ABC
                             , public network::VisionConeNotificationHandler_ABC
                             , public transport::TransportChangeNotificationHandler_ABC
                             , public transport::LoadingChangeNotificationHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_RolePion_Perceiver();
    explicit PHY_RolePion_Perceiver( MIL_Agent_ABC& pion );
    virtual ~PHY_RolePion_Perceiver();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //@{
    virtual void NotifyExternalPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level );
    virtual void NotifyPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level, bool bPerceptionRecorded );
    virtual bool NotifyPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level );
    virtual void NotifyPerception( MIL_Object_ABC& object, const PHY_PerceptionLevel& level );
    virtual bool NotifyPerception( MIL_PopulationConcentration& concentration, const PHY_PerceptionLevel& level );
    virtual bool NotifyPerception( MIL_PopulationFlow& flow, const PHY_PerceptionLevel& level, const T_PointVector& shape );
    virtual bool NotifyPerception( const MIL_Effect_IndirectFire& flyingShell ) const;
    virtual void NotifyPerceptionUrban( const MIL_UrbanObject_ABC& block, const PHY_PerceptionLevel& level ) const;
    virtual void NotifyPerception( MIL_Object_ABC& object, const MT_Vector2D& vPosition, const MT_Vector2D& vDirection );
    //@}

    //! @name Operations
    //@{
    virtual void                       Update            ( bool bIsDead );
    virtual void                       Clean             ();
    virtual void                       ExecutePerceptions();
    virtual void                       ExecuteCollisions();
    virtual const PHY_PerceptionLevel& ComputePerception ( const MT_Vector2D& vPoint             ) const;
    virtual const PHY_PerceptionLevel& ComputePerception ( const DEC_Knowledge_Object& knowledge ) const;

    virtual void SetVisionModeNormal   ();
    virtual void SetVisionModeDirection( const MT_Vector2D& vDirection );
    virtual void SetVisionModePoint    ( const MT_Vector2D& vPoint );
    virtual bool IsUsingActiveRadar() const;
    virtual bool IsUsingActiveRadar( const PHY_RadarClass& radarClass ) const; // LTO

    virtual void Execute( dotation::ConsumptionComputer_ABC& algorithm ) const;
    virtual void Execute( detection::DetectionComputer& algorithm ) const;
    virtual void NotifyComponentHasChanged();
    virtual void NotifyCaptured();
    virtual void NotifyReleased();
    virtual void NotifySurrendered();
    virtual void NotifySurrenderCanceled();
    virtual void NotifyVisionConeDataHasChanged();
    virtual void NotifyIsLoadedForTransport();
    virtual void NotifyIsUnLoadedForTransport();
    virtual void NotifyIsLoadedInVab();
    virtual void NotifyIsUnLoadedInVab();
    //@}

    //! @name Perceptions
    //@{
    virtual void EnableSensors( bool enable );

    virtual bool  HasDelayedPerceptions          () const;
    virtual void  EnableRecoAlat                 ( const TER_Localisation& localisation );
    virtual void  DisableRecoAlat                ();
    virtual void  EnableCoupDeSonde              ();
    virtual void  DisableCoupDeSonde             ();
    virtual void  EnablePerceptionUponRequest    ();
    virtual void  DisablePerceptionUponRequest   ();
    virtual int   EnableRecoPoint                ( const MT_Vector2D& center, double rSize, double rSpeed, DEC_Decision_ABC& callerAgent );
    virtual void  DisableRecoPoint               ( int );
    virtual int   EnableRecoLocalisation         ( const TER_Localisation& localisation, float rGrowthSpeed, DEC_Decision_ABC& callerAgent );
    virtual int   EnableRecoLocalisation         ( const TER_Localisation& localisation, DEC_Decision_ABC& callerAgent );
    virtual int   EnableRecoUrbanBlock           ( MIL_UrbanObject_ABC* pUrbanBlock );
    virtual void  DisableRecoUrbanBlock          ( int );
    virtual int   EnableControlLocalisation      ( const TER_Localisation& localisation, DEC_Decision_ABC& callerAgent );
    virtual void  DisableRecoLocalisation        ( int );
    virtual int   EnableSurveillanceLocalisation ( const TER_Localisation& localisation );
    virtual void  DisableSurveillanceLocalisation( int );
    virtual int   EnableRecoObjects              ( const TER_Localisation& localisation, const MT_Vector2D& vCenter, double rSpeed, DEC_Decision_ABC& callerAgent );
    virtual void  DisableRecoObjects             ( int );
    virtual void EnableFireObserver();
    virtual void DisableFireObserver();

    // Radars
    virtual int   EnableRadarOnLocalisation      ( const PHY_RadarClass& radarClass, const TER_Localisation& localisation );
    virtual void  DisableRadarOnLocalisation     ( const PHY_RadarClass& radarClass, int );
    virtual void  EnableRadar                    ( const PHY_RadarClass& radarClass );
    virtual void  DisableRadar                   ( const PHY_RadarClass& radarClass );

    virtual int   EnableFlyingShellDetection     ( const TER_Localisation& localisation );
    virtual void  DisableFlyingShellDetection    ( int );

    // Rens
    virtual void  EnableRecordMode ();
    virtual void  DisableRecordMode();

    virtual void DisableAllPerceptions();
    //@}

    //! @name Accessors
    //@{
    virtual const T_SurfaceAgentMap&   GetSurfacesAgent () const;
    virtual const T_SurfaceObjectMap&  GetSurfacesObject() const;
    virtual const T_DisasterDetectors& GetDisasterDetectors() const;
    virtual const T_RadarMap& GetRadars( const PHY_RadarClass& radarClass );
    //@}

    //! @name Tools
    //@{
    virtual boost::shared_ptr< MIL_KnowledgeGroup > GetKnowledgeGroup() const;
    virtual MIL_Agent_ABC& GetPion() const;
    virtual double GetMaxAgentPerceptionDistance() const;
    virtual double GetMaxTheoreticalAgentPerceptionDistance() const;
    virtual void GetMainPerceptionDirection( MT_Vector2D& vDirection ) const;

    virtual bool IsPeriphericalVisionEnabled() const;
    virtual bool IsKnown( const MIL_Agent_ABC& agent ) const;
    virtual bool IsIdentified( const MIL_Agent_ABC& agent ) const;
    virtual bool IsKnown( const MIL_Object_ABC& object ) const;
    virtual bool IsIdentified( const MIL_Object_ABC& object ) const;
    virtual bool IsIdentified( const MIL_PopulationConcentration& concentration ) const;
    virtual bool IsIdentified( const MIL_UrbanObject_ABC& object ) const;
    virtual bool IsFireObserver() const;
    virtual bool CanPerceive( const MIL_ObjectType_ABC& objectType ) const;
    virtual double GetPerception( const MT_Vector2D& from, const MT_Vector2D& to ) const;
    virtual bool IsReconnoitering( MIL_UrbanObject_ABC* pUrbanBlock ) const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& msg ) const;
    virtual void SendFullState   ( client::UnitAttributes& msg ) const;
    virtual void SendVisionCones() const;
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

    typedef std::vector< PHY_Perception_ABC* > T_Perceptions;
    //@}

public:
    //! @name Types
    //@{
    typedef tools::Map< const PHY_RadarClass*, T_RadarMap > T_RadarsPerClassMap;
    //@}

private:
    //! @name Tools
    //@{
    bool HasChanged() const;
    bool CanPerceive() const;
    void PreparePerceptionData();
    void PrepareRadarData();
    void UpdatePeriphericalVisionState();
    void ComputeMainPerceptionDirection( MT_Vector2D& vMainPerceptionDirection ) const;
    template< typename T >
    void Reset( std::unique_ptr< T >& pointer );
    //@}

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC* owner_;
    MT_Vector2D lastPerceiverPosition_;
    unsigned int nNextPeriphericalVisionStep_;
    // Perceptions
    T_RadarsPerClassMap radars_;
    T_SurfaceAgentMap surfacesAgent_;
    T_SurfaceObjectMap surfacesObject_;
    T_DisasterDetectors disasterDetectors_;
    double rMaxAgentPerceptionDistance_;

    std::unique_ptr< PHY_PerceptionView >             pPerceptionView_;
    std::unique_ptr< PHY_PerceptionRecoPoint >        pPerceptionRecoPoint_;
    std::unique_ptr< PHY_PerceptionRecoLocalisation > pPerceptionRecoLocalisation_;
    std::unique_ptr< PHY_PerceptionRecoUrbanBlock >   pPerceptionRecoUrbanBlock_;
    std::unique_ptr< PHY_PerceptionRecoObjects >      pPerceptionRecoObjects_;
    std::unique_ptr< PHY_PerceptionRecoSurveillance > pPerceptionSurveillance_;
    std::unique_ptr< PHY_PerceptionRadar >            pPerceptionRadar_;
    std::unique_ptr< PHY_PerceptionFlyingShell >      pPerceptionFlyingShell_;
    std::unique_ptr< PHY_PerceptionCoupDeSonde >      pPerceptionCoupDeSonde_;
    std::unique_ptr< PHY_PerceptionAlat >             pPerceptionAlat_;
    T_Perceptions                                     perceptions_;

    E_SensorMode nSensorMode_;
    MT_Vector2D  vSensorInfo_;

    bool bPeriphericalVisionEnabled_;
    bool bRecordModeEnabled_;
    bool bHasChanged_;
    bool bExternalMustChangePerception_;
    bool bExternalMustChangeRadar_;
    bool bExternalCanPerceive_;
    bool bExternalMustUpdateVisionCones_;
    bool bRadarStateHasChanged_;
    bool bFireObserver_;
    bool bPerceptionUponRequest_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_Perceiver )

#endif // __PHY_RolePion_Perceiver_h_
