// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Perception/PHY_RoleHLA_Perceiver.h $
// $Author: Nld $
// $Modtime: 2/05/05 18:25 $
// $Revision: 2 $
// $Workfile: PHY_RoleHLA_Perceiver.h $
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_Perceiver_h_
#define __PHY_RoleHLA_Perceiver_h_

#include "PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_AgentPion.h"

// =============================================================================
// @class  PHY_RoleHLA_Perceiver
// Created: JVT 2004-08-03
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_RoleHLA_Perceiver : public PHY_RoleInterface_Perceiver
{

public:
             PHY_RoleHLA_Perceiver();
    virtual ~PHY_RoleHLA_Perceiver();

    //! @name Operations
    //@{    
    virtual bool IsUsingActiveRadar() const;
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}
    virtual void NotifyExternalPerception( MIL_Agent_ABC& /*agent*/, const PHY_PerceptionLevel& /*level*/ ) {};
    virtual void NotifyPerception( MIL_Agent_ABC&               /*agent*/        , const PHY_PerceptionLevel& /*level*/, bool bPerceptionRecorded ) {};
    virtual void NotifyPerception( MIL_Agent_ABC&               /*agent*/        , const PHY_PerceptionLevel& /*level*/ ) {};
    virtual void NotifyPerception( MIL_Object_ABC&              /*object*/       , const PHY_PerceptionLevel& /*level*/ ) {};
    virtual void NotifyPerception( MIL_PopulationConcentration& /*concentration*/, const PHY_PerceptionLevel& /*level*/ ) {};
    virtual void NotifyPerception( MIL_PopulationFlow&          /*flow*/         , const PHY_PerceptionLevel& /*level*/, const T_PointVector& /*shape*/ ) {};
    virtual void NotifyPerception( const MIL_Effect_IndirectFire& /*flyingShell*/ ) const {};    
    virtual void NotifyPerception( const urban::TerrainObject_ABC& block, const PHY_PerceptionLevel& level ) const {};  
    //@}

    //! @name Operations
    //@{    
    virtual bool                       HasChanged        () const { return false; };
    virtual void                       Update            ( bool /*bIsDead*/ ) {};
    virtual void                       Clean             () {};
    virtual void                       ExecutePerceptions() {};
    virtual const PHY_PerceptionLevel& ComputePerception ( const MT_Vector2D& /*vPoint*/             ) const { return PHY_PerceptionLevel::notSeen_; };
    virtual const PHY_PerceptionLevel& ComputePerception ( const DEC_Knowledge_Object& /*knowledge*/ ) const { return PHY_PerceptionLevel::notSeen_; };
    virtual const PHY_PerceptionLevel& ComputePerception ( const DEC_Knowledge_Agent & /*knowledge*/ ) const { return PHY_PerceptionLevel::notSeen_; };
    virtual const PHY_PerceptionLevel& ComputePerception ( const urban::Block& block ) const { return PHY_PerceptionLevel::notSeen_; };

    virtual void SetVisionModeNormal   () {};
    virtual void SetVisionModeDirection( const MT_Vector2D& /*vDirection*/ ) {};
    virtual void SetVisionModePoint    ( const MT_Vector2D& /*vPoint*/ ) {};

    virtual bool HasRadarStateChanged() const { return false; };
    //@}

    //! @name Perceptions
    //@{
    virtual void EnableSensors (){};
    virtual void DisableSensors(){};

    virtual bool  HasDelayedPerceptions          () const { return false; };
    virtual void  EnableRecoAlat                 ( const TER_Localisation& /*localisation*/ ) {};
    virtual void  DisableRecoAlat                () {};
    virtual void  EnableCoupDeSonde              () {};
    virtual void  DisableCoupDeSonde             () {};
    virtual int   EnableRecoPoint                ( const MT_Vector2D& /*center*/, MT_Float /*rSize*/, MT_Float /*rSpeed*/, DEC_Decision_ABC& /*callerAgent*/ ) { return 0;};
    virtual void  DisableRecoPoint               ( int ) {};
    virtual int   EnableRecoLocalisation         ( const TER_Localisation& /*localisation*/, MT_Float /*rRadius*/ ) { return 0; };
    virtual int   EnableRecoLocalisation         ( const TER_Localisation& /*localisation*/ ) { return 0; };
    virtual int   EnableControlLocalisation      ( const TER_Localisation& /*localisation*/ ) { return 0; };
    virtual void  DisableRecoLocalisation        ( int ) {};
    virtual int   EnableSurveillanceLocalisation ( const TER_Localisation& /*localisation*/ ) { return 0; };
    virtual void  DisableSurveillanceLocalisation( int ) {};
    virtual int   EnableRecoObjects              ( const TER_Localisation& /*localisation*/, const MT_Vector2D& /*vCenter*/, MT_Float /*rSpeed*/, DEC_Decision_ABC& /*callerAgent*/ ) { return 0; };
    virtual void  DisableRecoObjects             ( int ) {};

    // Radars
    virtual int   EnableRadarOnLocalisation      ( const PHY_RadarClass& /*radarClass*/, const TER_Localisation& /*localisation*/ ) { return 0; };
    virtual void  DisableRadarOnLocalisation     ( const PHY_RadarClass& /*radarClass*/, int ) {};
    virtual void  EnableRadar                    ( const PHY_RadarClass& /*radarClass*/ ) {};
    virtual void  DisableRadar                   ( const PHY_RadarClass& /*radarClass*/ ) {};

    virtual int   EnableFlyingShellDetection     ( const TER_Localisation& /*localisation*/ ) { return 0;};
    virtual void  DisableFlyingShellDetection    ( int ) {};

    // Rens
    virtual void  EnableRecordMode () {};
    virtual void  DisableRecordMode() {};

    virtual void DisableAllPerceptions() {};
    //@}

    //! @name Accessors
    //@{
    virtual const T_SurfaceAgentMap&  GetSurfacesAgent () const { return surfacesAgent_; };
    virtual const T_SurfaceObjectMap& GetSurfacesObject() const { return surfacesObject_; };
    virtual const T_RadarSet&         GetRadars        ( const PHY_RadarClass& radarClass ) { return radars_[ &radarClass ]; };
    //@}

    //! @name Tools
    //@{
    virtual const MIL_KnowledgeGroup& GetKnowledgeGroup            () const { return pPion_->GetKnowledgeGroup(); };
    virtual MIL_AgentPion&      GetPion                      () const { return *pPion_; };
    virtual MT_Float            GetMaxAgentPerceptionDistance() const { return 0; };
    virtual void                GetMainPerceptionDirection   ( MT_Vector2D& /*vDirection*/ ) const {};

    virtual bool IsPeriphericalVisionEnabled() const { return false; };
    virtual bool IsKnown                    ( const MIL_Agent_ABC&               /*agent*/         ) const { return false; };
    virtual bool IsIdentified               ( const MIL_Agent_ABC&               /*agent*/         ) const { return false; };
    virtual bool WasPerceived               ( const MIL_Agent_ABC&               /*agent*/         ) const { return false; };
    virtual bool IsKnown                    ( const MIL_Object_ABC&              /*object*/        ) const { return false; };
    virtual bool IsIdentified               ( const MIL_Object_ABC&              /*object*/        ) const { return false; };
    virtual bool IsIdentified               ( const MIL_PopulationConcentration& /*concentration*/ ) const { return false; };
    virtual bool IsIdentified               ( const urban::TerrainObject_ABC&    object        ) const { return false; };
    //@}

    //! @name Network
    //@{
    virtual void SendDebugState  () const {};
    //@}

private:
    bool bIsUsingActiveRadar_; 
    T_RadarsPerClassMap radars_;
    T_SurfaceAgentMap   surfacesAgent_;
    T_SurfaceObjectMap  surfacesObject_;

    MIL_AgentPion* pPion_; // $$$Hack de test
};

#endif // __PHY_RoleHLA_Perceiver_h_
