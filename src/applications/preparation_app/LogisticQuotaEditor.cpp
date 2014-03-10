// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LogisticQuotaEditor.h"
#include "moc_LogisticQuotaEditor.cpp"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/LogisticHierarchiesBase.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: LogisticQuotaEditor constructor
// Created: SLI 2014-02-19
// -----------------------------------------------------------------------------
LogisticQuotaEditor::LogisticQuotaEditor( QWidget* parent, kernel::Controllers& controllers, const ::StaticModel& staticModel )
    : LogisticEditor( parent, "LogisticQuotaEditor", controllers, staticModel )
{
    setCaption( tools::translate( "LogisticQuotaEditor", "Quotas Edition" ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticQuotaEditor destructor
// Created: SLI 2014-02-19
// -----------------------------------------------------------------------------
LogisticQuotaEditor::~LogisticQuotaEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticQuotaEditor::SupplyHierarchy
// Created: SLI 2014-02-19
// -----------------------------------------------------------------------------
void LogisticQuotaEditor::SupplyHierarchy( const gui::LogisticHierarchiesBase& logHierarchy )
{
    std::map< const kernel::Entity_ABC*, T_Requirements > generatedQuotas;
    auto itLogChildren = logHierarchy.CreateSubordinateIterator();
    while( itLogChildren.HasMoreElements() )
    {
        const kernel::Entity_ABC& logChildren = itLogChildren.NextElement();
        if( !logistic_helpers::IsLogisticBase( logChildren ) )
            continue;
        const gui::LogisticHierarchiesBase* pLogChildrenHierarchy = logChildren.Retrieve< gui::LogisticHierarchiesBase >();
        if( !pLogChildrenHierarchy )
            continue;
        T_Requirements requirements;
        auto days = GetDaysBySupplyClass();
        for( auto it = days.begin(); it != days.end(); ++it )
            SupplyLogisticBaseStocks( pLogChildrenHierarchy->GetEntity(), *it->first, requirements );
        for( auto itRequired = requirements.begin(); itRequired != requirements.end(); ++itRequired )
        {
            const kernel::DotationType& dotationType = *itRequired->first;
            auto itDays = days.find( &dotationType.GetLogisticSupplyClass() );
            if( itDays != days.end() )
                generatedQuotas[ &logChildren ][ &dotationType ] += static_cast< unsigned int >( itDays->second * itRequired->second + 0.5 );
        }
    }
    emit DotationsQuotasComputed( generatedQuotas );
}
