// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/ter/src/TER_AgentTraits.h $
// $Author: Age $
// $Modtime: 31/01/05 16:32 $
// $Revision: 1 $
// $Workfile: TER_AgentTraits.h $
//
// *****************************************************************************

#ifndef __TER_AgentTraits_h_
#define __TER_AgentTraits_h_

#include "MT_Tools/MT_Vector2DTypes.h"

class TER_Agent_ABC;

// =============================================================================
/** @class  TER_AgentTraits
    @brief  TER_AgentTraits
*/
// Created: AGE 2005-01-31
// =============================================================================
class TER_AgentTraits
{
public:
    virtual ~TER_AgentTraits() {}
    //! @name Types
    //@{
    typedef TER_Agent_ABC* T_Value;
    //@}

    //! @name Operations
    //@{
    int CompareOnX( MT_Float rValue, const T_Value& pAgent ) const;
    int CompareOnY( MT_Float rValue, const T_Value& pAgent ) const;
    //@}

private:
    //! @name Operations
    //@{
    int Compare( MT_Float rDelta ) const;
    //@}
};

#endif // __TER_AgentTraits_h_
