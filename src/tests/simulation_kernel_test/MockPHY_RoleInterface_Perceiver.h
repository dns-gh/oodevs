// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockPHY_RoleInterface_Perceiver_h_
#define __MockPHY_RoleInterface_Perceiver_h_

#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"

MOCK_BASE_CLASS( MockPHY_RoleInterface_Perceiver, PHY_RoleInterface_Perceiver )
{
    MOCK_METHOD( IsUsingActiveRadar, 0, bool(), IsUsingActiveRadar );
    MOCK_METHOD( IsUsingActiveRadar, 1, bool( const PHY_RadarClass& ), IsUsingActiveRadarByClass );

    MOCK_METHOD( NotifyExternalPerception, 2 );
    MOCK_METHOD( NotifyPerception, 3, void( MIL_Agent_ABC&, const PHY_PerceptionLevel&, bool ), NotifyAgentPerceptionRecorded );
    MOCK_METHOD( NotifyPerception, 2, bool( MIL_Agent_ABC&, const PHY_PerceptionLevel& ), NotifyAgentPerception );
    MOCK_METHOD( NotifyPerception, 2, void( MIL_Object_ABC&, const PHY_PerceptionLevel& ), NotifyObjectPerception );
    MOCK_METHOD( NotifyPerception, 2, bool( MIL_PopulationConcentration&, const PHY_PerceptionLevel& ), NotifyPopulationConcentrationPerception );
    MOCK_METHOD( NotifyPerception, 3, bool( MIL_PopulationFlow&, const PHY_PerceptionLevel&, const T_PointVector& ), NotifyPopulationFlowPerception );
    MOCK_METHOD( NotifyPerception, 1, bool( const MIL_Effect_IndirectFire& ), NotifyIndirectFirePerception );
    MOCK_METHOD( NotifyPerception, 3, void( MIL_Object_ABC&, const MT_Vector2D&, const MT_Vector2D& ), NotifyDisasterCollision );
    MOCK_METHOD( NotifyPerceptionUrban, 2, void( const MIL_UrbanObject_ABC&, const PHY_PerceptionLevel& ), NotifyUrbanBlockPerception );

    MOCK_METHOD( Update, 1 );
    MOCK_METHOD( Clean, 0 );
    MOCK_METHOD( ExecutePerceptions, 0 );
    MOCK_METHOD( ExecuteCollisions, 0 );
    MOCK_METHOD( ComputePerception, 1, const PHY_PerceptionLevel&( const MT_Vector2D& ), ComputePointPerception );
    MOCK_METHOD( ComputePerception, 1, const PHY_PerceptionLevel&( const DEC_Knowledge_Object& ), ComputeObjectKnowledgePerception );
    MOCK_METHOD( ComputePerception, 1, const PHY_PerceptionLevel&( const DEC_Knowledge_Agent& ), ComputeAgentKnowledgePerception );

    MOCK_METHOD( SetVisionModeNormal, 0 );
    MOCK_METHOD( SetVisionModeDirection, 1 );
    MOCK_METHOD( SetVisionModePoint, 1 );

    MOCK_METHOD( EnableSensors, 1 );
    MOCK_METHOD( EnablePerceptionUponRequest, 0 );
    MOCK_METHOD( DisablePerceptionUponRequest, 0 );
    MOCK_METHOD( EnableFireObserver, 0 );
    MOCK_METHOD( DisableFireObserver, 0 );

    MOCK_METHOD( HasDelayedPerceptions, 0 );
    MOCK_METHOD( EnableRecoAlat, 1 );
    MOCK_METHOD( DisableRecoAlat, 0 );
    MOCK_METHOD( EnableCoupDeSonde, 0 );
    MOCK_METHOD( DisableCoupDeSonde, 0 );
    MOCK_METHOD( EnableRecoPoint, 4 );
    MOCK_METHOD( DisableRecoPoint, 1 );
    MOCK_METHOD( EnableRecoLocalisation, 3, int( const TER_Localisation&, float, DEC_Decision_ABC& callerAgent ), EnableRecoLocalisationWithRadius );
    MOCK_METHOD( EnableRecoLocalisation, 2, int( const TER_Localisation&, DEC_Decision_ABC& callerAgent ), EnableRecoLocalisation );
    MOCK_METHOD( EnableRecoUrbanBlock, 1 );
    MOCK_METHOD( DisableRecoUrbanBlock, 1 );
    MOCK_METHOD( EnableControlLocalisation, 2 );
    MOCK_METHOD( DisableRecoLocalisation, 1 );
    MOCK_METHOD( EnableSurveillanceLocalisation, 1 );
    MOCK_METHOD( DisableSurveillanceLocalisation, 1 );
    MOCK_METHOD( EnableRecoObjects, 4 );
    MOCK_METHOD( DisableRecoObjects, 1 );
    MOCK_METHOD( EnableRadarOnLocalisation, 2 );
    MOCK_METHOD( DisableRadarOnLocalisation, 2 );
    MOCK_METHOD( EnableRadar, 1 );
    MOCK_METHOD( DisableRadar, 1 );
    MOCK_METHOD( EnableFlyingShellDetection, 1 );
    MOCK_METHOD( DisableFlyingShellDetection, 1 );
    MOCK_METHOD( EnableRecordMode, 0 );
    MOCK_METHOD( DisableRecordMode, 0 );
    MOCK_METHOD( DisableAllPerceptions, 0 );
    MOCK_METHOD( GetSurfacesAgent, 0 );
    MOCK_METHOD( GetSurfacesObject, 0 );
    MOCK_METHOD( GetDisasterDetectors, 0 );
    MOCK_METHOD( GetRadars, 1 );

    MOCK_METHOD( GetKnowledgeGroup, 0 );
    MOCK_METHOD( GetPion, 0 );
    MOCK_METHOD( GetMaxAgentPerceptionDistance, 0 );
    MOCK_METHOD( GetMaxTheoreticalAgentPerceptionDistance, 0 );
    MOCK_METHOD( GetMainPerceptionDirection, 1 );

    MOCK_METHOD( IsPeriphericalVisionEnabled, 0 );
    MOCK_METHOD( IsKnown, 1, bool( const MIL_Agent_ABC& ), IsAgentKnown );
    MOCK_METHOD( IsKnown, 1, bool( const MIL_Object_ABC& ), IsObjectKnown );
    MOCK_METHOD( IsIdentified, 1, bool( const MIL_Agent_ABC& ), IsAgentIdentified );
    MOCK_METHOD( IsIdentified, 1, bool( const MIL_Object_ABC& ), IsObjectIdentified );
    MOCK_METHOD( IsIdentified, 1, bool( const MIL_PopulationConcentration& ), IsPopulationConcentrationIdentified );
    MOCK_METHOD( IsIdentified, 1, bool( const MIL_UrbanObject_ABC& ), IsUrbanBlockIdentified );
    MOCK_METHOD( IsFireObserver, 0 );
    MOCK_METHOD( CanPerceive, 1 );
    MOCK_METHOD( GetPerception, 2 );
    MOCK_METHOD( IsReconnoitering, 1 );

    MOCK_METHOD( SendChangedState, 1 );
    MOCK_METHOD( SendFullState, 1 );
    MOCK_METHOD( SendVisionCones, 0 );
};

#endif // __MockPHY_RoleInterface_Perceiver_h_
