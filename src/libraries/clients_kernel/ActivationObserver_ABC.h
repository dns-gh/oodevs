// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActivationObserver_ABC_h_
#define __ActivationObserver_ABC_h_

#include <tools/Observer_ABC.h>

namespace kernel
{

// =============================================================================
/** @class  ActivationObserver_ABC
    @brief  ActivationObserver_ABC
*/
// Created: AGE 2006-03-22
// =============================================================================
template< typename T >
class ActivationObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ActivationObserver_ABC() {};
    virtual ~ActivationObserver_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void NotifyActivated( const T& element ) = 0;
    //@}
};

}

#endif // __ActivationObserver_ABC_h_
