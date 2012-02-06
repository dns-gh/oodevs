// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PerformanceDialog.h"
#include "moc_PerformanceDialog.cpp"
#include "preparation/Model.h"
#include "preparation/StaticModel.h"
#include "preparation/performanceIndicator.h"
#include <QtGui/QGridLayout.h>
#include <QtGui/QPalette.h>
#include <QtGui/QProgressBar.h>
#include <QtGui/QBrush.h>

// -----------------------------------------------------------------------------
// Name: PerformanceDialog constructor
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
PerformanceDialog::PerformanceDialog( QWidget* parent, Model& model, const StaticModel& staticModel )
    : QDialog ( parent, "PerformanceDialog" )
    , model_( model )
    , staticModel_( staticModel )
    , progressValue_( new QProgressBar( this ) )
    , units_( new QLabel( this ) )
    , urbanBlocs_( new QLabel( this ) )
    , objects_( new QLabel( this ) )
    , terrainLoad_( new QLabel( this ) )
    , knowledges_( new QLabel( this ) )
    , loadLevel_( new QLabel( this ) )
{
    setCaption( tr( "Performance dialog" ) );
    setFixedSize( 450, 150 );

    QGridLayout* layout = new QGridLayout( this, 6, 2 );
    layout->setMargin( 15 );
    layout->setColumnMinimumWidth( 0, 230 );
    layout->setAlignment( Qt::AlignHCenter );
    layout->addWidget( units_, 0, 0 );
    layout->addWidget( urbanBlocs_, 1, 0 );
    layout->addWidget( objects_, 2, 0 );
    layout->addWidget( terrainLoad_, 3, 0 );
    layout->addWidget( knowledges_, 4, 0 );
    layout->addWidget( loadLevel_, 5, 0 );
    layout->addWidget( progressValue_ , 5, 1 );
    loadLevel_->setMinimumHeight( 50 );
}

// -----------------------------------------------------------------------------
// Name: PerformanceDialog destructor
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
PerformanceDialog::~PerformanceDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerformanceDialog showEvent
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
void PerformanceDialog::showEvent( QShowEvent* /* pEvent */ )
{
    UpdateDisplay();
}

// -----------------------------------------------------------------------------
// Name: PerformanceDialog UpdateDisplay
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
void PerformanceDialog::UpdateDisplay()
{
    PerformanceIndicator::IndicatorValues values;
    float loadLevel = model_.performanceIndicator_.ComputeLoadIndicator( values );

    units_      ->setText( tr( "Number of units: " )            + QString::number( values.units_ ) );
    urbanBlocs_ ->setText( tr( "Number of urban blocs: " )      + QString::number( values.blocs_ ) );
    objects_    ->setText( tr( "Number of objects: " )          + QString::number( values.objects_ ) );
    terrainLoad_->setText( tr( "Terrain size: " )               + QString::number( values.terrainLoad_ ) + QString( " km²" ) ) ;
    knowledges_ ->setText( tr( "Number of knowledge groups: " ) + QString::number( values.knowledges_ ) );
    loadLevel_  ->setText( "<b>" + tr( "Load level: " )         + QString::number( static_cast< unsigned int >( loadLevel ) ) + " / " + QString::number( values.limit_ ) + "<\b>" );

    assert( values.limit_ > 0 );
    float factor = loadLevel/ static_cast< float >( values.limit_ );
    if ( factor > 1.f )
        factor = 1.f;

    QColor barColor;
    barColor.setRed( static_cast< int >( factor * 255.f ) );
    barColor.setGreen( static_cast< int >( ( 1.f- factor ) * 255.f ) );
    barColor.setBlue( 0 );

    QPalette pal = progressValue_->palette();
    pal.setColor( QColorGroup::Highlight, barColor );    
    progressValue_->setPalette( pal );
    progressValue_->setValue( static_cast< unsigned int >( factor * 100.f ) );

    bool indicatorLoaded = model_.performanceIndicator_.IsLoaded();
    loadLevel_->setVisible( indicatorLoaded );
    progressValue_->setVisible( indicatorLoaded );
}