// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ReadOnlyModable_h_
#define __ReadOnlyModable_h_

#include "ModesObserver_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  ReadOnlyModable
    @brief  ReadOnlyModable
*/
// Created: ABR 2012-08-13
// =============================================================================
class ReadOnlyModable : public ModesObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ReadOnlyModable() : readOnlyModes_( 0 ) {}
    virtual ~ReadOnlyModable() {}
    //@}

    //! @name Operations
    //@{
    bool IsReadOnly() const
    {
        return ( readOnlyModes_ & GetCurrentMode() ) != 0;
    }

    void SetReadOnlyModes( int modes )
    {
        readOnlyModes_ = modes;
    }
    //@}

private:
    //! @name Member data
    //@{
    int readOnlyModes_;
    //@}
};

} //! namespace kernel

#endif // __ReadOnlyModable_h_
