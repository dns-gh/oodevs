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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Formations/Formation_ABC.h $
// $Author: Sbo $
// $Modtime: 18/07/05 18:48 $
// $Revision: 3 $
// $Workfile: Formation_ABC.h $
//
// *****************************************************************************

#ifndef __Formation_ABC_h_
#define __Formation_ABC_h_

#include "Types.h"

namespace TIC
{
    class Pawn;
    class Platform;

// =============================================================================
/** @class  Formation_ABC
    @brief  Formation_ABC
    @par    Using example
    @code
    Formation_ABC;
    @endcode
*/
// Created: SBO 2005-05-11
// =============================================================================
class Formation_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Formation_ABC();
    virtual ~Formation_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Begin  ( const Pawn&     pawn, uint nPlatformNbr ) = 0;
    virtual void ApplyTo(       Platform& platform                ) = 0;
    virtual void End    (                                         ) = 0;
    //@}

    //! @name Accessors
    //@{
    bool IsReverse() const;
    //@}

    //! @name Modifiers
    //@{
    virtual void SetDistanceInterPlatform( double rDistance ) = 0;
    //@}

protected:
    //! @name Member variables
    //@{
    const Pawn*     pPawn_;
    uint            nPlatformNbr_;

    // tell caller if platform list must be processed in normal or reverse order
    bool            bIsReverse_;
    //@}

};

} // end namespace TIC

#include "Formation_ABC.inl"


#endif // __Formation_ABC_h_
