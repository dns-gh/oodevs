// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ScoreGaugeConfiguration.h"
#include "moc_ScoreGaugeConfiguration.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Displayer_ABC.h"
#include "indicators/Gauge.h"
#include "indicators/GaugeNormalizer.h"
#include "indicators/GaugeType.h"
#include "indicators/GaugeTypes.h"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <qimage.h>

// -----------------------------------------------------------------------------
// Name: ScoreGaugeConfiguration constructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
ScoreGaugeConfiguration::ScoreGaugeConfiguration( QWidget* parent, kernel::Controllers& controllers, const indicators::GaugeTypes& gaugeTypes )
    : QVGroupBox( tr( "Gauge configuration" ), parent )
    , controllers_( controllers )
    , gaugeTypes_( gaugeTypes )
{
    {
        QHBox* box = new QHBox( this );
        new QLabel( tr( "Representation type: " ), box );
        type_ = new gui::ValuedComboBox< const indicators::GaugeType* >( box );
        connect( type_, SIGNAL( activated( int ) ), SLOT( OnTypeChanged() ) );
    }
    {
        QGroupBox* box = new QGroupBox( 1, Qt::Horizontal, tr( "Value normalization" ), this );
        {
            QHBox* hbox = new QHBox( box );
            {
                QLabel* label = new QLabel( tr( "Steps: " ), hbox );
                steps_ = new QSpinBox( 1, 100, 1, hbox );
                label->setBuddy( steps_ );
                connect( steps_, SIGNAL( valueChanged( int ) ), SLOT( OnChangeStep( int ) ) );
            }
            {
                QLabel* label = new QLabel( tr( "Min: " ), hbox );
                min_ = new QLineEdit( hbox );
                min_->setValidator( new QDoubleValidator( min_ ) );
                label->setBuddy( min_ );
                min_->setText( "0" );
                connect( min_, SIGNAL( textChanged( const QString& ) ), SLOT( OnChangeBoundaries() ) );
            }
            {
                QLabel* label = new QLabel( tr( "Max: " ), hbox );
                max_ = new QLineEdit( hbox );
                max_->setValidator( new QDoubleValidator( max_ ) );
                label->setBuddy( max_ );
                max_->setText( "100" );
                connect( max_, SIGNAL( textChanged( const QString& ) ), SLOT( OnChangeBoundaries() ) );
            }
        }
        {
            intervals_ = new QTable( 0, 3, box );
            intervals_->horizontalHeader()->setLabel( 0, tr( "Min" ) );
            intervals_->horizontalHeader()->setLabel( 1, tr( "Max" ) );
            intervals_->horizontalHeader()->setLabel( 2, tr( "Symbol" ) );
            intervals_->setFixedHeight( 100 );
            intervals_->setLeftMargin( 0 );
            intervals_->setHScrollBarMode( QScrollView::AlwaysOff );
            AddInterval();
            connect( intervals_, SIGNAL( valueChanged( int, int ) ), SLOT( OnChangeValue( int, int ) ) );
        }
    }
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ScoreGaugeConfiguration destructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
ScoreGaugeConfiguration::~ScoreGaugeConfiguration()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ScoreGaugeConfiguration::StartEdit
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
void ScoreGaugeConfiguration::StartEdit( const indicators::Gauge& gauge )
{
    type_->SetCurrentItem( &gauge.GetType() );
    intervals_->setNumRows( 0 );
    double min = std::numeric_limits< double >::max();
    double max = std::numeric_limits< double >::min();
    const indicators::GaugeNormalizer::T_Intervals& intervals = gauge.GetNormalizer().Intervals();
    BOOST_FOREACH( const indicators::GaugeNormalizer::T_Intervals::value_type& interval, intervals )
    {
        min = std::min( min, interval.first.first );
        max = std::max( max, interval.first.second );
        AddInterval( interval.first.first, interval.first.second, interval.second );
    }
    {
        min_->blockSignals( true );
        min_->setText( QString::number( min, 'f', 2 ) );
        min_->blockSignals( false );
    }
    {
        max_->blockSignals( true );
        max_->setText( QString::number( max, 'f', 2 ) );
        max_->blockSignals( false );
    }
    {
        steps_->blockSignals( true );
        steps_->setValue( intervals_->numRows() );
        steps_->blockSignals( false );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreGaugeConfiguration::GetValue
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
indicators::Gauge ScoreGaugeConfiguration::GetValue() const
{
    indicators::Gauge result( *type_->GetValue() );
    indicators::GaugeNormalizer normalizer;
    for( int i = 0; i < intervals_->numRows(); ++i )
        normalizer.AddInterval( intervals_->text( i, 0 ).toDouble(), intervals_->text( i, 1 ).toDouble(), intervals_->text( i, 2 ).toDouble() );
    result.SetNormalizer( normalizer );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ScoreGaugeConfiguration::OnChangeStep
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
void ScoreGaugeConfiguration::OnChangeStep( int steps )
{
    const int rows = intervals_->numRows();
    for( int i = rows; i < steps; ++i )
        AddInterval();
    for( int i = steps; i < rows; ++i )
        RemoveInterval();
    OnChangeBoundaries();
}

// -----------------------------------------------------------------------------
// Name: ScoreGaugeConfiguration::OnChangeBoundaries
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
void ScoreGaugeConfiguration::OnChangeBoundaries()
{
    const double min = min_->text().toDouble();
    const double max = max_->text().toDouble();
    const int rows = intervals_->numRows();
    const double interval = ( max - min ) / rows;
    for( int i = 0; i < rows; ++i )
    {
        intervals_->setText( i, 0, QString::number( min + i * interval, 'f', 2 ) );
        intervals_->setText( i, 1, QString::number( min + ( i + 1 ) * interval, 'f', 2 ) );
        UpdateSymbol( i, double( i ) / ( rows > 1 ? rows - 1 : rows ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreGaugeConfiguration::OnTypeChanged
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
void ScoreGaugeConfiguration::OnTypeChanged()
{
    for( int row = 0; row < intervals_->numRows(); ++row )
        UpdateSymbol( row, intervals_->text( row, 2 ).toDouble() );
}

// -----------------------------------------------------------------------------
// Name: ScoreGaugeConfiguration::OnChangeValue
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
void ScoreGaugeConfiguration::OnChangeValue( int row, int col )
{
    if( col == 2 )
        UpdateSymbol( row, intervals_->text( row, 2 ).toDouble() );
}

// -----------------------------------------------------------------------------
// Name: ScoreGaugeConfiguration::AddInterval
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
void ScoreGaugeConfiguration::AddInterval( double min /*= 0*/, double max /*= 0*/, double key /*= 0*/ )
{
    const int rows = intervals_->numRows();
    intervals_->setNumRows( rows + 1 );
    intervals_->setText( rows, 0, QString::number( min, 'f', 2 ) );
    intervals_->setText( rows, 1, QString::number( max, 'f', 2 ) );
    intervals_->setText( rows, 2, QString::number( key, 'f', 2 ) );
    UpdateSymbol( rows, key );
}

// -----------------------------------------------------------------------------
// Name: ScoreGaugeConfiguration::RemoveInterval
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
void ScoreGaugeConfiguration::RemoveInterval()
{
    intervals_->setNumRows( intervals_->numRows() - 1 );
}

namespace
{
    class GaugeItemDisplayer : public kernel::Displayer_ABC
                             , public tools::Caller< QPixmap >
    {
    public:
        explicit GaugeItemDisplayer( QTableItem* item ) : item_( item ) {}

        virtual void Hide() {}
        virtual void Clear() {}
        virtual kernel::Displayer_ABC& SubItem( const QString& ) { return *this; }
        virtual void StartDisplay()
        {
            text_ = "";
            pixmap_ = QPixmap();
        }
        virtual void DisplayFormatted( const QString& formatted )
        {
            text_ = formatted;
        }
        virtual void Call( const QPixmap& pixmap )
        {
            QImage image;
            image = pixmap;
            image = image.smoothScale( 16, 16 );
            pixmap_ = image;
        }
        virtual void EndDisplay()
        {
            item_->setText( text_ );
            item_->setPixmap( pixmap_ );
        }
    private:
        QTableItem* item_;
        QPixmap pixmap_;
        QString text_;
    };
}

// -----------------------------------------------------------------------------
// Name: ScoreGaugeConfiguration::UpdateSymbol
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
void ScoreGaugeConfiguration::UpdateSymbol( int row, double value )
{
    if( type_->Count() )
        if( const indicators::GaugeType* type = type_->GetValue() )
        {
            GaugeItemDisplayer displayer( intervals_->item( row, 2 ) );
            type->Display( displayer, value );
            intervals_->item( row, 2 )->setText( QString::number( value, 'f', 2 ) );
            intervals_->updateCell( row, 2 );
        }
}

// -----------------------------------------------------------------------------
// Name: ScoreGaugeConfiguration::NotifyUpdated
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
void ScoreGaugeConfiguration::NotifyUpdated( const kernel::ModelLoaded& /*model*/ )
{
    type_->Clear();
    kernel::Iterator< const indicators::GaugeType& > it( gaugeTypes_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const indicators::GaugeType& type = it.NextElement();
        type_->AddItem( type.GetName(), &type );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreGaugeConfiguration::NotifyUpdated
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
void ScoreGaugeConfiguration::NotifyUpdated( const kernel::ModelUnLoaded& /*model*/ )
{
    type_->Clear();
}
