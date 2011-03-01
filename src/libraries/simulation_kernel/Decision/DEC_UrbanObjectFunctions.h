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
class UrbanObjectWrapper;

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
    static float GetCurrentRecceProgress( boost::shared_ptr< UrbanObjectWrapper > pUrbanObject );
    static boost::shared_ptr< MT_Vector2D > GetCurrentBarycenter( boost::shared_ptr< UrbanObjectWrapper > pUrbanObject );
    static std::vector< boost::shared_ptr< MT_Vector2D > > GetBoundingBox( boost::shared_ptr< UrbanObjectWrapper > pUrbanObject );
    static float GetPathfindCost( const MIL_AgentPion& callerAgent, boost::shared_ptr< UrbanObjectWrapper > pUrbanObject );
    static float GetRapForLocal( const MIL_AgentPion& callerAgent, boost::shared_ptr< UrbanObjectWrapper > pUrbanObject );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesInBU( const MIL_AgentPion& callerAgent, boost::shared_ptr< UrbanObjectWrapper > pUrbanObject );
    //@}
};

#endif // __DEC_UrbanObjectFunctions_h_
