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

#include "TER_AgentTraits.h"
#include "TER_Agent_ABC.h"
#include <spatialcontainer/SpatialContainer.h>

class TER_Localisation;
class TER_Polygon;
class TER_AgentPositionHint;
class MT_Rect;
class MT_Ellipse;
class TER_AgentVisitor_ABC;

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
    typedef std::vector< TER_Agent_ABC* > T_AgentVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TER_AgentManager( const MT_Rect& extent );
    virtual ~TER_AgentManager();
    //@}

    //! @name Operations
    //@{
    void GetListWithinEllipse( const MT_Ellipse& ellipse, T_AgentVector& agents ) const;
    void GetListWithinCircle( const MT_Vector2D& vCenter, double rRadius, T_AgentVector& agents ) const;
    void GetListWithinLocalisation( const TER_Localisation& localisation, T_AgentVector& agents ) const;
    void GetListWithinLocalisation( const TER_Localisation& localisation, T_AgentVector& agents, double rPrecision ) const;
    void GetListWithinPolygon( const TER_Polygon& polygon, T_AgentVector& agents ) const;

    TER_Agent_ABC::T_Hint UpdatePosition( TER_Agent_ABC& agent, const TER_Agent_ABC::T_Hint& pHint );
    bool Remove( TER_Agent_ABC& agent, const TER_Agent_ABC::T_Hint& pHint );
    void Accept( TER_AgentVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TER_AgentManager( const TER_AgentManager& );            //!< Copy constructor
    TER_AgentManager& operator=( const TER_AgentManager& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef spatialcontainer::SpatialContainer< TER_Agent_ABC*, TER_AgentTraits, double > T_Agents;
    //@}

private:
    //! @name Member data
    //@{
    T_Agents agents_;
    //@}
};

#endif // __TER_AgentManager_h_
