// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ExerciseList.h"
#include "ExerciseLister_ABC.h"
#include "moc_ExerciseList.cpp"
#include "MenuButton.h"
#include "Profile.h"
#include "ProfileList.h"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "tools/GeneralConfig.h"
#include "clients_gui/Tools.h"

#include <qfileinfo.h>
#include <qheader.h>
#include <qlistview.h>
#include <qsettings.h>
#include <qtextcodec.h>

#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#pragma warning( push )
#pragma warning( disable: 4702 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>
#include <qcombobox.h>

namespace bfs = boost::filesystem;


namespace
{
    QString ReadLang()
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() );
    }

    bool IsDirectory( const QListViewItem* item )
    {
        return item->text( 1 ).startsWith( "/" );
    }

    class MyListViewItem : public QListViewItem
    {
    public:
        explicit MyListViewItem( QListView* parent ) : QListViewItem( parent ) {}
        explicit MyListViewItem( QListViewItem* parent ) : QListViewItem( parent ) {}

        virtual QString key( int column, bool /*ascending*/ ) const
        {
            if( column == 1 )
                return text( IsDirectory( this ) ? 1 : 0 );
            return text( column );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ExerciseList constructor
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
ExerciseList::ExerciseList( QWidget* parent, const tools::GeneralConfig& config, const ExerciseLister_ABC& lister, const std::string& subDir /*= ""*/, bool showBrief /*= true*/, bool showProfile /*=true*/, bool showParams /*= true*/, bool enableParams /*= true*/ )
    : QVBox      ( parent )
    , config_    ( config )
    , subDir_    ( subDir )
    , showBrief_ ( showBrief )
    , lister_    ( lister )
    , language_  ( ReadLang() )
    , parametersChanged_( false )
    , editTerrainList_( 0 )
    , editModelList_( 0 )
{
    QHBox* box = new QHBox( this );
    box->setMargin( 5 );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setSpacing( 50 );
    {
        QVBox* leftBox = new QVBox( box );
        leftBox->setSpacing( 5 );
        leftBox->setBackgroundOrigin( QWidget::WindowOrigin );
        QLabel* label = new QLabel( tools::translate( "ExerciseList", "Exercise:" ), leftBox );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        exercises_ = new QListView( leftBox );
        exercises_->setBackgroundOrigin( QWidget::WindowOrigin );
        exercises_->addColumn( "exercise" );
        exercises_->addColumn( "fullpath", 0 );
        exercises_->header()->hide();
        exercises_->setAllColumnsShowFocus( true );
        exercises_->setSortColumn( 1 );

        label = new QLabel( tools::translate( "ExerciseList", "Profile:" ), leftBox );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        profiles_ = new ProfileList( leftBox, config );
        leftBox->setStretchFactor( exercises_, 3 );
        leftBox->setStretchFactor( profiles_, 1 );
        label->setShown( showProfile );
        profiles_->setShown( showProfile );

        connect( profiles_ , SIGNAL( Select( const Profile& ) ), this, SLOT( SelectProfile( const Profile& ) ) );
        connect( exercises_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( SelectExercise( QListViewItem* ) ) );
    }

    if( showBrief || showParams )
    {
        QVBox* parametersRightBox = new QVBox( box );
        parametersRightBox->setMinimumWidth( 200 );
        parametersRightBox->setBackgroundOrigin( QWidget::WindowOrigin );
        parametersRightBox->setSpacing( 5 );

        if( showBrief )
        {
            QVBox* rightBox = new QVBox( parametersRightBox );
            rightBox->setMinimumWidth( 200 );
            rightBox->setBackgroundOrigin( QWidget::WindowOrigin );
            rightBox->setSpacing( 5 );
            briefingImage_ = new QLabel( rightBox );
            briefingImage_->setBackgroundOrigin( QWidget::WindowOrigin );
            briefingText_ = new QTextEdit( rightBox );
            briefingText_->setBackgroundOrigin( QWidget::WindowOrigin );
            briefingText_->setFont( QFont( "Georgia", 10, QFont::Normal, true ) );
            briefingText_->setReadOnly( true );
        }
        if( showParams )
        {
            QGroupBox* paramBox = new QGroupBox( 1, Qt::Vertical, parametersRightBox );
            paramBox->setEnabled( enableParams );
            paramBox->setMaximumHeight( 100 );
            paramBox->setBackgroundOrigin( QWidget::WindowOrigin );
            QVBox* editBox = new QVBox( paramBox );
            editBox->setMinimumWidth( 200 );
            editBox->setBackgroundOrigin( QWidget::WindowOrigin );
            editBox->setSpacing( 5 );
            QLabel* label = new QLabel( tools::translate( "ScenarioEditPage", "Exercise parameters:" ), editBox );
            label->setBackgroundOrigin( QWidget::WindowOrigin );
            editTerrainList_ = new QComboBox( editBox );
            editTerrainList_->setBackgroundOrigin( QWidget::WindowOrigin );
            connect( editTerrainList_, SIGNAL( activated( int ) ), SLOT( ComboChanged( int ) ) );
            editModelList_ = new QComboBox( editBox );
            editModelList_->setBackgroundOrigin( QWidget::WindowOrigin );
            connect( editModelList_, SIGNAL( activated( int ) ), SLOT( ComboChanged( int ) ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseList destructor
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
ExerciseList::~ExerciseList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::Update
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseList::Update()
{
    if( editTerrainList_ && editModelList_ )
    {
        editTerrainList_->clear();
        editTerrainList_->insertItem( tools::translate( "ScenarioEditPage", "Terrain:" ) );
        editTerrainList_->insertStringList( frontend::commands::ListTerrains( config_ ) );
        editModelList_->clear();
        editModelList_->insertItem( tools::translate( "ScenarioEditPage", "Model:" ) );
        QStringList decisionalModels = frontend::commands::ListModels( config_ );
        int index = 1;
        for( QStringList::const_iterator it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
        {
            const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).ascii() );
            for( QStringList::const_iterator itP = physicalModels.begin(); itP != physicalModels.end(); ++itP, ++index )
                editModelList_->insertItem( QString( "%1/%2" ).arg( *it ).arg( *itP ), index );
        }
        if( editModelList_->count() == 2 )
            editModelList_->setCurrentItem( 1 );
        editModelList_->setShown( editModelList_->count() > 2 );
    }
    QApplication::postEvent( this, new QCustomEvent( 4242 ) );
}

namespace
{
    QString MakePath( const std::string& dir, const std::string& file )
    {
        if( dir.empty() )
            return file.c_str();
        return QString( "%1/%2" ).arg( dir.c_str() ).arg( file.c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::SelectExercise
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseList::SelectExercise( QListViewItem* item )
{
    const QString exercise( item ? item->text( 1 ) : "" );
    profiles_->Update( MakePath( subDir_, exercise.ascii() ) );
    if( showBrief_ )
    {
        briefingText_->setText( tools::translate( "ExerciseList", "No briefing available" ) );
        briefingText_->hide();
        try
        {
            xml::xifstream xis( config_.GetExerciseFile( MakePath( subDir_, exercise.ascii() ).ascii() ) );
            std::string image, terrain, data, physical;
            xis >> xml::start( "exercise" )
                    >> xml::start( "terrain" )
                        >> xml::attribute( "name", terrain )
                    >> xml::end
                    >> xml::start( "model" )
                        >> xml::attribute( "dataset", data )
                        >> xml::attribute( "physical", physical )
                    >> xml::end
                    >> xml::optional >> xml::start( "meta" )
                        >> xml::optional >> xml::start( "briefing" )
                            >> xml::optional  >> xml::content( "image", image )
                                >> xml::list( "text", *this, &ExerciseList::ReadBriefingText );

            const std::string imagePath = config_.GetExerciseDir( MakePath( exercise.ascii(), image ).ascii() );
            const QImage pix( imagePath.c_str() );
            briefingImage_->setPixmap( pix );
            if( editTerrainList_ && editModelList_ )
            {
                const QStringList terrainList = frontend::commands::ListTerrains( config_ );
                int index = terrainList.findIndex( terrain.c_str() );
                editTerrainList_->setCurrentItem( index + 1 );

                QStringList decisionalModels = frontend::commands::ListModels( config_ );
                for( QStringList::const_iterator it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
                {
                    const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).ascii() );
                    int index = physicalModels.findIndex( QString( physical.c_str() ) );
                    if( index != -1 )
                        editModelList_->setCurrentItem( index + 1 );
                }
            }
        }
        catch( ... )
        {
            // $$$$ SBO 2008-10-07: error in exercise.xml meta, just don't show briefing
        }
    }
    emit Select( BuildExercisePath(), Profile() );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::GetExerciseDisplayName
// Created: RDS 2008-10-02
// -----------------------------------------------------------------------------
QString ExerciseList::GetExerciseDisplayName( const QString& exercise ) const
{
    std::string displayName( exercise.ascii() );
    try
    {
        xml::xifstream xis( config_.GetExerciseFile( MakePath( subDir_, exercise.ascii() ).ascii() ) );
        xis >> xml::start( "exercise" )
                >> xml::optional >> xml::start( "meta" )
                    >> xml::optional >> xml::content( "name", displayName );
    }
    catch( ... )
    {
        // $$$$ SBO 2008-10-07: error in exercise.xml meta, just show directory name
    }
    return displayName.c_str();
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::BuildExercisePath
// Created: SBO 2008-10-31
// -----------------------------------------------------------------------------
QString ExerciseList::BuildExercisePath() const
{
    QListViewItem* item = exercises_->currentItem();
    if( item && !IsDirectory( item ) )
        return MakePath( subDir_, item->text( 1 ).ascii() );
    return "";
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::ReadBriefingText
// Created: RDS 2008-08-28
// -----------------------------------------------------------------------------
void ExerciseList::ReadBriefingText( xml::xistream& xis )
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
// Name: ExerciseList::SelectProfile
// Created: SBO 2008-10-31
// -----------------------------------------------------------------------------
void ExerciseList::SelectProfile( const Profile& profile )
{
    if( exercises_->currentItem() )
        emit Select( BuildExercisePath(), profile );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::Clear
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
void ExerciseList::Clear()
{
    QApplication::postEvent( this, new QCustomEvent( 4243 ) );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::customEvent
// Created: SBO 2008-11-05
// -----------------------------------------------------------------------------
void ExerciseList::customEvent( QCustomEvent* e )
{
    if( e->type() == 4242 )
    {
        QStringList exercises;
        lister_.ListExercises( exercises );
        exercises_->clear();
        for( QStringList::iterator it = exercises.begin(); it != exercises.end(); ++it )
            AddExerciseEntry( *it );
        exercises_->sort();
        if( exercises_->childCount() )
            exercises_->setSelected( exercises_->firstChild(), true );
    }
    else if( e->type() == 4243 )
    {
        profiles_->clear();
        exercises_->clear();
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::AddExerciseEntry
// Created: SBO 2010-04-14
// -----------------------------------------------------------------------------
void ExerciseList::AddExerciseEntry( const QString& exercise )
{
    static const QImage directory( "resources/images/selftraining/directory.png" );
    static const QImage mission( "resources/images/selftraining/mission.png" );

    QStringList path( QStringList::split( '/', exercise ) );
    QStringList complete;
    QListViewItem* parent = 0;
    for( QStringList::iterator it( path.begin() ); it != path.end(); ++it )
    {
        complete.append( *it );
        const QString current( complete.join( "/" ) );
        QListViewItem* item = exercises_->findItem( "/" + current, 1 );
        if( !item )
        {
            if( parent )
                item = new MyListViewItem( parent );
            else
                item = new MyListViewItem( exercises_ );
            item->setText( 0, GetExerciseDisplayName( *it ) );
            item->setText( 1, complete.size() < path.size() ? "/" + current : current );
            item->setPixmap( 0, directory );
        }
        parent = item;
    }
    if( parent )
        parent->setPixmap( 0, mission );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::Exists
// Created: SBO 2010-04-15
// -----------------------------------------------------------------------------
bool ExerciseList::Exists( const QString& exercise ) const
{
    return exercises_->findItem( exercise, 0, Qt::ExactMatch ) != 0;
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::ComboChanged
// Created: SLG 2010-07-08
// -----------------------------------------------------------------------------
void ExerciseList::ComboChanged( int )
{
    parametersChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::ChangeExerciceParameters
// Created: SLG 2010-07-08
// -----------------------------------------------------------------------------
void ExerciseList::ChangeExerciceParameters( const std::string& exerciceName )
{
    if( parametersChanged_ )
    {
        if( editTerrainList_ && editTerrainList_->currentItem() > 0 && editModelList_ && editModelList_->currentItem() > 0 )
        {
            const std::string terrain = editTerrainList_->currentText().ascii();
            const QStringList model = QStringList::split( "/", editModelList_->currentText() );
            frontend::EditExerciseParameters( config_, exerciceName, terrain, model.front().ascii(), model.back().ascii() );
        }
    }
    parametersChanged_ = false;
}
