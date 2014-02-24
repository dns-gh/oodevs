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

const kernel::EquipmentType::CarryingSupplyFunction* LogisticSupplyCarriersTableWidget::GetLogSupplyFunctionCarrying( int row ) const
{
    const QString name = model()->data( model()->index( row, eName ), Qt::DisplayRole ).value< QString >();
    const auto it = types_.find( name );
    if( it == types_.end() )
        return 0;
    return it->second->GetLogSupplyFunctionCarrying();
}

void LogisticSupplyCarriersTableWidget::UpdateRow( int row )
{
    if( const auto function = GetLogSupplyFunctionCarrying( row ) )
    {
        SetContent( row, eMaxMass, function->stockMaxWeightCapacity_ );
        SetContent( row, eMaxVolume, function->stockMaxVolumeCapacity_ );
    }
    Update();
}

void LogisticSupplyCarriersTableWidget::Update()
{
    for( int row = 0; row < model()->rowCount() - 1; ++row )
        if( const auto function = GetLogSupplyFunctionCarrying( row ) )
        {
            const double maxMass = model()->data( model()->index( row, eMaxMass ), Qt::UserRole ).value< double >();
            const double maxVolume = model()->data( model()->index( row, eMaxVolume ), Qt::UserRole ).value< double >();
            const auto fractions = ComputeMassVolume( function, maxMass, maxVolume );
            SetContent( row, eMass, locale().toString( 100 * fractions.first, 'f', 0 ) + "%",
                fractions.first > 1 ? tr( "The convoy is unable to carry that much weight" ) :
                fractions.first * maxMass < function->stockMinWeightCapacity_ ? tr( "The convoy is under its minimal mass threshold" ) :
                "" );
            SetContent( row, eVolume, locale().toString( 100 * fractions.second, 'f', 0 ) + "%", 
                fractions.second > 1 ? tr( "The convoy is unable to carry that much volume" ) :
                fractions.second * maxVolume < function->stockMinVolumeCapacity_ ? tr( "The convoy is under its minimal volume threshold" ) :
                "" );
        }
}

double LogisticSupplyCarriersTableWidget::ComputeMaxMass( const std::string& nature ) const
{
    double result = 0;
    for( int row = 0; row < model()->rowCount() - 1; ++row )
    {
        const auto function = GetLogSupplyFunctionCarrying( row );
        if( function && function->stockNature_ == nature )
        {
            const int quantity = model()->data( model()->index( row, eValue ), Qt::UserRole ).value< int >();
            result += quantity * function->stockMaxWeightCapacity_;
        }
    }
    return result;
}

std::pair< double, double > LogisticSupplyCarriersTableWidget::ComputeMassVolume( const kernel::EquipmentType::CarryingSupplyFunction* function, double maxMass, double maxVolume ) const
{
    if( !function )
        return std::make_pair( 0, 0 );
    const double total = ComputeMaxMass( function->stockNature_ );
    if( total == 0 )
        return std::make_pair( 0, 0 );
    double mass = 0;
    double volume = 0;
    QMap< QString, int > quantities;
    resources_.GetQuantities( quantities );
    for( auto it = quantities.begin(); it != quantities.end(); ++it )
    {
        auto it2 = dotations_.find( it.key() );
        if( it2 != dotations_.end() && it2->second.type_->GetNature() == function->stockNature_ )
        {
            mass += it.value() * it2->second.type_->GetUnitWeight();
            volume += it.value() * it2->second.type_->GetUnitVolume();
        }
    }
    return std::make_pair( mass / total, volume * maxMass / total / maxVolume );
}

bool LogisticSupplyCarriersTableWidget::IsOverloaded() const
{
    for( int row = 0; row < model()->rowCount() - 1; ++row )
    {
        const auto function = GetLogSupplyFunctionCarrying( row );
        const double maxMass = model()->data( model()->index( row, eMaxMass ), Qt::UserRole ).value< double >();
        const double maxVolume = model()->data( model()->index( row, eMaxVolume ), Qt::UserRole ).value< double >();
        const auto fractions = ComputeMassVolume( function, maxMass, maxVolume );
        if( fractions.first > 1 || fractions.second > 1 )
            return true;
    }
    return false;
}

bool LogisticSupplyCarriersTableWidget::IsUnderloaded() const
{
    for( int row = 0; row < model()->rowCount() - 1; ++row )
    {
        const auto function = GetLogSupplyFunctionCarrying( row );
        const double maxMass = model()->data( model()->index( row, eMaxMass ), Qt::UserRole ).value< double >();
        const double maxVolume = model()->data( model()->index( row, eMaxVolume ), Qt::UserRole ).value< double >();
        const auto fractions = ComputeMassVolume( function, maxMass, maxVolume );
        if( fractions.first * maxMass < function->stockMinWeightCapacity_ ||
            fractions.second * maxVolume < function->stockMinVolumeCapacity_ )
            return true;
    }
    return false;
}

bool LogisticSupplyCarriersTableWidget::IsIncomplete() const
{
    QMap< QString, int > quantities;
    resources_.GetQuantities( quantities );
    for( auto it = quantities.begin(); it != quantities.end(); ++it )
    {
        auto it2 = dotations_.find( it.key() );
        bool found = false;
        for( int row = 0; row < model()->rowCount() - 1 && !found; ++row )
        {
            const auto function = GetLogSupplyFunctionCarrying( row );
            if( function && it2->second.type_->GetNature() == function->stockNature_ )
                found = true;
        }
        if( !found )
            return true;
    }
    return false;
}
