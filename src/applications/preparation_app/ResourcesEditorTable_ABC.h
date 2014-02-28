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
             ResourcesEditorTable_ABC( const QString& objectName, QWidget* parent, const kernel::Resolver2< kernel::DotationType >& dotationsType );
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
    void AddLine( int );
    void OnRemoveCurrentItem();
    void OnClearItems();
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
    virtual void AddResource( const kernel::DotationType& resource, double value = 0 ) = 0;
    virtual void InitHeader() = 0;
    virtual void OnValueChanged( int row, double value ) = 0;
    QStandardItemModel* GetDataModel() const;
    void ComputeValueByDotation( std::map< const kernel::DotationType*, double >& result ) const;
    void SetAllowedNatures( const std::set< std::string >& allowedNatures );
    //@}

protected:
    //! @name Member data
    //@{
    QStandardItemModel* dataModel_;
    gui::CommonDelegate* delegate_;
    kernel::ContextMenu* popupMenu_;
    QMenu* resourcesMenu_;
    QModelIndex possiblyToRemove_;
    const kernel::Resolver2< kernel::DotationType >& dotations_;
    std::set< std::string > allowedNatures_;
    //@}
};

#endif // _ResourcesEditorTable_ABC_h
