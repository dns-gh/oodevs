// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#ifndef SWORD_ROLE_PION_PERCEIVER_H
#define SWORD_ROLE_PION_PERCEIVER_H

#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "ComponentsChangedNotificationHandler_ABC.h"
#include "simulation_kernel/SurrenderNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkUnitAttributesMessageSender_ABC.h"
#include "simulation_kernel/VisionConeNotificationHandler_ABC.h"
#include "simulation_kernel/TransportChangeNotificationHandler_ABC.h"
#include "simulation_kernel/LoadingChangeNotificationHandler_ABC.h"

class DEC_KS_Perception;
class PHY_Perception_ABC;
class PHY_PerceptionCoupDeSonde;
class PHY_PerceptionRecoPoint;
class PHY_PerceptionRecoLocalisation;
class PHY_PerceptionRecoObjects;
class PHY_PerceptionRecoSurveillance;
class PHY_PerceptionRecoUrbanBlock;
class PHY_PerceptionRadar;
class PHY_PerceptionAlat;
class PHY_PerceptionFlyingShell;
class TER_Agent_ABC;
class TER_Object_ABC;
class TER_PopulationConcentration_ABC;
class TER_PopulationFlow_ABC;

namespace detection
{
    class DetectionComputer_ABC;
}

namespace core
{
    class Model;
    class Facade;
}

