// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "CreateExerciceWidget.h"
#include "moc_CreateExerciceWidget.cpp"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "tools/GeneralConfig.h"
#pragma warning( disable: 4127 4244 4245 )
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget constructor
// Created: FDS 2010-11-02
// -----------------------------------------------------------------------------
CreateExerciceWidget::CreateExerciceWidget(QWidget* parent, const tools::GeneralConfig& config  )
: QDialog( parent, "ExerciseCreate" )
, config_( config )
{
    setModal( true );

    setCaption( tr( "Exercise Creation" ) );
    QGridLayout* grid = new QGridLayout( this, 4, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 0, 1 );
    grid->setRowStretch( 1, 10 );
    grid->setRowStretch( 2, 5 );
    grid->setRowStretch( 3, 1 );
    grid->setRowStretch( 4, 1 );
    {
        QGroupBox* box = new QHGroupBox( tr( "Name" ), this );
        new QLabel( tr( "Name:" ), box );
        editName_ = new QLineEdit( "", box );
        grid->addMultiCellWidget( box, 0, 0, 0, 2 );
    }
    {
        QGroupBox* box = new QHGroupBox( tr( "Terrain" ), this );
        new QLabel( tr( "Name:" ), box );
        editTerrainList_ = new QComboBox( box );
        grid->addMultiCellWidget( box, 1, 1, 0, 2 );
        editTerrainList_->clear();
        editTerrainList_->insertStringList( frontend::commands::ListTerrains( config_ ) );
    }
    {
        QGroupBox* box = new QHGroupBox( tr( "Model" ), this );
        new QLabel( tr( "Name:" ), box );
        editModelList_ = new QComboBox( box );        
        grid->addMultiCellWidget( box, 2, 2, 0, 2 );

        editModelList_->clear();
        QStringList decisionalModels = frontend::commands::ListModels( config_ );
        for( QStringList::const_iterator it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
        {
            const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).ascii() );
            for( QStringList::const_iterator itP = physicalModels.begin(); itP != physicalModels.end(); ++itP )
                editModelList_->insertItem( QString( "%1/%2" ).arg( *it ).arg( *itP ) );
        }
    }
    {
        QHBox* box = new QHBox( this );
        QButton* ok = new QPushButton( tr( "Ok" ), box );
        QButton* cancel = new QPushButton( tr( "Cancel" ), box );
        grid->addWidget( box, 4, 2 );
        connect( ok, SIGNAL( clicked() ), this, SLOT( OnAccept() ) );
        connect( cancel, SIGNAL( clicked() ), this, SLOT( OnCancel() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget destructor
// Created: FDS 2010-11-02
// -----------------------------------------------------------------------------
CreateExerciceWidget::~CreateExerciceWidget()
{
    // NOTHING        
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::OnAccept
// Created: FDS 2010-11-02
// -----------------------------------------------------------------------------
void CreateExerciceWidget::OnAccept()
{
    if( editName_->text().isEmpty() ){
        QMessageBox::critical(this, "Erreur", tr("Exercise name is empty !"));
        return;
    }

    if ( bfs::exists( config_.GetExerciseFile(editName_->text().ascii()).c_str() ) ){
        QMessageBox::critical(this, "Erreur", tr("Exercise already exist !"));
        return;
    }

    const std::string terrain = editTerrainList_->currentText().ascii();
    const QStringList model = QStringList::split( "/", editModelList_->currentText() );
    frontend::CreateExercise( config_, editName_->text().ascii(), terrain, model.front().ascii(), model.back().ascii() );
    close();    
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::OnCancel
// Created: FDS 2010-11-02
// -----------------------------------------------------------------------------
void CreateExerciceWidget::OnCancel()
{
    editName_->setText("");
    close();    
}


// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::getFileName
// Created: FDS 2010-11-03
// -----------------------------------------------------------------------------
QString CreateExerciceWidget::getFileName()
{
    if (editName_->text().isEmpty())
        return editName_->text().ascii();
    else
        return config_.GetExerciseFile(editName_->text().ascii()).c_str();
}
