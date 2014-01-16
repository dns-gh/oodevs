// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticSupplyCarriersTableWidget.h"
#include "moc_LogisticSupplyCarriersTableWidget.cpp"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/DotationType.h"
#include "gaming/Dotation.h"

namespace
{
    enum eColumnType
    {
        eName,
        eAvailable,
        eValue,
        eMaxMass,
        eMaxVolume,
        eMass,
        eVolume
    };
}

LogisticSupplyCarriersTableWidget::LogisticSupplyCarriersTableWidget( QWidget* parent, const QStringList& header,
    const T_CarrierTypes& types, const LogisticSupplyAvailabilityTableWidget& resources, const T_AvailableDotations& dotations )
    : LogisticSupplyAvailabilityTableWidget( parent, header )
    , types_( types )
    , resources_( resources )
    , dotations_( dotations )
{
    connect( this, SIGNAL( OnChanged( int ) ), SLOT( UpdateRow( int ) ) );
}

void LogisticSupplyCarriersTableWidget::UpdateRow( int row )
{
    const QString newName = model()->data( model()->index( row, eName ), Qt::DisplayRole ).value< QString >();
    const auto it = types_.find( newName );
    if( it != types_.end() )
    {
        const auto function = it->second->GetLogSupplyFunctionCarrying();
        if( function )
        {
            SetContent( row, eMaxMass, function->stockMaxWeightCapacity_ );
            SetContent( row, eMaxVolume, function->stockMaxVolumeCapacity_ );
        }
    }
    Update();
}

void LogisticSupplyCarriersTableWidget::Update()
{
    const double total = ComputeTotalMass();
    const double load = ComputeLoad();
    for( int row = 0; row < model()->rowCount(); ++row )
    {
        const QString name = model()->data( model()->index( row, eName ), Qt::DisplayRole ).value< QString >();
        const auto it = types_.find( name );
        if( it == types_.end() )
            continue;
        const double percent = load * 100 / total;
        SetContent( row, eMass, locale().toString( percent ) + "%" );
        SetContent( row, eVolume, "-" ); // $$$$ MCO 2014-01-15: compute volume based on mass
    }
}

double LogisticSupplyCarriersTableWidget::ComputeTotalMass() const
{
    double result = 0;
    for( int row = 0; row < model()->rowCount(); ++row )
    {
        const QString name = model()->data( model()->index( row, eName ), Qt::DisplayRole ).value< QString >();
        const int quantity = model()->data( model()->index( row, eValue ), Qt::UserRole ).value< int >();
        if( !name.isEmpty() && quantity > 0 )
        {
            const auto it = types_.find( name );
            if( it == types_.end() )
                continue;
            const auto function = it->second->GetLogSupplyFunctionCarrying();
            if( !function )
                continue;
            //function->stockNature_
            result += quantity * function->stockMaxVolumeCapacity_; // $$$$ MCO 2014-01-15: filter on nature
        }
    }
    return result;
}

double LogisticSupplyCarriersTableWidget::ComputeLoad() const // $$$$ MCO 2014-01-15: filter on nature
{
    double result = 0;
    QMap< QString, int > quantities;
    resources_.GetQuantities( quantities );
    for( auto it = quantities.begin(); it != quantities.end(); ++it )
    {
        auto it2 = dotations_.find( it.key() );
        if( it2 != dotations_.end() )
            result += it.value() * it2->second.type_->GetUnitWeight(); // $$$$ MCO 2014-01-16: GetNature here too
    }
    return result;
}
