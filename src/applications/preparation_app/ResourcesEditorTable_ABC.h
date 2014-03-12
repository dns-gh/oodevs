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
    class Entity_ABC;
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
    void ComputeValueByDotation( std::map< const kernel::DotationType*, unsigned int >& result ) const;
    void UpdateAllowedSupplyClasses( const kernel::Entity_ABC& entity );
    //@}

public slots:
    //! @name Slots
    //@{
    virtual void OnClearItems();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void AddResource( const kernel::DotationType& resource, int value = 0 );
    void Connect();
    void Disconnect();
    void SetData( int row, int col, const QVariant& value, int role = Qt::DisplayRole, bool isSortType = false, Qt::Alignment aligment = 0 );
    const kernel::DotationType* GetDotation( int row ) const;
    //@}

signals:
    //! @name Signals
    //@{
    void ResourceValueChanged();
    //@}

private slots:
    //! @name Slots
    //@{
    void RemoveResource( int row );
    void OnDataChanged( const QModelIndex& index, const QModelIndex& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void contextMenuEvent( QContextMenuEvent* e );
    virtual void CustomizeMenuAction( QAction* action, const kernel::DotationType& actionDotation ) const;
    virtual void UpdateLine( int row, int value );
    void InitHeader();
    int GetValue( int row ) const;
    //@}

private:
    //! @name Member data
    //@{
    QStringList headers_;
    QStandardItemModel* dataModel_;
    const kernel::Resolver2< kernel::DotationType >& dotations_;
    std::set< std::string > allowedSupplyClasses_;
    //@}
};

#endif // _ResourcesEditorTable_ABC_h
