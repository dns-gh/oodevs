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

#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/DisplayableModesObserver_ABC.h"
#include <tools/Observer_ABC.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class ModeController;
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
               , public kernel::DisplayableModesObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             RichMenu( const QString& objectName, QWidget* parent, kernel::Controllers& controllers, const QString& title = "" );
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
    kernel::ModeController& modeController_;
    //@}
};

} //! namespace gui

#endif // __gui_RichMenu_h_
