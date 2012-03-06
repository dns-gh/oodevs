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
    , limitValue_( new QLabel( this ) )
    , limitLine_ ( new QLabel( this ) )
    , progressLimit_( 40 )
{
    setCaption( tr( "Performance dialog" ) );
    setFixedSize( 350, 200 );

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
    layout->addWidget( progressValue_ , 0, 1, 6, 1 );
    loadLevel_->setMinimumWidth( 280 );
    loadLevel_->setMinimumHeight( 40 );
    loadLevel_->setAlignment( Qt::AlignBottom );
    progressValue_->setOrientation( Qt::Vertical );
    limitLine_->setText( "<b>____<\b>" );
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
    terrainLoad_->setText( tr( "Terrain size: " ) + QString::number( values.terrainLoad_ ) + QString( " Mo" ) ) ;
    knowledges_ ->setText( tr( "Number of knowledge groups: " ) + QString::number( values.knowledges_ ) );
    loadLevel_  ->setText( "<b>" + tr( "Load level: " )         + QString::number( static_cast< unsigned int >( loadLevel ) ) + " / " + QString::number( values.limit_ ) + "<\b>" );
    limitValue_ ->setText( "<b>" + QString::number( values.limit_ ) + "<\b>" );
    limitLine_->move( progressValue_->pos().x() - 3,
                      progressValue_->pos().y() + progressValue_->size().height() * ( 100 - progressLimit_ ) / 100 - limitLine_->size().height() / 2 - 6 );
    limitValue_->move( progressValue_->pos().x() + progressValue_->size().width() + 6,
                       progressValue_->pos().y() + progressValue_->size().height() * ( 100 - progressLimit_ ) / 100 - limitValue_->size().height() / 2 );

    assert( values.limit_ > 0 );
    float factor = loadLevel / static_cast< float >( values.limit_ ); 
    bool aboveLimit = factor > 1.f;
    float limitFactor = aboveLimit ? 1.f - 1.f / factor  : 0.f;
    float fProgressLimit = static_cast< float >( progressLimit_ );
    float progressValue = aboveLimit ? fProgressLimit + ( 100.f - fProgressLimit ) * limitFactor : fProgressLimit * factor;

    QColor barColor;
    barColor.setRed( aboveLimit ? 155 + static_cast< int >( 100.f * limitFactor ) : 0 );
    barColor.setGreen( !aboveLimit ? 200 : 0 );
    QPalette pal = progressValue_->palette();
    pal.setColor( QColorGroup::Highlight, barColor );
    progressValue_->setPalette( pal );
    progressValue_->setValue( static_cast< int >( progressValue ) );
}