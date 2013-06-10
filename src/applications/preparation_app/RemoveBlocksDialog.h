// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __RemoveBlocksDialog_h_
#define __RemoveBlocksDialog_h_

#include "clients_kernel/ActionController.h"
#include "clients_kernel/MultipleSelectionObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class UrbanObject_ABC;
}

namespace gui
{
    class RichPushButton;
    class RichSpinBox;
    template< typename T > class RichWidget;
}

class UrbanModel;

// =============================================================================
/** @class  RemoveBlocksDialog
    @brief  RemoveBlocksDialog
*/
// Created: ABR 2012-06-13
// =============================================================================
class RemoveBlocksDialog : public QDialog
                         , public tools::Observer_ABC
                         , public kernel::MultipleSelectionObserver< kernel::UrbanObject_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             RemoveBlocksDialog( QWidget* parent, kernel::Controllers& controllers, UrbanModel& model );
    virtual ~RemoveBlocksDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged( int value );
    void OnSelectionChanged( const QModelIndex& );
    void OnItemChanged( QStandardItem* );
    void OnLinkActivated( const QString& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* event );
    virtual void accept();
    virtual void NotifySelectionChanged( const T_Elements& elements );
    template< typename T >
    void AddItem( QList< QStandardItem* >& items, T sortElement, const QString& displayText, const kernel::UrbanObject_ABC& entity, bool checkable = false );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&                    controllers_;
    UrbanModel&                             urbanModel_;
    kernel::GraphicalEntity_ABC::T_GraphicalEntities selectables_;
    T_Elements                              urbanObjects_;
    gui::RichSpinBox*                       sizeEditor_;
    gui::RichPushButton*                    deleteButton_;
    QWidget*                                listWidget_;
    gui::RichWidget< QTableView >*                             tableView_;
    QStandardItemModel*                     dataModel_;
    QStringList                             horizontalHeaders_;
    bool                                    processing_;
    //@}
};

#endif // __RemoveBlocksDialog_h_
