// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-28 $
// $Archive: /MVW_v10/Build/SDK/ter/src/TER_Agent_ABC.h $
// $Author: Age $
// $Modtime: 24/02/05 18:10 $
// $Revision: 1 $
// $Workfile: TER_Agent_ABC.h $
//
// *****************************************************************************

#ifndef __TER_Agent_ABC_h_
#define __TER_Agent_ABC_h_

#include "TER_AgentTraits.h"

class TER_AgentManager;

namespace spatialcontainer
{
    template< typename T, typename Traits, typename Coordinate > class SpatialContainerNode;
};

// =============================================================================
// Created: AGE 2005-01-28
// =============================================================================
class TER_Agent_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< TER_Agent_ABC* >      T_AgentPtrVector;
    typedef T_AgentPtrVector::iterator        IT_AgentPtrVector;
    typedef T_AgentPtrVector::const_iterator CIT_AgentPtrVector;

    typedef spatialcontainer::SpatialContainerNode< TER_Agent_ABC*, TER_AgentTraits, double >* T_Hint;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TER_Agent_ABC();
    virtual ~TER_Agent_ABC();
    //@}

    //! @name Operations
    //@{
    virtual    const MT_Vector2D& GetPosition() const = 0;

    void UpdatePatch();
    void SetListener( TER_AgentManager& agentManager );
    //@}

protected:
    //! @name Operations
    //@{
    void RemoveFromPatch();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TER_Agent_ABC( const TER_Agent_ABC& );            //!< Copy constructor
    TER_Agent_ABC& operator=( const TER_Agent_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void InsertInPatch();
    //@}

private:
    //! @name Member data
    //@{
    T_Hint hint_;
    TER_AgentManager* pAgentManager_;
    //@}
};

#endif // __TER_Agent_ABC_h_
