// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ChangeHealthStateDialog_ABC_h_
#define __ChangeHealthStateDialog_ABC_h_

#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class ContextMenu;
    class Controllers;
    class Entity_ABC;
    class Profile_ABC;
    class Time_ABC;
}

namespace actions
{
    class Action_ABC;
    class ActionsModel;
}

// =============================================================================
/** @class  ChangeHealthStateDialog_ABC
    @brief  ChangeHealthStateDialog_ABC
*/
// Created: JSR 2011-03-15
// =============================================================================
class ChangeHealthStateDialog_ABC : public QDialog
                                  , public tools::Observer_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ChangeHealthStateDialog_ABC( QWidget* pParent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile, unsigned int numberOfLines );
    virtual ~ChangeHealthStateDialog_ABC();
    //@}

protected :
    //! @name Helpers
    //@{
    void DoNotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    void AddLine( unsigned int line, const QString& objectName, const QString& label );
    unsigned int GetSpinboxValue( unsigned int line ) const;
    const kernel::Entity_ABC& GetSelected() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    void Validate();
    void Reject();
    void closeEvent( QCloseEvent* e );
    void OnValuesChanged();
    //@}

private:
    //! @name types
    //@{
    typedef std::map< unsigned int, QSpinBox* > T_Spinboxes;
    typedef T_Spinboxes::const_iterator       CIT_Spinboxes;
    //@}

    //! @name Helpers
    //@{
    virtual void CreateMagicAction( actions::ActionsModel& actionsModel ) const = 0;
    virtual unsigned int GetValue( unsigned int index ) const = 0;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    T_Spinboxes spinboxes_;
    QGridLayout* grid_;
    QLabel* newTotalLabel_;
    QLabel* originalTotalLabel_;
    //@}
};

#endif // __ChangeHealthStateDialog_ABC_h_
