// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ExerciseProperties.h"
#include "moc_ExerciseProperties.cpp"
#include "MessageDialog.h"
#include "clients_gui/Tools.h"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "frontend/Exercise_ABC.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: ExerciseProperties constructor
// Created: SBO 2010-11-12
// -----------------------------------------------------------------------------
ExerciseProperties::ExerciseProperties( QWidget* parent, QWidget* granParent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader, bool briefing, bool models, bool editable )
    : gui::LanguageChangeObserver_ABC< Q3VBox >( parent )
    , parent_         ( granParent )
    , config_         ( config )
    , fileLoader_     ( fileLoader )
    , language_       ( tools::readLang().c_str() )
    , parametersLabel_( 0 )
    , briefingText_   ( 0 )
    , terrainList_    ( 0 )
    , dataChanged_    ( false )
{
    setMinimumWidth( 200 );
    setSpacing( 5 );
    if( briefing )
    {
        Q3VBox* box = new Q3VBox( this );
        box->setMinimumWidth( 200 );
        box->setSpacing( 5 );
        briefingImage_ = new QLabel( box );
        briefingText_ = new Q3TextEdit( box );
        briefingText_->setFont( QFont( "Georgia", 10, QFont::Normal, true ) );
        briefingText_->setReadOnly( true );
        briefingText_->hide();
    }
    if( models )
    {
        Q3GroupBox* box = new Q3GroupBox( 1, Qt::Vertical, this );
        box->setEnabled( editable );
        box->setMaximumHeight( 100 );
        Q3VBox* editBox = new Q3VBox( box );
        editBox->setMinimumWidth( 200 );
        editBox->setSpacing( 5 );
        parametersLabel_ = new QLabel( editBox );
        {
            terrainList_ = new QComboBox( editBox );
            connect( terrainList_, SIGNAL( activated( int ) ), SLOT( ModelChanged() ) );
        }
        {
            modelList_ = new QComboBox( editBox );
            connect( modelList_, SIGNAL( activated( int ) ), SLOT( ModelChanged() ) );
        }
    }
    setShown( briefing || models );
}

// -----------------------------------------------------------------------------
// Name: ExerciseProperties destructor
// Created: SBO 2010-11-12
// -----------------------------------------------------------------------------
ExerciseProperties::~ExerciseProperties()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseProperties::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void ExerciseProperties::OnLanguageChanged()
{
    if( parametersLabel_ )
        parametersLabel_->setText( tools::translate( "ExerciseProperties", "Exercise parameters:" ) );
}

