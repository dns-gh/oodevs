// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-02-23 $
// $Archive: /MVW_v10/Build/SDK/ter/src/TER_PathFindRequest_ABC.h $
// $Author: Age $
// $Modtime: 24/02/05 18:04 $
// $Revision: 1 $
// $Workfile: TER_PathFindRequest_ABC.h $
//
// *****************************************************************************

#ifndef __TER_PathFindRequest_ABC_h_
#define __TER_PathFindRequest_ABC_h_

class TER_Pathfinder_ABC;

// =============================================================================
/** @class  TER_PathFindRequest_ABC
    @brief  Pathfind request definition
*/
// Created: AGE 2005-02-23
// =============================================================================
class TER_PathFindRequest_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    //! @name
    //@{
             TER_PathFindRequest_ABC() {}
    virtual ~TER_PathFindRequest_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool IgnoreDynamicObjects() const = 0;
    virtual void FindPath( TER_Pathfinder_ABC& pathfind ) = 0;
    //@}
};

#endif // __TER_PathFindRequest_ABC_h_