namespace sword
{
    class ListenerHelper;

// =============================================================================
// @class  RolePion_Perceiver
// Created: JVT 2004-08-03
// Modified: JVT 2004-10-28
// =============================================================================
class RolePion_Perceiver : public PHY_RoleInterface_Perceiver
                         , public tools::AlgorithmModifier_ABC< detection::DetectionComputer_ABC >
                         , public component::ComponentsChangedNotificationHandler_ABC
                         , public surrender::SurrenderNotificationHandler_ABC
                         , public network::NetworkUnitAttributesMessageSender_ABC
                         , public network::VisionConeNotificationHandler_ABC
                         , public transport::TransportChangeNotificationHandler_ABC
                         , public transport::LoadingChangeNotificationHandler_ABC
{
public:
    //! @name Static Initializer
    //@{
    static void Initialize( core::Facade& facade );
    //@}

    //! @name Constructors/Destructor
    //@{
             RolePion_Perceiver( MIL_Agent_ABC& pion, core::Model& entity );
    virtual ~RolePion_Perceiver();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //@{
    virtual void NotifyExternalPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level );
    void NotifyPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level, bool bPerceptionRecorded );
    bool NotifyPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level );
    void NotifyPerception( MIL_Object_ABC& object, const PHY_PerceptionLevel& level );
    bool NotifyPerception( MIL_PopulationConcentration& concentration, const PHY_PerceptionLevel& level );
    bool NotifyPerception( MIL_PopulationFlow& flow, const PHY_PerceptionLevel& level, const T_PointVector& shape );
    void NotifyPerception( const MIL_Effect_IndirectFire& flyingShell ) const;
    void NotifyPerceptionUrban( const UrbanObjectWrapper& block, const PHY_PerceptionLevel& level ) const;
    //@}

    //! @name Operations
    //@{
    void                       Update            ( bool bIsDead );
    void                       Clean             ();
    void                       ExecutePerceptions();
    const PHY_PerceptionLevel& ComputePerception ( const MT_Vector2D& vPoint             ) const;
    const PHY_PerceptionLevel& ComputePerception ( const DEC_Knowledge_Object& knowledge ) const;

    void SetVisionModeNormal   ();
    void SetVisionModeDirection( const MT_Vector2D& vDirection );
    void SetVisionModePoint    ( const MT_Vector2D& vPoint );
    virtual bool IsUsingActiveRadar() const;
    virtual bool IsUsingActiveRadar( const PHY_RadarClass& radarClass ) const; // LTO

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
    int   EnableRecoPoint                ( const MT_Vector2D& center, double rSize, double rSpeed, DEC_Decision_ABC& callerAgent );
    void  DisableRecoPoint               ( int );
    int   EnableRecoLocalisation         ( const TER_Localisation& localisation, float rGrowthSpeed, DEC_Decision_ABC& callerAgent );
    int   EnableRecoLocalisation         ( const TER_Localisation& localisation, DEC_Decision_ABC& callerAgent );
    int   EnableRecoUrbanBlock           ( UrbanObjectWrapper* pUrbanBlock );
    void  DisableRecoUrbanBlock          ( int );
    int   EnableControlLocalisation      ( const TER_Localisation& localisation, DEC_Decision_ABC& callerAgent );
    void  DisableRecoLocalisation        ( int );
    int   EnableSurveillanceLocalisation ( const TER_Localisation& localisation );
    void  DisableSurveillanceLocalisation( int );
    int   EnableRecoObjects              ( const TER_Localisation& localisation, const MT_Vector2D& vCenter, double rSpeed, DEC_Decision_ABC& callerAgent );
    void  DisableRecoObjects             ( int );
    virtual void EnableFireObserver      ();
    virtual void DisableFireObserver     ();

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
    const MIL_KnowledgeGroup& GetKnowledgeGroup() const;
    MIL_Agent_ABC& GetPion() const;
    double GetMaxAgentPerceptionDistance() const;
    double GetMaxTheoreticalAgentPerceptionDistance() const;
    void GetMainPerceptionDirection( MT_Vector2D& vDirection ) const;

    bool IsPeriphericalVisionEnabled() const;
    bool IsKnown( const MIL_Agent_ABC& agent ) const;
    bool IsIdentified( const MIL_Agent_ABC& agent ) const;
    bool IsKnown( const MIL_Object_ABC& object ) const;
    bool IsIdentified( const MIL_Object_ABC& object ) const;
    bool IsIdentified( const MIL_PopulationConcentration& concentration ) const;
    bool IsIdentified( const UrbanObjectWrapper& object ) const;
    virtual bool IsFireObserver() const;
    //@}

    //! @name Network
    //@{
    void SendDebugState  () const;
    void SendChangedState( client::UnitAttributes& msg ) const;
    void SendFullState   ( client::UnitAttributes& msg ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< ListenerHelper > > T_Listeners;
    typedef std::vector< PHY_Perception_ABC* > T_PerceptionVector;
    typedef T_PerceptionVector::iterator IT_PerceptionVector;
    typedef T_PerceptionVector::const_iterator CIT_PerceptionVector;
    typedef boost::function< void( DEC_KS_Perception& ) > T_Notification;
    typedef std::vector< T_Notification > T_Notifications;
    //@}

private:
    //! @name Tools
    //@{
    bool HasChanged() const;
    bool HasRadarStateChanged() const;
    bool CanPerceive() const;
    void EnsurePerceptionRecoLocalisation();
    void AppendHackedAgents( std::vector< TER_Agent_ABC* >& perceivableAgents ) const;
    void AppendHackedPopulationConcentrations( std::vector< TER_PopulationConcentration_ABC* >& perceivableAgents ) const;
    void AppendHackedPopulationFlows( std::vector< TER_PopulationFlow_ABC* >& perceivableAgents ) const;
    template< typename Listener, typename Enable, typename Disable > void AddListener( const std::string& perception, Enable enable, Disable disable );
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( sword::RolePion_Perceiver )
    //@}

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC& owner_;
    core::Model& entity_;
    T_Listeners listeners_;
    MT_Vector2D lastPerceiverPosition_;
    unsigned int nNextPeriphericalVisionStep_;
    // Perceptions
    T_RadarsPerClassMap radars_;
    T_SurfaceAgentMap surfacesAgent_;
    T_SurfaceObjectMap surfacesObject_;

    PHY_PerceptionCoupDeSonde*      pPerceptionCoupDeSonde_;
    PHY_PerceptionRecoPoint*        pPerceptionRecoPoint_;
    PHY_PerceptionRecoLocalisation* pPerceptionRecoLocalisation_;
    PHY_PerceptionRecoUrbanBlock*   pPerceptionRecoUrbanBlock_;
    PHY_PerceptionRecoObjects*      pPerceptionRecoObjects_;
    PHY_PerceptionRecoSurveillance* pPerceptionSurveillance_;
    PHY_PerceptionRadar*            pPerceptionRadar_;
    PHY_PerceptionAlat*             pPerceptionAlat_;
    PHY_PerceptionFlyingShell*      pPerceptionFlyingShell_;
    T_PerceptionVector              activePerceptions_;

    bool bRecordModeEnabled_;
    bool bHasChanged_;
    bool bExternalMustChangePerception_;
    bool bExternalMustChangeRadar_;
    bool bExternalCanPerceive_;
    bool bExternalMustUpdateVisionCones_;
    bool bRadarStateHasChanged_;
    T_Notifications notifications_;
    //@}

    //! @name Static member data
    //@{
    static const unsigned int nNbrStepsBetweenPeriphericalVision_;
    //@}
};

}

BOOST_CLASS_EXPORT_KEY( sword::RolePion_Perceiver )

#endif // SWORD_ROLE_PION_PERCEIVER_H
