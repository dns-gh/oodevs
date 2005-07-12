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
// $Archive: /MVW_v10/Build/SDK/ter/src/TER_ObjectPositionHint.h $
// $Author: Age $
// $Modtime: 31/01/05 17:54 $
// $Revision: 1 $
// $Workfile: TER_ObjectPositionHint.h $
//
// *****************************************************************************

#ifndef __TER_ObjectPositionHint_h_
#define __TER_ObjectPositionHint_h_

namespace pathfind
{
    template< typename T, typename Traits, typename Coordinate > class SpatialContainerNode;
};
class TER_DynaObject_ABC;
class TER_ObjectTraits;

// =============================================================================
/** @class  TER_ObjectPositionHint
    @brief  TER_ObjectPositionHint
*/
// Created: AGE 2005-01-31
// =============================================================================
class TER_ObjectPositionHint
{

public:
    //! @name Types
    //@{
    typedef pathfind::SpatialContainerNode< TER_DynaObject_ABC*, TER_ObjectTraits, MT_Float >* T_Hint;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    TER_ObjectPositionHint( const T_Hint& hint ) : hint_( hint ) {};
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

#endif // __TER_ObjectPositionHint_h_
