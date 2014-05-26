// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UnitStateTableResource.h"
#include "moc_UnitStateTableResource.cpp"
#include "RichView_ABC.h"
#include "protocol/Protocol.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource constructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTableResource::UnitStateTableResource( const QString& objectName,
                                                QWidget* parent,
                                                const QString maximalCapacityLabel,
                                                kernel::Controllers& controllers )
    : UnitStateTable_ABC( objectName, parent, controllers,
                          QStringList() << tr( "Supplies" )
                                        << tr( "Category" )
                                        << tr( "Quantity" )
                                        << maximalCapacityLabel
                                        << tr( "Percentage (%)" )
                                        << tr( "Logistic low threshold (%)" )
                                        << tr( "Logistic high threshold (%)" )
                                        << tr( "Normalized quantity" ) )
    , blockSlots_( false )
{
    delegate_.AddDoubleSpinBoxOnColumn( eLowThreshold, 0, 100, 0.5 );
    delegate_.AddDoubleSpinBoxOnColumn( eHighThreshold, 0, 100, 0.5 );
    delegate_.AddDoubleSpinBoxOnColumn( ePercentage, 0, std::numeric_limits< double >::max(), 0.5 );
    connect( &dataModel_, SIGNAL( itemChanged( QStandardItem* ) ), SLOT( OnItemChanged( QStandardItem* ) ) );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource destructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTableResource::~UnitStateTableResource()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::IsReadOnlyForType
// Created: JSR 2012-03-22
// -----------------------------------------------------------------------------
bool UnitStateTableResource::IsReadOnlyForType( const std::string& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::MergeLine
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateTableResource::MergeLine( const QString& name, const QString& category, unsigned quantity, unsigned maximum,
                                        double lowThreshold, double highThreshold, double consumption )
{
    for( int row = 0; row < dataModel_.rowCount(); ++row )
        if( GetDisplayData( row, eName ) == name )
        {
            quantity += GetUserData( row, eQuantity ).toUInt();
            maximum += GetUserData( row, eMaximum ).toUInt();
            consumption += GetUserData( row, eConsumption ).toDouble();
            const QString strNormalizedQuantity = consumption == 0 ?
                tr( "N/A" ) : locale().toString( quantity / consumption );
            SetData( row, eMaximum, locale().toString( maximum ), maximum );
            SetData( row, eQuantity, locale().toString( quantity ), quantity );
            SetData( row, eConsumption, strNormalizedQuantity, consumption );
            return;
        }
    AddLine( name, category, quantity, maximum, lowThreshold, highThreshold, consumption );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::AddLine
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTableResource::AddLine( const QString& name, const QString& category, unsigned quantity /* = 0*/, unsigned maximum /* = 0*/,
                                      double lowThreshold /* = 0.*/, double highThreshold /* = 0.*/, double consumption /* = 0.*/ )
{
    int row = dataModel_.rowCount();
    AddItem( row, eName, name, name );
    AddItem( row, eCategory, category, category );
    AddItem( row, ePercentage, "", "", Qt::ItemIsEditable );
    AddItem( row, eMaximum, locale().toString( maximum ), maximum );
    AddItem( row, eQuantity, locale().toString( quantity ), quantity, Qt::ItemIsEditable );
    AddItem( row, eLowThreshold, locale().toString( lowThreshold, 'f', 2 ), lowThreshold, aggregated_ ? Qt::ItemFlags( 0 ) : Qt::ItemIsEditable );
    AddItem( row, eHighThreshold, locale().toString( highThreshold, 'f', 2 ), highThreshold, aggregated_ ? Qt::ItemFlags( 0 ) : Qt::ItemIsEditable );
    const QString strNormalizedQuantity = consumption == 0 ?
        tr( "N/A" ) : locale().toString( quantity / consumption );
    AddItem( row, eConsumption, strNormalizedQuantity, consumption );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::OnItemChanged
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTableResource::OnItemChanged( QStandardItem* item )
{
    if( blockSlots_ || !item )
        return;
    blockSlots_ = true;
    try
    {
        int quantity = 0;
        int maximum = 0;
        bool changed = false;
        if( item->column() == ePercentage && dataModel_.item( item->row(), eQuantity ) && dataModel_.item( item->row(), eMaximum ) )
        {
            double percentage = 0.01 * GetUserData( item->row(), ePercentage ).toDouble();
            maximum = GetUserData( item->row(), eMaximum ).toInt();
            quantity = static_cast< int >( maximum * percentage );
            int oldQuantity = GetUserData( item->row(), eQuantity ).toInt();
            double oldPercentage = oldQuantity / static_cast< double >( maximum );
            if( static_cast< int >( 10000 * percentage ) != static_cast< int >( 10000 * oldPercentage ) )
            {
                SetData( item->row(), eQuantity, locale().toString( quantity ), quantity );
                UpdateColor( item, quantity, maximum );
                UpdateNormalizedQuantity( item->row(), quantity );
            }
        }
        else if( item->column() == eQuantity )
        {
            changed = true;
            quantity = GetUserData( item->row(), eQuantity ).toInt();
            maximum = GetUserData( item->row(), eMaximum ).toInt();
            double percentage = maximum ? ( quantity * 100. / static_cast< double >( maximum ) ) : 0;
            SetData( item->row(), ePercentage, locale().toString( percentage, 'f', 2 ), percentage );
            UpdateColor( item, quantity, maximum );
            UpdateNormalizedQuantity( item->row(), quantity );
        }
    }
    catch( ... )
    {
        blockSlots_ = false;
        throw;
    }
    blockSlots_ = false;
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::UpdateColor
// Created: LDC 2014-02-17
// -----------------------------------------------------------------------------
void UnitStateTableResource::UpdateColor( QStandardItem* item, int quantity, int maximum )
{
    if( quantity <= maximum )
    {
        SetColor( item->row(), eMaximum, item->background(), -1 );
        SetColor( item->row(), ePercentage, item->background(), -1 );
    }
    else
    {
        SetColor( item->row(), eMaximum, Qt::gray, 0 );
        SetColor( item->row(), ePercentage, Qt::gray, 0 );
    }
    if( maximum == 0 )
    {
        QStandardItem* itemPercentage = dataModel_.item( item->row(), ePercentage );
        if( itemPercentage )
            itemPercentage->setFlags( Qt::ItemIsSelectable );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::UpdateNormalizedQuantity
// Created: JSR 2014-02-26
// -----------------------------------------------------------------------------
void UnitStateTableResource::UpdateNormalizedQuantity( int row, int quantity )
{
    if( dataModel_.item( row, eConsumption ) == 0 )
        return;
    const double consumption = GetUserData( row, eConsumption ).toDouble();
    const QString strNormalizedQuantity = consumption == 0 ?
        tr( "N/A" ) : locale().toString( quantity / consumption );
    SetData( row, eConsumption, strNormalizedQuantity, consumption );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::NeedClearFilterButton
// Created: ABR 2014-04-29
// -----------------------------------------------------------------------------
bool UnitStateTableResource::NeedClearButton() const
{
    return true;
}

namespace
{
    std::string ContentExtractor( const QStandardItem& item, bool& valid, bool& empty )
    {
        valid = true;
        empty = false;
        return item.text().toStdString();
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::CreateFilters
// Created: ABR 2014-04-25
// -----------------------------------------------------------------------------
void UnitStateTableResource::CreateFilters( gui::RichView_ABC& richView )
{
    richView.AddCheckComboBoxFilter( eCategory,
                                     RichView_ABC::eFilterAreas_Menu,
                                     "",
                                     false,
                                     true,
                                     &ContentExtractor,
                                     boost::bind( &CheckComboBox::FillFromEnum< sword::DotationType >, _1, sword::DotationType_ARRAYSIZE, &ENT_Tr::ConvertFromDotationType ) );
}
