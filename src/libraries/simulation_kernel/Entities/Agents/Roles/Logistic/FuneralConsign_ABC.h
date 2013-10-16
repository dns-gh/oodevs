// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FuneralConsign_ABC_h_
#define __FuneralConsign_ABC_h_

#include "SupplyConvoysObserver_ABC.h"

namespace logistic
{
    class SupplyConsign_ABC;

// =============================================================================
/** @class  FuneralConsign_ABC
    @brief  FuneralConsign_ABC
*/
// Created: NLD 2011-08-24
// =============================================================================
class FuneralConsign_ABC : public SupplyConvoysObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FuneralConsign_ABC() {}
    virtual ~FuneralConsign_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Update() = 0;
    virtual bool IsFinished() const = 0;

    virtual void SendChangedState() const = 0;
    virtual void SendFullState( unsigned int context ) const = 0;
    virtual void Clean() = 0;
    //@}
};

}

#endif // __FuneralConsign_ABC_h_
