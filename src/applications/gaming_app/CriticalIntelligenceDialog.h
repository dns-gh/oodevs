// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __CriticalIntelligenceDialog_h_
#define __CriticalIntelligenceDialog_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Agent_ABC;
    class Entity_ABC;
    class Population_ABC;
    class Controllers;
    class Profile_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

class StaticModel;

// =============================================================================
/** @class  CriticalIntelligenceDialog
    @brief  Critical intelligence dialog
*/
// Created: LGY 2011-05-27
// =============================================================================
class CriticalIntelligenceDialog : public QDialog
                                 , private boost::noncopyable
                                 , public tools::Observer_ABC
                                 , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                                 , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             CriticalIntelligenceDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel,
                                         actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~CriticalIntelligenceDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void Validate();
    void Reject();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;
    actions::ActionsModel& actionsModel_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QLineEdit* value_;
    //@}
};

#endif // __CriticalIntelligenceDialog_h_
