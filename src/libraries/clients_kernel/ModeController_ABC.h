// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ModeController_ABC_h_
#define __ModeController_ABC_h_

#include "tools/SortedInterfaceContainer.h"
#include "tools/Observer_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  ModeController_ABC
    @brief  ModeController_ABC
*/
// Created: ABR 2012-05-09
// =============================================================================
class ModeController_ABC : protected tools::SortedInterfaceContainer< tools::Observer_ABC >
{
    friend void Controllers::ChangeMode( int newMode );

public:
    //! @name Constructors/Destructor
    //@{
    explicit ModeController_ABC() {}
    virtual ~ModeController_ABC() {}
    //@}

    //! @name Abstract operations
    //@{
    virtual int GetCurrentMode() const = 0;
    virtual const QString& GetRegisteryEntry() const = 0;
    //@}

    //! @name SortedInterfaceContainer implementation
    //@{
    void Register( tools::Observer_ABC& observer )
    {
        tools::SortedInterfaceContainer< tools::Observer_ABC >::Register( observer );
    }
    void Unregister( tools::Observer_ABC& observer )
    {
        tools::SortedInterfaceContainer< tools::Observer_ABC >::Unregister( observer );
    }
    //@}

private:
    //! @name Abstract operations
    //@{
    virtual void ChangeMode( int newMode ) = 0;
    //@}
};

} //! namespace kernel

#endif // __ModeController_ABC_h_
