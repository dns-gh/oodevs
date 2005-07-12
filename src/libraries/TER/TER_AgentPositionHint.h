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
// $Archive: /MVW_v10/Build/SDK/ter/src/TER_AgentPositionHint.h $
// $Author: Age $
// $Modtime: 31/01/05 17:32 $
// $Revision: 1 $
// $Workfile: TER_AgentPositionHint.h $
//
// *****************************************************************************

#ifndef __TER_AgentPositionHint_h_
#define __TER_AgentPositionHint_h_

namespace pathfind
{
    template< typename T, typename Traits, typename Coordinate > class SpatialContainerNode;
};
class TER_Agent_ABC;
class TER_AgentTraits;

// =============================================================================
/** @class  TER_AgentPositionHint
    @brief  TER_AgentPositionHint
*/
// Created: AGE 2005-01-31
// =============================================================================
class TER_AgentPositionHint
{
public:
    //! @name Types
    //@{
    typedef pathfind::SpatialContainerNode< TER_Agent_ABC*, TER_AgentTraits, MT_Float >* T_Hint;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    TER_AgentPositionHint( const T_Hint& hint ) : hint_( hint ) {};
    //@}

    //@{
    operator T_Hint() const { return hint_; };
    //@}

private:
    //! @name Member data
    //@{
    T_Hint hint_;
    //@}
};

#endif // __TER_AgentPositionHint_h_
