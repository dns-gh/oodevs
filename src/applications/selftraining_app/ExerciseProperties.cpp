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
#include "clients_gui/Tools.h"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "frontend/Exercise_ABC.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    QString ReadLang()
    {
        QSettings settings( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() );
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseProperties constructor
// Created: SBO 2010-11-12
// -----------------------------------------------------------------------------
ExerciseProperties::ExerciseProperties( QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader, bool briefing, bool models, bool editable )
    : gui::LanguageChangeObserver_ABC< Q3VBox >( parent )
    , config_         ( config )
    , fileLoader_     ( fileLoader )
    , language_       ( ReadLang() )
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
            const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).ascii() );
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
        std::string image, terrain, data, physical;
        *xis >> xml::start( "exercise" )
                >> xml::start( "terrain" )
                    >> xml::attribute( "name", terrain )
                >> xml::end
                >> xml::start( "model" )
                    >> xml::attribute( "dataset", data )
                    >> xml::attribute( "physical", physical )
                >> xml::end;
        if( briefingText_ )
        {
            *xis >> xml::optional >> xml::start( "meta" )
                    >> xml::optional >> xml::start( "briefing" )
                        >> xml::optional  >> xml::content( "image", image )
                            >> xml::list( "text", *this, &ExerciseProperties::ReadBriefingText );
            if( !image.empty() )
            {
                const std::string imagePath = config_.GetExerciseDir( QString( "%1/%2" ).arg( exercise->GetName().c_str() ).arg( image.c_str() ).ascii() );
                const QImage pix( imagePath.c_str() );
                QPixmap px;
                px.fromImage( pix );
                briefingImage_->setPixmap( px );
            }
        }
        if( terrainList_ )
        {
            const QStringList terrainList = frontend::commands::ListTerrains( config_ );
            int index = terrainList.findIndex( terrain.c_str() );
            terrainList_->setCurrentItem( index + 1 );
            int modelIndex = modelList_->findText( QString( "%1/%2" ).arg( data.c_str() ).arg( physical.c_str() ) );
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
    if( lang == language_.ascii() )
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
void ExerciseProperties::Commit( const frontend::Exercise_ABC& exercise )
{
    if( dataChanged_ )
        if( terrainList_ && terrainList_->currentItem() > 0 && modelList_ && modelList_->currentItem() > 0 )
        {
            const std::string terrain = terrainList_->currentText().ascii();
            const QStringList model = QStringList::split( "/", modelList_->currentText() );
            frontend::EditExerciseParameters( config_, exercise.GetName(), terrain, model.front().ascii(), model.back().ascii() );
        }
    dataChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: ExerciseProperties::IsValid
// Created: LGY 2012-06-07
// -----------------------------------------------------------------------------
bool ExerciseProperties::IsValid() const
{
    return terrainList_->currentText().ascii() != tools::translate( "ExerciseProperties", "Terrain:" ) &&
           modelList_->currentText().ascii() != tools::translate( "ExerciseProperties", "Model:" );
}
