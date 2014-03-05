// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "QuotasResourcesTable.h"
#include "moc_QuotasResourcesTable.cpp"
#include "clients_gui/CommonDelegate.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Tools.h"

namespace
{
    QStringList GetHeaders()
    {
        return QStringList() << tools::translate( "QuotasResourcesTable", "Resource" )
                             << tools::translate( "QuotasResourcesTable", "Quantity" );
    }
}

// -----------------------------------------------------------------------------
// Name: QuotasResourcesTable Constructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
QuotasResourcesTable::QuotasResourcesTable( const QString& objectName, QWidget* parent, const kernel::Resolver2< kernel::DotationType >& dotationsType ) 
    : ResourcesEditorTable_ABC( GetHeaders(), objectName, parent, dotationsType )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: QuotasResourcesTable Destructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
QuotasResourcesTable::~QuotasResourcesTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: QuotasResourcesTable::SetQuotas
// Created: JSR 2014-03-05
// -----------------------------------------------------------------------------
void QuotasResourcesTable::SetQuotas( const LogisticEditor::T_Requirements& stocks )
{
    Disconnect();
    OnClearItems();
    for( auto it = stocks.begin(); it != stocks.end(); ++it )
        AddResource( *it->first, it->second );
    Connect();
}
