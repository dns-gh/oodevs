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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Formations/Formation_Diamond.h $
// $Author: Sbo $
// $Modtime: 18/07/05 18:48 $
// $Revision: 3 $
// $Workfile: Formation_Diamond.h $
//
// *****************************************************************************

#ifndef __Formation_Diamond_h_
#define __Formation_Diamond_h_

#include "Types.h"
#include "Formation_ABC.h"

namespace TIC
{
    class Pawn;

// =============================================================================
/** @class  Formation_Diamond
    @brief  Formation_Diamond
    @par    Using example
    @code
    Formation_Diamond;
    @endcode
*/
// Created: SBO 2005-05-11
// =============================================================================
class Formation_Diamond : public Formation_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Formation_Diamond();
    virtual ~Formation_Diamond();
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
    double  rAngle_;
    double  rRadius_;
    int     nPlatformPositioned_;
    int     nPlatformPositionedOnLevel_;
    int     nPlatformPerLevel_;
    //@}

private:
    //! @name Static variables
    //@{
    static double rDistanceBetweenPlatforms_;
    //@}

};

} // end namespace TIC


#endif // __Formation_Diamond_h_
