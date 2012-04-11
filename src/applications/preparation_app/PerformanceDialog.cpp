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
    , model_              ( model )
    , staticModel_        ( staticModel )
    , progressValueSingle_( new QProgressBar( this ) )
    , progressValueMulti_ ( new QProgressBar( this ) )
    , units_              ( new QLabel( this ) )
    , urbanBlocs_         ( new QLabel( this ) )
    , objects_            ( new QLabel( this ) )
    , populations_        ( new QLabel( this ) )
    , crowds_             ( new QLabel( this ) )
    , terrainLoad_        ( new QLabel( this ) )
    , knowledges_         ( new QLabel( this ) )
    , loadLevelSingle_    ( new QLabel( this ) )
    , loadLevelMulti_     ( new QLabel( this ) )
    , limitValueSingle_   ( new QLabel( this ) )
    , limitValueMulti_    ( new QLabel( this ) )
    , limitLineSingle_    ( new QLabel( this ) )
    , limitLineMulti_     ( new QLabel( this ) )
    , terrainSize_        ( new QLabel( this ) )
    , profiles_           ( new QLabel( this ) )
    , maxAutomatsKG_      ( new QLabel( this ) )
    , maxUnitsKG_         ( new QLabel( this ) )
    , avgAutomatKG_       ( new QLabel( this ) )
    , avgUnitsKG_         ( new QLabel( this ) )
    , textEdit_           ( new QTextEdit( this ) )
    , progressLimit_      ( 40 )
{
    setCaption( tr( "Performance dialog" ) );
    setFixedSize( 600, 700 );

    QGridLayout* layout = new QGridLayout( this, 17, 3 );
    layout->setSpacing( 10 );
    layout->setMargin( 15 );
    layout->setColumnMinimumWidth( 0, 500 );
    layout->setAlignment( Qt::AlignHCenter );
    layout->addWidget( new QLabel( tr( "Single station" ), this ), 0, 1 );
    layout->addWidget( new QLabel( tr( "Multi station" ), this ), 0, 2 );
    layout->addWidget( progressValueSingle_ , 1, 1, 14, 1, Qt::AlignHCenter );
    layout->addWidget( progressValueMulti_ , 1, 2, 14, 2, Qt::AlignHCenter );
    layout->addWidget( profiles_, 0, 0 );
    layout->addWidget( units_, 1, 0 );
    layout->addWidget( populations_, 2, 0 );
    layout->addWidget( crowds_, 3, 0 );
    layout->addWidget( urbanBlocs_, 4, 0 );
    layout->addWidget( objects_, 5, 0 );
    layout->addWidget( knowledges_, 6, 0 );
    layout->addWidget( terrainLoad_, 7, 0 );
    layout->addWidget( terrainSize_, 8, 0 );
    layout->addWidget( maxAutomatsKG_, 9, 0 );
    layout->addWidget( maxUnitsKG_, 10, 0 );
    layout->addWidget( avgAutomatKG_, 11, 0 );
    layout->addWidget( avgUnitsKG_, 12, 0 );
    layout->addWidget( loadLevelSingle_ , 13, 0 );
    layout->addWidget( loadLevelMulti_ , 14, 0 );
    layout->addWidget( new QLabel( "<b>" + tr( "Detail: " ) + "<\b>" ), 15, 0, 1, 2 );
    layout->addWidget( textEdit_, 16, 0, 1, 3 );

    textEdit_->setReadOnly( true );
    textEdit_->setLineWrapMode( QTextEdit::NoWrap );
    loadLevelSingle_->setMinimumWidth( 280 );
    loadLevelSingle_->setMinimumHeight( 25 );
    loadLevelSingle_->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    loadLevelMulti_->setMinimumWidth( 280 );
    loadLevelMulti_->setMinimumHeight( 25 );
    loadLevelMulti_->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    progressValueSingle_->setOrientation( Qt::Vertical );
    progressValueMulti_->setOrientation( Qt::Vertical );
    limitLineSingle_->setText( "<b>____<\b>" );
    limitLineMulti_->setText( "<b>____<\b>" );

    QDesktopWidget* pDesktop = QApplication::desktop();
    if( pDesktop )
    {
        QWidget* pScreen = pDesktop->screen( pDesktop->primaryScreen() );
        if( pScreen )
        {
            QRect scr = pScreen->geometry();
            move( ( scr.x() + scr.width() - size().width() ) / 2, ( scr.y() + scr.height() - size().height() ) / 2 - 50 );
        }
    }
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
    if( parent() && parent()->parent() )
    {
        QPoint parentPos = static_cast< QWidget* >( parent()->parent() )->pos();
        QSize parentSize = static_cast< QWidget* >( parent()->parent() )->size();
        move( ( parentPos.x() + parentSize.width() - size().width() ) / 2, ( parentPos.y() + parentSize.height() - size().height() ) / 2 );
    }

    UpdateDisplay();
}
 
