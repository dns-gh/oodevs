// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_RichMenu_h_
#define __gui_RichMenu_h_

#include <boost/noncopyable.hpp>

#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include "tools/Observer_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{

// =============================================================================
/** @class  RichMenu
    @brief  RichMenu
*/
// Created: ABR 2012-05-14
// =============================================================================
class RichMenu : public kernel::ContextMenu
               , public tools::Observer_ABC
               , public kernel::ModesObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             RichMenu( QWidget* parent, kernel::Controllers& controllers, const QString& title = "" );
    virtual ~RichMenu();
    //@}

    //! @name Operations
    //@{
    virtual void SetVisible( bool visible );
    virtual void ForceEnabled( bool enabled );
    virtual void EnsureIsEnabled();
    virtual bool IsVisible() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

} //! namespace gui

#endif // __gui_RichMenu_h_
