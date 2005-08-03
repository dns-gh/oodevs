// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-17 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/PawnPlatform.h $
// $Author: Sbo $
// $Modtime: 24/06/05 17:22 $
// $Revision: 1 $
// $Workfile: PawnPlatform.h $
//
// *****************************************************************************

#ifndef __PawnPlatform_h_
#define __PawnPlatform_h_

#include "Types.h"

namespace TIC
{
    class PlatformType;

// =============================================================================
/** @class  PawnPlatform
    @brief  PawnPlatform
    @par    Using example
    @code
    PawnPlatform;
    @endcode
*/
// Created: SBO 2005-05-11
// =============================================================================
class PawnPlatform
{
    MT_COPYNOTALLOWED( PawnPlatform );

public:
    //! @name Constructors/Destructor
    //@{
             PawnPlatform( const PlatformType& platformType, bool bIsEmbarquable );
    virtual ~PawnPlatform();
    //@}

    //! @name Accessors
    //@{
    const PlatformType& GetType      () const;
    bool                IsEmbarquable() const;
    //@}

private:
    //! @name Member data
    //@{
    const PlatformType* pType_;
          bool          bIsEmbarquable_;
    //@}
};

} // end namespace TIC

#include "PawnPlatform.inl"

#endif // __PawnPlatform_h_