namespace
{
    void UpdateBar( const PerformanceIndicator::Values& values, QProgressBar* progressBar, int limit )
    {
        QColor barColor;
        assert( values.limit_ > 0 );
        float factor = values.performance_ / static_cast< float >( values.limit_ ); 
        bool aboveLimit = factor > 1.f;
        float limitFactor = aboveLimit ? 1.f - 1.f / factor  : 0.f;
        float fProgressLimit = static_cast< float >( limit );
        float progressValue = aboveLimit ? fProgressLimit + ( 100.f - fProgressLimit ) * limitFactor : fProgressLimit * factor; 
        barColor.setRed( aboveLimit ? 155 + static_cast< int >( 100.f * limitFactor ) : 0 );
        barColor.setGreen( !aboveLimit ? 200 : 0 );
        QPalette pal = progressBar->palette();
        pal.setColor( QColorGroup::Highlight, barColor );
        progressBar->setPalette( pal );
        progressBar->setValue( static_cast< int >( progressValue ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PerformanceDialog UpdateDisplay
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
void PerformanceDialog::UpdateDisplay()
{
    const PerformanceIndicator::Values& values =  model_.performanceIndicator_.ComputeValues();
    profiles_       ->setText( tr( "Number of profiles: " )                 + locale().toString( values.profiles_ ) );
    units_          ->setText( tr( "Number of units: " )                    + locale().toString( values.units_ ) );
    populations_    ->setText( tr( "Number of populations: " )              + locale().toString( values.populations_ ) );
    crowds_         ->setText( tr( "Number of crowds: " )                   + locale().toString( values.crowds_ ) );
    urbanBlocs_     ->setText( tr( "Number of urban blocs: " )              + locale().toString( values.blocs_ ) );
    objects_        ->setText( tr( "Number of objects: " )                  + locale().toString( values.objects_ ) );
    knowledges_     ->setText( tr( "Number of knowledge groups: " )         + locale().toString( values.knowledgeGroups_ ) );
    terrainLoad_    ->setText( tr( "Terrain memory size: " )                + locale().toString( values.terrainLoad_, 'f', 3 ) + " Mo" ) ;
    terrainSize_    ->setText( tr( "Terrain size: " )                       + locale().toString( values.terrainWidth_ ) + " km x "
                                                                            + locale().toString( values.terrainHeight_ ) + " km" );
    maxAutomatsKG_  ->setText( tr( "Max automats in a knowledge group: " )  + locale().toString( values.maxAutomatsKG_ ) );
    maxUnitsKG_     ->setText( tr( "Max units in a knowledge group:" )      + locale().toString( values.maxUnitsKG_ ) );
    avgAutomatKG_   ->setText( tr( "Average automats by knowledge group: " )+ locale().toString( values.avgAutomatsKG_ ) );
    avgUnitsKG_     ->setText( tr( "Average units by knowledge group: " )   + locale().toString( values.avgUnitsKG_ ) );
    loadLevelSingle_->setText( "<b>" + tr( "Single station load level: " )  + locale().toString( static_cast< unsigned int >( values.performance_ ) ) + " / " + locale().toString( values.limit_ ) + "<\b>" );
    loadLevelMulti_ ->setText( "<b>" + tr( "Multi station load level: " )   + locale().toString( static_cast< unsigned int >( values.performance_ ) ) + " / " + locale().toString( values.limit_ ) + "<\b>" );
    limitValueSingle_->setText( "<b>" + locale().toString( values.limit_ ) + "<\b>" );
    limitValueMulti_->setText( "<b>" + locale().toString( values.limit_ ) + "<\b>" );
    limitLineSingle_->move( progressValueSingle_->pos().x() - 3,
                      progressValueSingle_->pos().y() + progressValueSingle_->size().height() * ( 100 - progressLimit_ ) / 100 - limitLineSingle_->size().height() / 2 - 6 );
    limitValueSingle_->move( progressValueSingle_->pos().x() + progressValueSingle_->size().width() + 6,
                       progressValueSingle_->pos().y() + progressValueSingle_->size().height() * ( 100 - progressLimit_ ) / 100 - limitValueSingle_->size().height() / 2 );

    limitLineMulti_->move( progressValueMulti_->pos().x() - 3,
                     progressValueMulti_->pos().y() + progressValueMulti_->size().height() * ( 100 - progressLimit_ ) / 100 - limitLineMulti_->size().height() / 2 - 6 );
    limitValueMulti_->move( progressValueMulti_->pos().x() + progressValueMulti_->size().width() + 6,
                      progressValueMulti_->pos().y() + progressValueMulti_->size().height() * ( 100 - progressLimit_ ) / 100 - limitValueMulti_->size().height() / 2 );

    UpdateBar( values, progressValueSingle_, progressLimit_ );
    UpdateBar( values, progressValueMulti_, progressLimit_ );

    QString detail;
    detail +=        tr( "Exercise: " )         + QString::fromStdString( values.exercise_ ) + "\n";
    detail += "\n" + tr( "Teams: " )            + locale().toString( values.teams_ );
    detail += "\n" + tr( "Profiles: ")          + locale().toString( values.profiles_ );
    detail += "\n" + tr( "Automats: ")          + locale().toString( values.automats_ );
    detail += "\n" + tr( "Units: ")             + locale().toString( values.units_ );
    detail += "\n" + tr( "Crowds: ")            + locale().toString( values.crowds_ );
    detail += "\n" + tr( "Populations: ")       + locale().toString( values.populations_ );
    detail += "\n" + tr( "Urban blocs: ")       + locale().toString( values.blocs_ );
    detail += "\n" + tr( "Objects: ")           + locale().toString( values.objects_ );
    detail += "\n" + tr( "Knowledge groups: ")  + locale().toString( values.knowledgeGroups_ );
    detail += "\n" + tr( "Terrain: ")           + locale().toString( values.terrainWidth_ ) + " km x " + locale().toString( values.terrainHeight_ ) + " km";
    detail += "\n" + tr( "Terrain memory size: " ) + " " + locale().toString( values.terrainLoad_, 'f', 6 ) + " Mo";
    detail += "\n" + tr( "Max automats in a knowledge group: " )     + locale().toString( values.maxAutomatsKG_ );
    detail += "\n" + tr( "Max units in a knowledge group:" )         + locale().toString( values.maxUnitsKG_ );
    detail += "\n" + tr( "Average automats by knowledge group: " )   + locale().toString( values.avgAutomatsKG_ );
    detail += "\n" + tr( "Average units by knowledge group: " )      + locale().toString( values.avgUnitsKG_ );
    detail += "\n" + tr( "Single station load level: " )             + locale().toString( values.performance_, 'f', 2 ) + " / " + locale().toString( values.limit_ );
    detail += "\n" + tr( "Multi station load level: " )              + locale().toString( values.performance_, 'f', 2 ) + " / " + locale().toString( values.limit_ );
    for( PerformanceIndicator::CIT_TeamsDatas it = values.teamsDatas_.begin(); it != values.teamsDatas_.end(); ++it )
    {
        const PerformanceIndicator::TeamData& teamData = it->second;
        detail += "\n\n" + tr( "Team: " ) + QString::fromStdString( teamData.name_ );
        detail += "\n\t" + tr( "Formations: " ) + locale().toString( teamData.formations_ );
        detail += "\n\t" + tr( "Automats: " ) + locale().toString( teamData.automats_ );
        for( PerformanceIndicator::CIT_EntityTypeCount subIt = teamData.automatTypes_.begin(); subIt != teamData.automatTypes_.end(); ++subIt )
            detail += "\n\t\t" + QString::fromStdString( subIt->first ) + QString( ": " ) + locale().toString( subIt->second );
        detail += "\n\n\t" + tr( "Units: " ) + locale().toString( teamData.units_ );
        for( PerformanceIndicator::CIT_EntityTypeCount subIt = teamData.unitTypes_.begin(); subIt != teamData.unitTypes_.end(); ++subIt )
            detail += "\n\t\t" + QString::fromStdString( subIt->first ) + QString( ": " ) + locale().toString( subIt->second );
        detail += "\n\n\t" + tr( "Crowds: " ) + locale().toString( teamData.crowds_ );
        detail += "\n\t" + tr( "Populations: " ) + locale().toString( teamData.populations_ );
        detail += "\n\t" + tr( "Objects: " ) + locale().toString( teamData.objects_ );
        for( PerformanceIndicator::CIT_EntityTypeCount subIt = teamData.objectTypes_.begin(); subIt != teamData.objectTypes_.end(); ++subIt )
            detail += "\n\t\t" + QString::fromStdString( subIt->first ) + QString( ": " ) + locale().toString( subIt->second );
        detail += "\n\n\t" + tr( "Knowledge groups: " ) + locale().toString( teamData.knowledgeGroups_ );
        for( PerformanceIndicator::CIT_KGDatas subIt = teamData.datasKG_.begin(); subIt != teamData.datasKG_.end(); ++subIt )
            detail += "\n\t\t" + QString::fromStdString( subIt->first ) + QString( ": \t" ) + tr( "Automats: " ) + locale().toString( subIt->second.automats_ ) + QString( "\t" )
            + tr( "Units: " ) + locale().toString( subIt->second.units_ );
    }
    detail += "\n";
    textEdit_->setText( detail );
}