// -----------------------------------------------------------------------------
// Name: ExerciseProperties::Update
// Created: RBA 2011-04-05
// -----------------------------------------------------------------------------
void ExerciseProperties::Update()
{
    if( terrainList_ )
    {
        terrainList_->clear();
        terrainList_->insertItem( tools::translate( "ExerciseProperties", "Terrain:" ) );
        terrainList_->insertStringList( frontend::commands::ListTerrains( config_ ) );

        modelList_->clear();
        modelList_->insertItem( tools::translate( "ExerciseProperties", "Model:" ) );
        const QStringList decisionalModels = frontend::commands::ListModels( config_ );
        int index = 1;
        for( QStringList::const_iterator it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
        {
            const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).toUtf8().constData() );
            for( QStringList::const_iterator itP = physicalModels.begin(); itP != physicalModels.end(); ++itP, ++index )
                modelList_->insertItem( QString( "%1/%2" ).arg( *it ).arg( *itP ), index );
        }
        if( modelList_->count() == 2 )
            modelList_->setCurrentItem( 1 );
        modelList_->setShown( modelList_->count() > 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseProperties::Select
// Created: SBO 2010-11-12
// -----------------------------------------------------------------------------
void ExerciseProperties::Select( const frontend::Exercise_ABC* exercise )
{
    Update();
    if( briefingText_ )
    {
        briefingText_->setText( tools::translate( "ExerciseProperties", "No briefing available" ) );
        briefingText_->hide();
    }
    if( !exercise )
        return;
    try
    {
        std::auto_ptr< xml::xistream > xis = fileLoader_.LoadFile( config_.GetExerciseFile( exercise->GetName() ) );
        currentTerrain_.clear();
        currentData_.clear();
        currentPhysical_.clear();
        std::string image;
        *xis >> xml::start( "exercise" )
                >> xml::start( "terrain" )
                    >> xml::attribute( "name", currentTerrain_ )
                >> xml::end
                >> xml::start( "model" )
                    >> xml::attribute( "dataset", currentData_ )
                    >> xml::attribute( "physical", currentPhysical_ )
                >> xml::end;
        if( briefingText_ )
        {
            *xis >> xml::optional >> xml::start( "meta" )
                    >> xml::optional >> xml::start( "briefing" )
                        >> xml::optional  >> xml::content( "image", image )
                            >> xml::list( "text", *this, &ExerciseProperties::ReadBriefingText );
            if( !image.empty() )
            {
                const std::string imagePath = config_.GetExerciseDir( QString( "%1/%2" ).arg( exercise->GetName().c_str() ).arg( image.c_str() ).toUtf8().constData() );
                const QImage pix( imagePath.c_str() );
                QPixmap px;
                px.fromImage( pix );
                briefingImage_->setPixmap( px );
            }
        }
        if( terrainList_ )
        {
            const QStringList terrainList = frontend::commands::ListTerrains( config_ );
            int index = terrainList.findIndex( currentTerrain_.c_str() );
            terrainList_->setCurrentItem( index + 1 );
            int modelIndex = modelList_->findText( QString( "%1/%2" ).arg( currentData_.c_str() ).arg( currentPhysical_.c_str() ) );
            if( modelIndex != -1 )
                modelList_->setCurrentItem( modelIndex );
        }
    }
    catch( ... )
    {
        // $$$$ SBO 2008-10-07: error in exercise.xml meta, just don't show briefing
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseProperties::ReadBriefingText
// Created: RDS 2008-08-28
// -----------------------------------------------------------------------------
void ExerciseProperties::ReadBriefingText( xml::xistream& xis )
{
    std::string lang, text;
    xis >> xml::attribute( "lang", lang )
        >> text;
    if( lang == language_.toUtf8().constData() )
    {
        briefingText_->setText( text.c_str() );
        briefingText_->show();
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseProperties::ModelChanged
// Created: SBO 2010-11-15
// -----------------------------------------------------------------------------
void ExerciseProperties::ModelChanged()
{
    dataChanged_ = true;
    emit ExercisePropertiesChanged();
}

// -----------------------------------------------------------------------------
// Name: ExerciseProperties::Commit
// Created: SBO 2010-11-15
// -----------------------------------------------------------------------------
bool ExerciseProperties::Commit( const frontend::Exercise_ABC& exercise )
{
    // Be sure to commit if mismatched terrain or data, even if no changes has occured 
    if( terrainList_ && terrainList_->currentItem() > 0 && terrainList_->currentText().ascii() != currentTerrain_ )
    {
        MessageDialog message( parent_, tools::translate( "ExerciseProperties", "Warning" ), tools::translate( "ExerciseProperties", "The selected terrain is not the one referenced by the selected exercise.\nDo really you want to replace it ?" ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() == QMessageBox::Yes )
            dataChanged_ = true;
        else
            return false;
    }
    if( modelList_ && modelList_->currentItem() > 0 )
    {
        const QStringList model = QStringList::split( "/", modelList_->currentText() );
        if( model.front().ascii() != currentData_ || model.back().ascii() != currentPhysical_ )
        {
            MessageDialog message( parent_, tools::translate( "ExerciseProperties", "Warning" ), tools::translate( "ExerciseProperties", "The selected model is not the one referenced by the selected exercise.\nDo really you want to replace it ?" ), QMessageBox::Yes, QMessageBox::No );
            if( message.exec() == QMessageBox::Yes )
                dataChanged_ = true;
            else
                return false;
        }
    }

    if( dataChanged_ )
        if( terrainList_ && terrainList_->currentItem() > 0 && modelList_ && modelList_->currentItem() > 0 )
        {
            const std::string terrain = terrainList_->currentText().toUtf8().constData();
            const QStringList model = QStringList::split( "/", modelList_->currentText() );
            frontend::EditExerciseParameters( config_, exercise.GetName(), terrain, model.front().toUtf8().constData(), model.back().toUtf8().constData() );
        }
    dataChanged_ = false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ExerciseProperties::IsValid
// Created: LGY 2012-06-07
// -----------------------------------------------------------------------------
bool ExerciseProperties::IsValid() const
{
    return terrainList_->currentText().toUtf8().constData() != tools::translate( "ExerciseProperties", "Terrain:" ) &&
           modelList_->currentText().toUtf8().constData() != tools::translate( "ExerciseProperties", "Model:" );
}
