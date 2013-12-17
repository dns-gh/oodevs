// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipments_Speeds_GUI.h"
#include "moc_ADN_Equipments_Speeds_GUI.cpp"
#include "ADN_Equipments_Data.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Equipments_Data::SpeedInfos SpeedInfos;
typedef ADN_Equipments_Data::EquipmentInfos ComposanteInfos;

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_Speeds_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Equipments_Speeds_GUI::ADN_Equipments_Speeds_GUI( QLineEdit* maxSpeed, const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
    , maxSpeed_( maxSpeed )
    , popupIsDisplayed_( false )
{
    dataModel_.setColumnCount( 3 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Ground type" )
                      << tr( "Speed (km/h)" )
                      << tr( "Construction (%)" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddDoubleSpinBoxOnColumn( 1, 0, INT_MAX );
    delegate_.AddSpinBoxOnColumn( 2, 0, 100 );
    setSortingEnabled( true );
    connect( maxSpeed, SIGNAL( editingFinished() ), SLOT( OnMaxSpeedFinishedEditing() ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_Speeds_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Equipments_Speeds_GUI::~ADN_Equipments_Speeds_GUI()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Speeds_GUI::UpdateSpeedsValidator
// Created: ABR 2012-04-17
// -----------------------------------------------------------------------------
bool ADN_Equipments_Speeds_GUI::UpdateSpeedsValidator( double maxSpeed )
{
    bool needReBound = false;
    for( int row = 0; row < numRows(); ++row )
    {
        delegate_.SetSpinBoxMinMax< double >( row, 1, 0, maxSpeed );
        bool ok = false;
        double value = dataModel_.item( row, 1 )->text().toDouble( &ok );
        if( !needReBound && ok && value > maxSpeed )
            needReBound = true;
    }
    return needReBound;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Speeds_GUI::OnItemSelected
// Created: ABR 2012-04-17
// -----------------------------------------------------------------------------
void ADN_Equipments_Speeds_GUI::OnItemSelected( void* pData )
{
    if( pData == 0 )
        return;
    ComposanteInfos* infos = static_cast< ComposanteInfos* >( pData );
    oldMaxSpeed_ = infos->rMaxSpeed_.GetData();
    UpdateSpeedsValidator( oldMaxSpeed_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Speeds_GUI::OnMaxSpeedFinishedEditing
// Created: ABR 2012-04-16
// -----------------------------------------------------------------------------
void ADN_Equipments_Speeds_GUI::OnMaxSpeedFinishedEditing()
{
    if( popupIsDisplayed_ )
        return;
    bool ok = false;
    double maxSpeed = locale().toDouble( maxSpeed_->text(), &ok );
    if( ok && UpdateSpeedsValidator( maxSpeed ) )
    {
        popupIsDisplayed_ = true;
        int nResult = QMessageBox::warning( this,
            tr( "Warning" ),
            tr( "At least one speed is above the new max speed.\nClick \"Ok\" to bound it to the new max speed value." ),
            QMessageBox::Ok     | QMessageBox::Default,
            QMessageBox::Cancel | QMessageBox::Escape );
        popupIsDisplayed_ = false;

        if( nResult == QMessageBox::Ok )
        {
            for( int row = 0; row < numRows(); ++row )
            {
                ADN_StandardItem* item = static_cast< ADN_StandardItem* >( dataModel_.item( row, 1 ) );
                SpeedInfos* infos = static_cast< SpeedInfos* >( item->GetData() );
                if( infos->rSpeed_.GetData() > maxSpeed )
                    infos->rSpeed_ = maxSpeed;
            }
        }
        else
        {
            maxSpeed_->setText( locale().toString( oldMaxSpeed_ ) );
            maxSpeed = oldMaxSpeed_;
        }
    }
    oldMaxSpeed_ = maxSpeed;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Speeds_GUI::AddRow
// Created: NPT 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Equipments_Speeds_GUI::AddRow( int row, void* data )
{
    SpeedInfos* infos = static_cast< SpeedInfos* >( data );
    if( !infos )
        return;
    QString typeTerrain = ENT_Tr::ConvertFromLocation( infos->nTypeTerrain_, ENT_Tr::eToTr ).c_str();
    AddItem( row, 0, data, typeTerrain );
    AddItem( row, 1, data, &infos->rSpeed_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
    AddItem( row, 2, data, &infos->nConstruction_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
}

