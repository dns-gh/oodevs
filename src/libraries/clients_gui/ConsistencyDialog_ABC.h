// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_ConsistencyDialog_ABC_h_
#define __gui_ConsistencyDialog_ABC_h_

#include "clients_kernel/VariantPointer.h"

namespace kernel
{
    class ConsistencyChecker_ABC;
}

namespace gui
{
    template< typename EnumError > class FilterProxyModel;
    template< typename T > class RichWidget;
    class RichGroupBox;
    class RichCheckBox;

// =============================================================================
/** @class  ConsistencyDialog_ABC
    @brief  ConsistencyDialog_ABC
*/
// Created: ABR 2012-06-06
// =============================================================================
class ConsistencyDialog_ABC : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ConsistencyDialog_ABC( const QString& objectName, QWidget* parent, kernel::ConsistencyChecker_ABC& checker );
    virtual ~ConsistencyDialog_ABC();
    //@}

public slots:
    //! @name Slots
    //@{
    void CheckConsistency();
    void Display();
    //@}

signals:
    //! @name Signals
    //@{
    void ClearLoadingErrors();
    //@}

protected:
    //! @name Types
    //@{
    typedef std::map< unsigned int, QString > T_Types;
    //@}

    //! @name Helpers
    //@{
    virtual void UpdateDataModel();
    void CreateCheckbox( const T_Types& names );
    void ShowTypeFilter( bool visible );
    void ShowLevelFilter( bool visible );
    //@}

protected slots:
    //! @name Slots
    //@{
    void OnRefresh();
    void OnClose();
    //@}

    //! @name Abstract Slots
    //@{
    virtual void OnSelectionChanged( const QModelIndex& ) = 0;
    virtual void OnFilterChanged( int type ) = 0;
    virtual void OnLevelChanged() = 0;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::ConsistencyChecker_ABC&     checker_;
    RichWidget< QTableView >*                      tableView_;
    QStandardItemModel*                 dataModel_;
    QSignalMapper*                      pMapper_;

    QStringList                         horizontalHeaders_;
    RichGroupBox*                       typeGroup_;
    RichGroupBox*                       levelGroup_;
    QGridLayout*                        checkBoxLayout_;
    RichCheckBox*                       warningCheckBox_;
    RichCheckBox*                       errorCheckBox_;
    std::map< unsigned int, QString >   errorDescriptions_;
    //@}
};

template< typename EnumError, typename TargetItem >
class ConsistencyDialog : public ConsistencyDialog_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ConsistencyDialog( const QString& objectName, QWidget* parent, kernel::ConsistencyChecker_ABC& checker, FilterProxyModel< EnumError >& proxyModel );
    virtual ~ConsistencyDialog();
    //@}

protected:
    //! @name Helper
    //@{
    virtual void OnFilterChanged( int type );
    virtual void OnLevelChanged();

    void AddIcon( const TargetItem& targetItem, EnumError type, QList< QStandardItem* >& items );
    void AddLabel( const TargetItem& targetItem, EnumError type, const QString& text, QList< QStandardItem* >& items );
    template< typename T >
    void AddItem( T data, QString text, const TargetItem& targetItem, EnumError type, QList< QStandardItem* >& items );
    //@}

protected:
    //! @name Member data
    //@{
    FilterProxyModel< EnumError >& proxyModel_;
    //@}
};

} //! namespace gui

#include "ConsistencyDialog_ABC.inl"

#endif // __gui_ConsistencyDialog_ABC_h_
