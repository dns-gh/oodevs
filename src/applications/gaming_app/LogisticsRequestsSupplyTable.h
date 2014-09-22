// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticsRequestsSupplyTable_h_
#define __LogisticsRequestsSupplyTable_h_

#include "clients_kernel/Tools.h"
#include "clients_gui/CommonDelegate.h"
#include "clients_gui/RichTableView.h"

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
    class LinkItemDelegate;
}

class LogisticsConsign_ABC;

// =============================================================================
/** @class  LogisticsRequestsSupplyTable
    @brief  LogisticsRequestsSupplyTable
*/
// Created: MMC 2013-09-11
// =============================================================================
class LogisticsRequestsSupplyTable : public gui::RichTableView
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticsRequestsSupplyTable( const QString& objectName, QWidget* parent );
    virtual ~LogisticsRequestsSupplyTable();
    //@}

public:
    //! @name Operations
    //@{
    virtual void AddRecipientResource( const kernel::Entity_ABC& entity,
                                       const QString& recipient,
                                       const QString& resource,
                                       unsigned int requested,
                                       unsigned int granted,
                                       unsigned int conveyed,
                                       bool delivered );
    //@}

    //! @name Accessors
    //@{
    const gui::LinkItemDelegate* GetLinkItemDelegate() const;
    void UpdateRecipient( const kernel::Entity_ABC& entity, const QString& name );
    //@}

private:
    //! @name Operations
    //@{
    QStandardItem* SetData( int row, int col, QVariant text, bool checkable = false, bool checked = false );
    //@}

private:
    //! @name Data Members
    //@{
    gui::LinkItemDelegate* linkItemDelegate_;
    //@}
};

#endif // __LogisticsRequestsSupplyTable_h_
