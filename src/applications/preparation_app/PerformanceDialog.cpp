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
#include "clients_gui/RichWidget.h"
#include "clients_gui/RichWidget.h"
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
    , loadLevelSingle_    ( new QLabel( this ) )
    , loadLevelMulti_     ( new QLabel( this ) )
    , limitValueSingle_   ( new QLabel( this ) )
    , limitValueMulti_    ( new QLabel( this ) )
    , limitLineSingle_    ( new QLabel( this ) )
    , limitLineMulti_     ( new QLabel( this ) )
    , textEdit_           ( new gui::RichWidget< QTextEdit >( "textEdit", this ) )
    , progressLimit_      ( 40 )
{
    gui::SubObjectName subObject( "PerformanceDialog" );
    setCaption( tr( "Performance dialog" ) );
    setFixedSize( 600, 700 );

    dataModel_ = new QStandardItemModel();
    dataModel_->setColumnCount( 2 );
    dataModel_->setRowCount( eNbrPerformanceData );
    dataModel_->setItem( ePerformanceData_Profiles              , 0, new QStandardItem( tr( "Number of profiles" ) ) );
    dataModel_->setItem( ePerformanceData_Units                 , 0, new QStandardItem( tr( "Number of units" ) ) );
    dataModel_->setItem( ePerformanceData_Populations           , 0, new QStandardItem( tr( "Number of populations" ) ) );
    dataModel_->setItem( ePerformanceData_Crowds                , 0, new QStandardItem( tr( "Number of crowds" ) ) );
    dataModel_->setItem( ePerformanceData_UrbanBlocs            , 0, new QStandardItem( tr( "Number of urban blocs" ) ) );
    dataModel_->setItem( ePerformanceData_Objects               , 0, new QStandardItem( tr( "Number of objects" ) ) );
    dataModel_->setItem( ePerformanceData_knowledgeGroups       , 0, new QStandardItem( tr( "Number of knowledge groups" ) ) );
    dataModel_->setItem( ePerformanceData_TerrainMemorySize     , 0, new QStandardItem( tr( "Terrain memory size" ) ) );
    dataModel_->setItem( ePerformanceData_TerrainSize           , 0, new QStandardItem( tr( "Terrain size" ) ) );
    dataModel_->setItem( ePerformanceData_MaxAutomatsKG         , 0, new QStandardItem( tr( "Max automats in a knowledge group" ) ) );
    dataModel_->setItem( ePerformanceData_MaxUnitsKG            , 0, new QStandardItem( tr( "Max units in a knowledge group" ) ) );
    dataModel_->setItem( ePerformanceData_AvgAutomatsKG         , 0, new QStandardItem( tr( "Average automats by knowledge group" ) ) );
    dataModel_->setItem( ePerformanceData_AvgUnitsKG            , 0, new QStandardItem( tr( "Average units by knowledge group" ) ) );
    dataModel_->setItem( ePerformanceData_SingleStationLoadLevel, 0, new QStandardItem( tr( "Single station load level" ) ) );
    dataModel_->setItem( ePerformanceData_MultiStationLoadLevel , 0, new QStandardItem( tr( "Multi station load level" ) ) );

    for( int i=0; i < dataModel_->rowCount(); ++i )
    {
        dataModel_->setItem( i, 1, new QStandardItem() );
        dataModel_->item( i, 0 )->setEditable( false );
        dataModel_->item( i, 1 )->setEditable( false );
        dataModel_->item( i, 1 )->setTextAlignment( Qt::AlignRight );
    }

    QFont boldFont = dataModel_->item( ePerformanceData_SingleStationLoadLevel, 0 )->font();
    boldFont.setBold( true );
    for( int i=0; i < dataModel_->columnCount(); ++i )
    {
        dataModel_->item( ePerformanceData_SingleStationLoadLevel, i )->setFont( boldFont );
        dataModel_->item( ePerformanceData_MultiStationLoadLevel, i )->setFont( boldFont );
    }

    gui::RichWidget< QTableView >* dataTable = new gui::RichWidget< QTableView >( "dataTable", this );
    dataTable->setModel( dataModel_ );
    dataTable->horizontalHeader()->setVisible( false );
    dataTable->verticalHeader()->setVisible( false );
    dataTable->setColumnWidth( 0, 308 );
    dataTable->setColumnWidth( 1, 130 );
    dataTable->verticalHeader()->setDefaultSectionSize( 21 );
    dataTable->setAlternatingRowColors( true );

    QGridLayout* layout = new QGridLayout( this, 4, 3 );
    layout->setSpacing( 10 );
    layout->setMargin( 15 );
    layout->setColumnMinimumWidth( 0, 520 );
    layout->setAlignment( Qt::AlignHCenter );
    layout->addWidget( dataTable, 0, 0, 2, 1 );
    layout->addWidget( new QLabel( tr( "Single station" ), this ), 0, 1 );
    layout->addWidget( new QLabel( tr( "Multi station" ), this ), 0, 2 );
    layout->addWidget( progressValueSingle_, 1, 1, 1, 1, Qt::AlignHCenter );
    layout->addWidget( progressValueMulti_, 1, 2, 1, 1, Qt::AlignHCenter );
    layout->addWidget( new QLabel( "<b>" + tr( "Detail: " ) + "<\b>" ), 2, 0, 1, 2 );
    layout->addWidget( textEdit_, 3, 0, 1, 2 );

    textEdit_->setReadOnly( true );
    textEdit_->setLineWrapMode( gui::RichWidget< QTextEdit >::NoWrap );
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
    const PerformanceIndicator::Values& values =  model_.performanceIndicator_->ComputeValues();
    dataModel_->item( ePerformanceData_Profiles                 , 1 )->setText( locale().toString( values.profiles_ ) );
    dataModel_->item( ePerformanceData_Units                    , 1 )->setText( locale().toString( values.units_ ) );
    dataModel_->item( ePerformanceData_Populations              , 1 )->setText( locale().toString( values.populations_ ) );
    dataModel_->item( ePerformanceData_Crowds                   , 1 )->setText( locale().toString( values.crowds_ ) );
    dataModel_->item( ePerformanceData_UrbanBlocs               , 1 )->setText( locale().toString( values.blocs_ ) );
    dataModel_->item( ePerformanceData_Objects                  , 1 )->setText( locale().toString( values.objects_ ) );
    dataModel_->item( ePerformanceData_knowledgeGroups          , 1 )->setText( locale().toString( values.knowledgeGroups_ ) );
    dataModel_->item( ePerformanceData_TerrainMemorySize        , 1 )->setText( locale().toString( values.terrainLoad_, 'f', 2 ) + " Mo" );
    dataModel_->item( ePerformanceData_TerrainSize              , 1 )->setText( locale().toString( values.terrainWidth_ ) + " km x " + locale().toString( values.terrainHeight_ ) + " km" );
    dataModel_->item( ePerformanceData_MaxAutomatsKG            , 1 )->setText( locale().toString( values.maxAutomatsKG_ ) );
    dataModel_->item( ePerformanceData_MaxUnitsKG               , 1 )->setText( locale().toString( values.maxUnitsKG_ )  );
    dataModel_->item( ePerformanceData_AvgAutomatsKG            , 1 )->setText( locale().toString( values.avgAutomatsKG_ ) );
    dataModel_->item( ePerformanceData_AvgUnitsKG               , 1 )->setText( locale().toString( values.avgUnitsKG_ ) );
    dataModel_->item( ePerformanceData_SingleStationLoadLevel   , 1 )->setText( locale().toString( static_cast< unsigned int >( values.performance_ ) ) + " / " + locale().toString( values.limit_ ) );
    dataModel_->item( ePerformanceData_MultiStationLoadLevel    , 1 )->setText( locale().toString( static_cast< unsigned int >( values.performance_ ) ) + " / " + locale().toString( values.limit_ ) );

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
    detail +=        tr( "Exercise: " )         + values.exercise_.FileName().ToUTF8().c_str() + "\n";
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
