// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_RichAction_h_
#define __gui_RichAction_h_

#include "tools/Observer_ABC.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include <boost/noncopyable.hpp>

class QAction;

namespace kernel
{
    class Controllers;
}

namespace gui
{

// =============================================================================
/** @class  RichAction
    @brief  RichAction
*/
// Created: ABR 2012-05-14
// =============================================================================
class RichAction : public tools::Observer_ABC
                 , public kernel::ModesObserver_ABC
                 , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             RichAction( QAction* action, kernel::Controllers& controllers );
    virtual ~RichAction();
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
    QAction*             action_;
    kernel::Controllers& controllers_;
    //@}
};

} //! namespace gui


#endif // __gui_RichAction_h_
