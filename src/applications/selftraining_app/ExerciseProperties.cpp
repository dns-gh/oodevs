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
#include "clients_gui/ImageWrapper.h"
#include "clients_kernel/Tools.h"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "frontend/Exercise_ABC.h"
#include "tools/GeneralConfig.h"
#include "tools/Language.h"
#include "tools/Loader_ABC.h"
#include <xeumeuleu/xml.hpp>

namespace fc = frontend::commands;

namespace
{
    class AutoResizedCombo : public QComboBox
    {
        void showPopup()
        {
            auto w = view();
            w->setMinimumWidth( w->sizeHintForColumn( 0 ) + 16 );
            QComboBox::showPopup();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ExerciseProperties constructor
// Created: SBO 2010-11-12
// -----------------------------------------------------------------------------
ExerciseProperties::ExerciseProperties( QWidget* parent, QWidget* granParent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader, bool briefing, bool models, bool editable )
    : gui::WidgetLanguageObserver_ABC< QWidget >( parent )
    , parent_         ( granParent )
    , config_         ( config )
    , fileLoader_     ( fileLoader )
    , parametersLabel_( 0 )
    , briefingText_   ( 0 )
    , terrainList_    ( 0 )
    , dataChanged_    ( false )
{
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->setSpacing( 5 );
    setMinimumWidth( 200 );
    if( briefing )
    {
        briefingImage_ = new QLabel();
        briefingText_ = new QTextEdit();
        briefingText_->setFont( QFont( "Georgia", 10, QFont::Normal, true ) );
        briefingText_->setReadOnly( true );
        briefingText_->hide();

        QWidget* box = new QWidget( this );
        QVBoxLayout* boxLayout = new QVBoxLayout( box );
        boxLayout->addWidget( briefingImage_ );
        boxLayout->addWidget( briefingText_ );
        boxLayout->setSpacing( 5 );
        box->setMinimumWidth( 200 );
        layout->addWidget( box );
    }
    if( models )
    {
        parametersLabel_ = new QLabel();

        terrainList_ = new AutoResizedCombo();
        connect( terrainList_, SIGNAL( activated( int ) ), SLOT( ModelChanged() ) );

        modelList_ = new AutoResizedCombo();
        connect( modelList_, SIGNAL( activated( int ) ), SLOT( ModelChanged() ) );

        QGroupBox* box = new QGroupBox();
        QVBoxLayout* boxLayout = new QVBoxLayout( box );
        boxLayout->setSpacing( 5 );
        boxLayout->addWidget( parametersLabel_ );
        boxLayout->addWidget( terrainList_);
        boxLayout->addWidget( modelList_ );
        box->setEnabled( editable );
        box->setMaximumHeight( 100 );
        box->setMinimumWidth( 200 );
        layout->addWidget( box );
    }
    setShown( briefing || models );
    parent->layout()->addWidget( this );
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
        terrainList_->addItem( tools::translate( "ExerciseProperties", "Terrain:" ) );
        terrainList_->addItems( fc::PathListToQStringList( fc::ListTerrains( config_ ) ) );

        modelList_->clear();
        modelList_->addItem( tools::translate( "ExerciseProperties", "Model:" ) );
        const tools::Path::T_Paths decisionalModels = fc::ListModels( config_ );
        int index = 1;
        for( auto it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
        {
            const tools::Path::T_Paths physicalModels = fc::ListPhysicalModels( config_, *it );
            for( auto itP = physicalModels.begin(); itP != physicalModels.end(); ++itP, ++index )
                modelList_->addItem( QString( "%1/%2" ).arg( it->ToUTF8().c_str() ).arg( itP->ToUTF8().c_str() ), index );
        }
        if( modelList_->count() == 2 )
            modelList_->setCurrentIndex( 1 );
        modelList_->setVisible( modelList_->count() > 1 );
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
        std::unique_ptr< xml::xistream > xis = fileLoader_.LoadFile( config_.GetExerciseFile( exercise->GetName() ) );
        currentTerrain_.Clear();
        currentData_.Clear();
        currentPhysical_.Clear();
        tools::Path image;
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
            if( !image.IsEmpty() )
            {
                const tools::Path imagePath = config_.GetExerciseDir( exercise->GetName() / image );
                const gui::Image pix( imagePath );
                QPixmap px;
                px.fromImage( pix );
                briefingImage_->setPixmap( px );
            }
        }
        if( terrainList_ )
        {
            const QStringList terrainList = fc::PathListToQStringList( fc::ListTerrains( config_ ) );
            int index = terrainList.indexOf( QString::fromStdWString( currentTerrain_.ToUnicode().c_str() ) );
            terrainList_->setCurrentIndex( index + 1 );
            int modelIndex = modelList_->findText( QString( "%1/%2" ).arg( QString::fromStdWString( currentData_.ToUnicode().c_str() ) ).arg( QString::fromStdWString( currentPhysical_.ToUnicode().c_str() ) ) );
            modelList_->setCurrentIndex( modelIndex == -1 ? 0 : modelIndex );
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
    if( lang == tools::Language::Current().c_str() )
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
    if( terrainList_ && terrainList_->currentIndex() > 0 && terrainList_->currentText() != QString::fromStdWString( currentTerrain_.ToUnicode().c_str() ) )
    {
        MessageDialog message( parent_, tools::translate( "ExerciseProperties", "Warning" ), tools::translate( "ExerciseProperties", "The selected terrain is not the one referenced by the selected exercise.\nDo you want to replace it?" ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() == QMessageBox::Yes )
            dataChanged_ = true;
        else
            return false;
    }
    if( modelList_ && modelList_->currentIndex() > 0 )
    {
        const QStringList model = QString( modelList_->currentText() ).split( "/" );
        if( model.front() != QString::fromStdWString( currentData_.ToUnicode().c_str() ) || model.back() != QString::fromStdWString( currentPhysical_.ToUnicode().c_str() ) )
        {
            MessageDialog message( parent_, tools::translate( "ExerciseProperties", "Warning" ), tools::translate( "ExerciseProperties", "The selected model is not the one referenced by the selected exercise.\nDo you want to replace it?" ), QMessageBox::Yes, QMessageBox::No );
            if( message.exec() == QMessageBox::Yes )
                dataChanged_ = true;
            else
                return false;
        }
    }

    if( dataChanged_ )
        if( terrainList_ && terrainList_->currentIndex() > 0 && modelList_ && modelList_->currentIndex() > 0 )
        {
            const tools::Path terrain = tools::Path::FromUnicode( terrainList_->currentText().toStdWString() );
            const QStringList model = QString( modelList_->currentText() ).split( "/" );
            frontend::EditExerciseParameters( config_, exercise.GetName(), terrain, tools::Path::FromUnicode( model.front().toStdWString() ), tools::Path::FromUnicode( model.back().toStdWString() ) );
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
    return terrainList_->currentText() != tools::translate( "ExerciseProperties", "Terrain:" ) &&
           modelList_->currentText() != tools::translate( "ExerciseProperties", "Model:" );
}
