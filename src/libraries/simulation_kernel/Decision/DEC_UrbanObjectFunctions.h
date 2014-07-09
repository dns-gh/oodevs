// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __DEC_UrbanObjectFunctions_h_
#define __DEC_UrbanObjectFunctions_h_

#include "Knowledge/DEC_Knowledge_Def.h"

class MIL_AgentPion;
class MT_Vector2D;
class MIL_UrbanObject_ABC;
class TER_Localisation;
class PHY_DotationCategory;

// =============================================================================
/** @class  DEC_UrbanObjectFunctions
    @brief  DEC Knowledge urban functions
*/
// Created: SLG 2010-02-01
// =============================================================================
class DEC_UrbanObjectFunctions
{
public:
    //! @name Functions
    //@{
    static float GetCurrentRecceProgress( const MIL_AgentPion& pion, MIL_UrbanObject_ABC* pUrbanObject );
    static boost::shared_ptr< MT_Vector2D > GetCurrentBarycenter( MIL_UrbanObject_ABC* pUrbanObject );
    static boost::shared_ptr< MT_Vector2D > GetBarycenter( MIL_UrbanObject_ABC* pUrbanObject );
    static std::vector< boost::shared_ptr< MT_Vector2D > > GetBoundingBox( MIL_UrbanObject_ABC* pUrbanObject );
    static double GetPathfindCost( const MIL_AgentPion& callerAgent, MIL_UrbanObject_ABC* pUrbanObject );
    static float GetRapForLocal( const MIL_AgentPion& callerAgent, MIL_UrbanObject_ABC* pUrbanObject );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesInBU( const MIL_AgentPion& callerAgent, MIL_UrbanObject_ABC* pUrbanObject );
    static void DestroyUrbanBlock(  MIL_AgentPion& callerAgent, MIL_UrbanObject_ABC* pUrbanObject, const PHY_DotationCategory* category );
    static float GetStateUrbanBlock( MIL_UrbanObject_ABC* pUrbanObject );
    static void SetUrbanBlockState( MIL_UrbanObject_ABC* pUrbanObject, float state );
    static boost::shared_ptr< TER_Localisation > GetPolygonFromUrbanBlock( const MIL_UrbanObject_ABC* pUrbanObject );
    static std::string GetType( const MIL_UrbanObject_ABC* pUrbanObject );
    static T_UrbanObjectVector GetUrbanBlockInZone( TER_Localisation* );
    //@}
};

#endif // __DEC_UrbanObjectFunctions_h_
