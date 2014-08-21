// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LogisticStockEditor.h"
#include "moc_LogisticStockEditor.cpp"
#include "clients_gui/LogisticHierarchiesBase.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/tools.h"
#include "preparation/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor constructor
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
LogisticStockEditor::LogisticStockEditor( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel )
    : LogisticEditor( parent, "LogisticStockEditor", controllers, staticModel )
{
    setCaption( tools::translate( "LogisticStockEditor", "Stocks Edition" ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor destructor
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
LogisticStockEditor::~LogisticStockEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticStockEditor::SupplyHierarchy
// Created: SLI 2014-02-19
// -----------------------------------------------------------------------------
void LogisticStockEditor::SupplyHierarchy( const gui::LogisticHierarchiesBase& logHierarchy )
{
    logistic_helpers::T_Requirements requirements;
    auto classes = GetDaysBySupplyClass();
    for( auto it = classes.begin(); it != classes.end(); ++it )
    {
        logistic_helpers::T_Requirements curRequirements;
        logistic_helpers::ComputeLogisticConsumptions( staticModel_, logHierarchy.GetEntity(), *it->first, curRequirements, true );
        for( auto itReq = curRequirements.begin(); itReq != curRequirements.end(); ++itReq )
            requirements[ itReq->first ] += static_cast< unsigned int >( it->second * itReq->second + 0.5 );
    }
    emit DotationsStocksComputed( requirements );
}
