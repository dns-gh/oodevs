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

#include "clients_kernel/SafePointer.h"
#include <boost/shared_ptr.hpp>
#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
    class RichView_ABC;
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
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UnitStateDialog( QWidget* parent, kernel::Controllers& controllers );
    virtual ~UnitStateDialog();
    //@}

public:
    //! @name Operations
    //@{
    void Load();
    void Purge();
    //@}

protected:
    //! @name Types
    //@{
    enum E_Tabs { eCrew = 0, eEquipment = 1, eSupplies = 2, eNbTabs = 3 };
    //@}

    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    bool IsReadOnly() const;
    template< typename View >
    void AddView( const QString& name, View* view )
    {
        tabs_.push_back( std::pair< RichView_ABC*, UnitStateTable_ABC* >( view, view->GetView() ) );
        tabWidget_->addTab( view, name );
        connect( view, SIGNAL( FiltersStatusChanged( bool ) ), clearButton_, SLOT( setEnabled( bool ) ) );
    }
    //@}

signals:
    //! @name Signals
    //@{
    void OnClearClicke();
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void Validate() const;
    virtual void Reset();
    void Show();
    void Clear();
    void CurrentTabChanged( int tab );
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QHBoxLayout* headerLayout_;
    QTabWidget* tabWidget_;
    std::vector< std::pair< RichView_ABC*, UnitStateTable_ABC* > > tabs_;  
    QPushButton* clearButton_;
    QPushButton* resetButton_;
    QPushButton* validateButton_;
    QLabel* selectedEntityLabel_;
    //@}
};
}

#endif // __gui_UnitStateDialog_h_
