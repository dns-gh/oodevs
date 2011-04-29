// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InitialStateDialog_h_
#define __InitialStateDialog_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Controllers;
}

class InitialStateTable_ABC;
class StaticModel;

// =============================================================================
/** @class  InitialStateDialog
    @brief  InitialStateDialog
*/
// Created: ABR 2011-02-24
// =============================================================================
class InitialStateDialog : public QDialog
                         , public tools::Observer_ABC
                         , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                         , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             InitialStateDialog( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel );
    virtual ~InitialStateDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Slots
    //@{
    enum E_Tabs { eCrew = 0, eEquipment = 1, eResources = 2 };
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    void Validate();
    void Reject();
    void Reset();
    void closeEvent( QCloseEvent * e );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const StaticModel&   staticModel_;
    kernel::SafePointer< kernel::Agent_ABC > selected_;
    std::vector< boost::shared_ptr< InitialStateTable_ABC > > tabs_;
    //@}
};

#endif // __InitialStateDialog_h_
