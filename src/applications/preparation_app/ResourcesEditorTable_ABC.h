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
    virtual void Connect();
    virtual void Disconnect();
    void SetData( int row, int col, const QVariant& value, int role = Qt::DisplayRole, bool isSortType = false, Qt::Alignment aligment = 0 );
    const kernel::DotationType* GetDotation( int row ) const;
    int GetValue( int row ) const;
    //@}

signals:
    //! @name Signals
    //@{
    void ResourceValueChanged();
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnDataChanged( const QModelIndex& index, const QModelIndex& );
    //@}

private slots:
    //! @name Slots
    //@{
    void RemoveResource( int row );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void contextMenuEvent( QContextMenuEvent* e );
    virtual void CustomizeMenuAction( QAction* action, const kernel::DotationType& actionDotation ) const;
    virtual void UpdateLine( int row, int value );
    void InitHeader();
    //@}

protected:
    //! @name Member data
    //@{
    QStandardItemModel* dataModel_;
    gui::CommonDelegate* delegate_;
    //@}

private:
    //! @name Member data
    //@{
    QStringList headers_;
    const kernel::Resolver2< kernel::DotationType >& dotations_;
    std::set< std::string > allowedSupplyClasses_;
    //@}
};

#endif // _ResourcesEditorTable_ABC_h
