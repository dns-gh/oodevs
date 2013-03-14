// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ExerciseCreationDialog.h"
#include "moc_ExerciseCreationDialog.cpp"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "tools/GeneralConfig.h"
#include <xeumeuleu/xml.hpp>

namespace fc = frontend::commands;

// -----------------------------------------------------------------------------
// Name: ExerciseCreationDialog constructor
// Created: FDS 2010-11-02
// -----------------------------------------------------------------------------
ExerciseCreationDialog::ExerciseCreationDialog( QWidget* parent, const tools::GeneralConfig& config )
    : QDialog( parent, "ExerciseCreate" )
    , config_( config )
{
//    setModal( true );
    setMinimumWidth(150);
    setCaption( tr( "Exercise Creation" ) );
    Q3GridLayout* grid = new Q3GridLayout( this, 4, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 0, 1 );
    grid->setRowStretch( 1, 10 );
    grid->setRowStretch( 2, 5 );
    grid->setRowStretch( 3, 1 );
    grid->setRowStretch( 4, 1 );
    {
        Q3GroupBox* box = new Q3HGroupBox( tr( "Name" ), this );
        //new QLabel( tr( "Name:" ), box );
        exerciseName_ = new QLineEdit( "", box );
        connect( exerciseName_, SIGNAL( textChanged( const QString& ) ), SLOT( OnFileChanged() ) );
        grid->addMultiCellWidget( box, 0, 0, 0, 2 );
    }
    {
        Q3GroupBox* box = new Q3HGroupBox( tr( "Terrain" ), this );
        //new QLabel( tr( "Name:" ), box );
        editTerrainList_ = new QComboBox( box );
        grid->addMultiCellWidget( box, 1, 1, 0, 2 );
        editTerrainList_->clear();
        editTerrainList_->addItems( fc::PathListToQStringList( fc::ListTerrains( config_ ) ) );
    }
    {
        Q3GroupBox* box = new Q3HGroupBox( tr( "Model" ), this );
        //new QLabel( tr( "Name:" ), box );
        editModelList_ = new QComboBox( box );
        grid->addMultiCellWidget( box, 2, 2, 0, 2 );

        editModelList_->clear();
        const tools::Path::T_Paths decisionalModels = fc::ListModels( config_ );
        for( auto it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
        {
            const tools::Path::T_Paths physicalModels = fc::ListPhysicalModels( config_, *it );
            for( auto itP = physicalModels.begin(); itP != physicalModels.end(); ++itP )
                editModelList_->addItem( QString( "%1/%2" ).arg( it->ToUTF8().c_str() ).arg( itP->ToUTF8().c_str() ) );
        }
    }
    {
        Q3HBox* box = new Q3HBox( this );
        ok_ = new QPushButton( tr( "Ok" ), box );
        ok_->setEnabled( false );
        QPushButton* cancel = new QPushButton( tr( "Cancel" ), box );
        grid->addWidget( box, 4, 2 );
        connect( ok_, SIGNAL( clicked() ), this, SLOT( OnAccept() ) );
        connect( cancel, SIGNAL( clicked() ), this, SLOT( OnCancel() ) );
    }
    adjustSize();
}

// -----------------------------------------------------------------------------
// Name: ExerciseCreationDialog destructor
// Created: FDS 2010-11-02
// -----------------------------------------------------------------------------
ExerciseCreationDialog::~ExerciseCreationDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseCreationDialog::OnAccept
// Created: FDS 2010-11-02
// -----------------------------------------------------------------------------
void ExerciseCreationDialog::OnAccept()
{
    try
    {
        const QStringList model = QStringList::split( "/", editModelList_->currentText() );
        frontend::CreateExercise( config_,
                                  tools::Path::FromUnicode( exerciseName_->text().toStdWString() ),
                                  tools::Path::FromUnicode( editTerrainList_->currentText().toStdWString() ),
                                  tools::Path::FromUnicode( model.front().toStdWString() ),
                                  tools::Path::FromUnicode( model.back().toStdWString() ) );
        accept();
    }
    catch( const std::exception& e )
    {
        QMessageBox::critical( this, tr( "Error" ), tools::GetExceptionMsg( e ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseCreationDialog::OnCancel
// Created: FDS 2010-11-02
// -----------------------------------------------------------------------------
void ExerciseCreationDialog::OnCancel()
{
    exerciseName_->setText( "" );
    reject();
}

// -----------------------------------------------------------------------------
// Name: ExerciseCreationDialog::GetFileName
// Created: FDS 2010-11-03
// -----------------------------------------------------------------------------
tools::Path ExerciseCreationDialog::GetFileName() const
{
    return config_.GetExerciseFile( tools::Path::FromUnicode( exerciseName_->text().toStdWString() ) );
}

// -----------------------------------------------------------------------------
// Name: ExerciseCreationDialog::OnFileChanged
// Created: FDS 2010-11-03
// -----------------------------------------------------------------------------
void ExerciseCreationDialog::OnFileChanged()
{
    ok_->setDisabled( exerciseName_->text().isEmpty() ||  config_.GetExerciseFile( tools::Path::FromUnicode( exerciseName_->text().toStdWString() ) ).Exists() || editTerrainList_->count() == 0 || editModelList_->count() == 0 );
}
