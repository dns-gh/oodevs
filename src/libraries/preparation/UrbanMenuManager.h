// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanMenuManager_h_
#define __UrbanMenuManager_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include "clients_kernel/MultipleSelectionObserver_ABC.h"
#include <boost/noncopyable.hpp>
#include <tools/Observer_ABC.h>

namespace kernel
{
    class Controllers;
    class UrbanObject_ABC;
}

class UrbanModel;

// =============================================================================
/** @class  UrbanMenuManager
    @brief  UrbanMenuManager
*/
// Created: JSR 2012-06-07
// =============================================================================
class UrbanMenuManager : public QObject
                       , public tools::Observer_ABC
                       , public kernel::MultipleSelectionObserver< kernel::UrbanObject_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::UrbanObject_ABC >
                       , public kernel::ModesObserver_ABC
                       , private boost::noncopyable
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             UrbanMenuManager( kernel::Controllers& controllers, UrbanModel& model );
    virtual ~UrbanMenuManager();
    //@}

    //! @name Operations
    //@{
    void Register();
    void Unregister();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreateDistrict();
    void OnDelete();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelectionChanged( const T_Elements& elements );
    virtual void NotifyContextMenu( const kernel::UrbanObject_ABC& object, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    UrbanModel& model_;
    T_Elements selected_;
    //@}
};

#endif // __UrbanMenuManager_h_
