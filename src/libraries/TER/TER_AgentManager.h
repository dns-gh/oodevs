// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/ter/src/TER_AgentManager.h $
// $Author: Age $
// $Modtime: 31/01/05 18:53 $
// $Revision: 1 $
// $Workfile: TER_AgentManager.h $
//
// *****************************************************************************

#ifndef __TER_AgentManager_h_
#define __TER_AgentManager_h_

#include "TER.h"
#include "pathfind/SpatialContainer.h"
#include "TER_AgentTraits.h"

class MT_Ellipse;
class TER_Agent_ABC;
class TER_Localisation;
class TER_Polygon;
class TER_AgentPositionHint;
class MT_Rect;

// =============================================================================
/** @class  TER_AgentManager
    @brief  TER_AgentManager
*/
// Created: AGE 2005-01-31
// =============================================================================
class TER_AgentManager
{
public:
    //! @name Types
    //@{
    typedef std::vector< TER_Agent_ABC* >                                           T_AgentVector;
    typedef pathfind::SpatialContainer< TER_Agent_ABC*, TER_AgentTraits, MT_Float > T_Agents;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TER_AgentManager( const MT_Rect& extent );
    virtual ~TER_AgentManager();
    //@}

    //! @name Operations
    //@{
    void GetListAgentWithinEllipse( const MT_Ellipse& ellipse, T_AgentVector& agentVector ) const;
    void GetListAgentWithinCircle ( const MT_Vector2D& vCenter, MT_Float rRadius, T_AgentVector& agentVector ) const;
    void GetListAgentWithinLocalisation( const TER_Localisation& localisation, T_AgentVector& agentVector ) const;
    void GetListAgentWithinPolygon( const TER_Polygon& polygon, T_AgentVector& agentVector ) const;

    TER_AgentPositionHint UpdateAgentPosition( TER_Agent_ABC& agent, const TER_AgentPositionHint& hint );
    bool RemoveAgent( TER_Agent_ABC& agent, const TER_AgentPositionHint& hint );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TER_AgentManager( const TER_AgentManager& );            //!< Copy constructor
    TER_AgentManager& operator=( const TER_AgentManager& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    T_Agents agents_;
    //@}
};

#endif // __TER_AgentManager_h_
