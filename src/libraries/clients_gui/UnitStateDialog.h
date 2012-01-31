// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __gui_UnitStateDialog_h_
#define __gui_UnitStateDialog_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "clients_kernel/SafePointer.h"
#include "tools/SelectionObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
    class UnitStateTable_ABC;

// =============================================================================
/** @class  UnitStateDialog
    @brief  UnitStateDialog
*/
// Created: ABR 2011-07-05
// =============================================================================
class UnitStateDialog : public QDialog
                      , public tools::Observer_ABC
                      , public tools::SelectionObserver< kernel::Entity_ABC >
                      , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UnitStateDialog( QWidget* parent, kernel::Controllers& controllers );
    virtual ~UnitStateDialog();
    //@}

protected:
    //! @name Types
    //@{
    enum E_Tabs { eCrew = 0, eEquipment = 1, eResources = 2, eNbTabs = 3 };
    //@}

    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    bool IsReadOnly() const;
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void Validate() const;
    virtual void Reset();
    void Show();
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers&                                   controllers_;
    kernel::SafePointer< kernel::Entity_ABC >              selected_;
    QTabWidget*                                            tabWidget_;
    QGridLayout*                                           buttons_;
    std::vector< boost::shared_ptr< UnitStateTable_ABC > > tabs_;
    QPushButton*                                           resetButton_;
    QPushButton*                                           validateButton_;
    QLabel*                                                selectedEntityLabel_;
    //@}
};
}

#endif // __gui_UnitStateDialog_h_
