// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_RichDockWidget_h_
#define __gui_RichDockWidget_h_

#include "clients_kernel/DisplayableModesObserver_ABC.h"
#include <tools/Observer_ABC.h>

namespace kernel
{
    class Controllers;
}

namespace gui
{

// =============================================================================
/** @class  RichDockWidget
    @brief  RichDockWidget
*/
// Created: ABR 2012-05-11
// =============================================================================
class RichDockWidget : public QDockWidget
                     , public tools::Observer_ABC
                     , public kernel::DisplayableModesObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             RichDockWidget( kernel::Controllers& controllers, QWidget* parent, const QString& objectName, const QString& windowTitle = "" );
    virtual ~RichDockWidget();
    //@}

    //! @name Operations
    //@{
    virtual void SetVisible( bool visible );
    virtual void ForceEnabled( bool enabled );
    virtual void EnsureIsEnabled();
    virtual bool IsVisible() const;
    virtual void SetMenuVisibility( bool windowMenuVisibility );
    virtual void NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode );
    virtual void SetReadOnly( bool readOnly );
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    bool windowMenuVisibility_;
    //@}
};

} //! namespace gui

#endif // __gui_RichDockWidget_h_
