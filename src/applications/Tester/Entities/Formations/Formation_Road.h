// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Formations/Formation_Road.h $
// $Author: Sbo $
// $Modtime: 18/07/05 18:48 $
// $Revision: 2 $
// $Workfile: Formation_Road.h $
//
// *****************************************************************************

#ifndef __Formation_Road_h_
#define __Formation_Road_h_

#include "Types.h"
#include "Formation_ABC.h"

namespace TIC
{
    class Pawn;

// =============================================================================
/** @class  Formation_Road
    @brief  Formation_Road
    @par    Using example
    @code
    Formation_Road;
    @endcode
*/
// Created: SBO 2005-05-11
// =============================================================================
class Formation_Road : public Formation_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Formation_Road();
    virtual ~Formation_Road();
    //@}

    //! @name Operations
    //@{
    virtual void Begin  ( const Pawn&     pawn, uint nPlatformNbr );
    virtual void ApplyTo(       Platform& platform                );
    virtual void End    (                                         );
    //@}

    //! @name Modifiers
    //@{
    virtual void SetDistanceInterPlatform( double rDistance );
    //@}

private:
    //! @name Member variables
    //@{
    uint      nPlatformPositioned_;
    Platform* pLastPlatform_;
    //@}

private:
    //! @name Static variables
    //@{
    static double rDistanceBetweenPlatforms_;
    //@}

};

} // end namespace TIC


#endif // __Formation_Road_h_
