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
#include "simulation_kernel/NetworkUnitAttributesMessageSender_ABC.h"

class DEC_KS_Perception;

namespace detection
{
    class DetectionComputer_ABC;
}

namespace core
{
    class Model;
}

namespace sword
{
    class ListenerHelper;
    class Sink;

// =============================================================================
// @class  RolePion_Perceiver
// Created: JVT 2004-08-03
// Modified: JVT 2004-10-28
// =============================================================================
class RolePion_Perceiver : public PHY_RoleInterface_Perceiver
                         , public tools::AlgorithmModifier_ABC< detection::DetectionComputer_ABC >
                         , public network::NetworkUnitAttributesMessageSender_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RolePion_Perceiver();
             RolePion_Perceiver( Sink& sink, const core::Model& model, MIL_Agent_ABC& pion, core::Model& entity );
    virtual ~RolePion_Perceiver();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template< typename Archive > void load( Archive&, const unsigned int );
    template< typename Archive > void save( Archive&, const unsigned int ) const;
    //@}

    //@{
    virtual void NotifyExternalPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level );
    virtual void NotifyPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level, bool bPerceptionRecorded );
    virtual bool NotifyPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level );
    virtual void NotifyPerception( MIL_Object_ABC& object, const PHY_PerceptionLevel& level );
    virtual bool NotifyPerception( MIL_PopulationConcentration& concentration, const PHY_PerceptionLevel& level );
    virtual bool NotifyPerception( MIL_PopulationFlow& flow, const PHY_PerceptionLevel& level, const T_PointVector& shape );
    virtual void NotifyPerception( const MIL_Effect_IndirectFire& flyingShell ) const;
    virtual void NotifyPerceptionUrban( const MIL_UrbanObject_ABC& block, const PHY_PerceptionLevel& level ) const;
    virtual void NotifyPerception( MIL_Object_ABC& object, const MT_Vector2D& vPosition, const MT_Vector2D& vDirection );
    //@}

    //! @name Operations
    //@{
    virtual void                       Finalize          ();
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

    virtual void Execute( detection::DetectionComputer_ABC& algorithm ) const;
    //@}

    //! @name Perceptions
    //@{
    virtual void EnableSensors ();
    virtual void DisableSensors();

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
    virtual void EnableFireObserver      ();
    virtual void DisableFireObserver     ();

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
    virtual const T_SurfaceAgentMap&  GetSurfacesAgent () const;
    virtual const T_SurfaceObjectMap& GetSurfacesObject() const;
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
    double GetPerception( const MT_Vector2D &, const MT_Vector2D& ) const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& msg ) const;
    virtual void SendFullState   ( client::UnitAttributes& msg ) const;
    //@}

private:
    void SendVisionCones() const;

private:
    //! @name Member data
    //@{
    Sink* sink_;
    const core::Model* model_;
    MIL_Agent_ABC* owner_;
    core::Model* entity_;
    boost::shared_ptr< ListenerHelper > recordModeListener_;
    //@}
};
}

BOOST_CLASS_EXPORT_KEY( sword::RolePion_Perceiver )

#endif // SWORD_ROLE_PION_PERCEIVER_H
