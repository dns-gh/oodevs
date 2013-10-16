// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ContextMenuObserver_ABC_h_
#define __ContextMenuObserver_ABC_h_

#include "ContextMenu.h"
#include <tools/Observer_ABC.h>

namespace kernel
{
// =============================================================================
/** @class  ContextMenuObserver_ABC
    @brief  ContextMenuObserver_ABC
*/
// Created: AGE 2006-02-27
// =============================================================================
template< typename Element >
class ContextMenuObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ContextMenuObserver_ABC() {}
    virtual ~ContextMenuObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const Element&, ContextMenu& ) = 0;
    //@}
};

}

#endif // __ContextMenuObserver_ABC_h_
