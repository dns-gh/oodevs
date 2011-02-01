// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InhabitantChangeAffinitiesDialog_h_
#define __InhabitantChangeAffinitiesDialog_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace gui
{
    class DecimalSpinBoxAndSlider;
}

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Inhabitant_ABC;
    class Profile_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

class StaticModel;
class TeamsModel;

// =============================================================================
/** @class  InhabitantChangeAffinitiesDialog
    @brief  InhabitantChangeAffinitiesDialog
*/
// Created: ABR 2011-01-25
// =============================================================================
class InhabitantChangeAffinitiesDialog : public QDialog
                                       , public tools::Observer_ABC
                                       , public kernel::ContextMenuObserver_ABC< kernel::Inhabitant_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             InhabitantChangeAffinitiesDialog( QWidget* pParent, kernel::Controllers& controllers, const TeamsModel& teams, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~InhabitantChangeAffinitiesDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    void Validate();
    void Reject();
    void closeEvent( QCloseEvent * e );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InhabitantChangeAffinitiesDialog( const InhabitantChangeAffinitiesDialog& );            //!< Copy constructor
    InhabitantChangeAffinitiesDialog& operator=( const InhabitantChangeAffinitiesDialog& ); //!< Assignment operator
    //@}

private:
    //! @name Type
    //@{
    typedef std::map< unsigned long, gui::DecimalSpinBoxAndSlider* >  T_AffinitiesSpinBoxs;
    typedef T_AffinitiesSpinBoxs::iterator                           IT_AffinitiesSpinBoxs;
    typedef T_AffinitiesSpinBoxs::const_iterator                    CIT_AffinitiesSpinBoxs;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const TeamsModel& teams_;
    const StaticModel& static_;
    actions::ActionsModel& actionsModel_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;
    kernel::SafePointer< kernel::Inhabitant_ABC > selected_;
    QGrid* affinitiesGrid_;
    T_AffinitiesSpinBoxs affinitiesSpinboxs_;
    //@}
};

#endif // __InhabitantChangeAffinitiesDialog_h_
