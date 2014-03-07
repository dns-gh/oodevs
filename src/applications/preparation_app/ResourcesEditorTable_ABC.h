// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef _ResourcesEditorTable_ABC_h
#define _ResourcesEditorTable_ABC_h

#include "clients_gui/RichWidget.h"
#include "clients_kernel/Resolver2.h"

namespace gui
{
    class CommonDelegate;
}

namespace kernel
{
    class ContextMenu;
    class DotationType;
}

// =============================================================================
/** @class  ResourcesEditorTable_ABC
    @brief  ResourcesEditorTable_ABC
*/
// Created: MMC 2013-10-24
// =============================================================================
class ResourcesEditorTable_ABC : public gui::RichWidget< QTableView >
{
    Q_OBJECT
    
public:
    //! @name Constructors/Destructor
    //@{
             ResourcesEditorTable_ABC( const QStringList& headers, const QString& objectName, QWidget* parent, const kernel::Resolver2< kernel::DotationType >& dotationsType );
    virtual ~ResourcesEditorTable_ABC();
    //@}

public:
    //! @name Operations
    //@{
    virtual void contextMenuEvent( QContextMenuEvent* e );
    void Connect();
    void Disconnect();
    //@}

public slots:
    //! @name Slots
    //@{
    virtual void OnClearItems();
    void RemoveResource( int row );
    void OnDataChanged( const QModelIndex& index, const QModelIndex& );
    //@}

signals:
    //! @name Signals
    //@{
    void ResourceValueChanged();
    //@}

public:
    //! @name Helpers
    //@{
    virtual void AddResource( const kernel::DotationType& resource, int value = 0 );
    virtual void UpdateLine( int row, int value );
    void ComputeValueByDotation( std::map< const kernel::DotationType*, unsigned int >& result ) const;

protected:
    //! @name Helpers
    //@{
    virtual void CustomizeMenuAction( QAction* action, const kernel::DotationType& actionDotation ) const;
    void SetData( int row, int col, const QVariant& value, bool isSortType = false, int role = Qt::DisplayRole, Qt::Alignment aligment = 0 );
    const kernel::DotationType* GetDotation( int row ) const;
    int GetValue( int row ) const;

private:
    void InitHeader();
    //@}

private:
    //! @name Member data
    //@{
    QStringList headers_;
    QStandardItemModel* dataModel_;
    const kernel::Resolver2< kernel::DotationType >& dotations_;
    //@}
};

#endif // _ResourcesEditorTable_ABC_h
