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
#include "clients_gui/CommonDelegate.h"
#include "clients_gui/LogisticBase.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/LogisticHierarchiesBase.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichWidget.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DotationCapacityType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/LogisticSupplyClass.h"
#include "clients_kernel/tools.h"
#include "preparation/Dotation.h"
#include "preparation/LogisticBaseStates.h"
#include "preparation/StaticModel.h"
#include "preparation/Stocks.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticQuotaEditor constructor
// Created: SLI 2014-02-19
// -----------------------------------------------------------------------------
LogisticQuotaEditor::LogisticQuotaEditor( QWidget* parent, Controllers& controllers, const ::StaticModel& staticModel )
    : LogisticEditor( parent, "LogisticQuotaEditor", controllers, staticModel )
{
    setCaption( tools::translate( "StocksEditionDialog", "Quotas Edition" ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticQuotaEditor destructor
// Created: SLI 2014-02-19
// -----------------------------------------------------------------------------
LogisticQuotaEditor::~LogisticQuotaEditor()
{
    // NOTHING
}

namespace
{
    bool HasLogisticBaseSubordinate( const kernel::Entity_ABC& entity )
    {
        const gui::LogisticHierarchiesBase* logHierarchy = entity.Retrieve< gui::LogisticHierarchiesBase >();
        if( !logHierarchy )
            return false;
        if( !logistic_helpers::IsLogisticBase( entity ) )
            return false;
        auto itLogChildren = logHierarchy->CreateSubordinateIterator();
        while( itLogChildren.HasMoreElements() )
        {
            if( logistic_helpers::IsLogisticBase( itLogChildren.NextElement() ) )
                return true;
        }
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticQuotaEditor::SupplyHierarchy
// Created: SLI 2014-02-19
// -----------------------------------------------------------------------------
void LogisticQuotaEditor::SupplyHierarchy( const kernel::Entity_ABC& /*entity*/, const gui::LogisticHierarchiesBase& logHierarchy )
{
    std::map< const kernel::Entity_ABC*, T_Requirements > generatedQuotas;
    auto itLogChildren = logHierarchy.CreateSubordinateIterator();
    while( itLogChildren.HasMoreElements() )
    {
        const Entity_ABC& logChildren = itLogChildren.NextElement();
        if( logistic_helpers::IsLogisticBase( logChildren ) )
        {
            const gui::LogisticHierarchiesBase* pLogChildrenHierarchy = logChildren.Retrieve< gui::LogisticHierarchiesBase >();
            if( pLogChildrenHierarchy )
            {
                T_Requirements requirements;
                auto itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
                for( int row = 0; itLogClass.HasMoreElements(); ++row )
                {
                    const kernel::LogisticSupplyClass& supplyClass = itLogClass.NextElement();
                    if( dataModel_->item( row )->checkState() == Qt::Checked )
                        SupplyLogisticBaseStocks( pLogChildrenHierarchy->GetEntity(), supplyClass, requirements );
                }
                SetQuotas( *pLogChildrenHierarchy, requirements, generatedQuotas[ &logChildren ] );
            }
        }
    }
    emit DotationsQuotasComputed( generatedQuotas );
}

// -----------------------------------------------------------------------------
// Name: LogisticQuotaEditor::SetQuotas
// Created: MMC 2012-03-23
// -----------------------------------------------------------------------------
void LogisticQuotaEditor::SetQuotas( const gui::LogisticHierarchiesBase& logHierarchy, const T_Requirements& requirements, T_Requirements& generatedQuotas )
{
    const LogisticBaseStates* pBaseStates = dynamic_cast< const LogisticBaseStates* >( &logHierarchy );
    if( !pBaseStates )
        return;

    for( auto itRequired = requirements.begin(); itRequired != requirements.end(); ++itRequired )
    {
        const DotationType& dotationType = *itRequired->first;
        auto itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
        for( int row = 0; itLogClass.HasMoreElements(); ++row )
        {
            const kernel::LogisticSupplyClass& supplyClass = itLogClass.NextElement();
            if( row < dataModel_->rowCount() && dataModel_->item( row )->checkState() == Qt::Checked )
            {
                if( supplyClass.GetId() == dotationType.GetLogisticSupplyClass().GetId() )
                {
                    const unsigned int quantity = GetQuantity( *dataModel_, row, itRequired->second );
                    generatedQuotas[ &dotationType ] += quantity;
                }
            }
        }
    }
}
