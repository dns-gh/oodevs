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
#include "moc_ExerciseList.cpp"
#include "ExerciseProperties.h"
#include "ProfileList.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/LocalExerciseFilter.h"
#include "frontend/Profile.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include "clients_gui/ValuedListItem.h"
#include "clients_gui/Tools.h"
#include <boost/filesystem.hpp>
#include <qheader.h>
#include <qlistview.h>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

namespace
{
    static int clearEvent = 4242;

    bool IsDirectory( const QListViewItem* item )
    {
        return item->text( 1 ).startsWith( "/" );
    }

    class MyListViewItem : public gui::ValuedListItem
    {
    public:
        explicit MyListViewItem( QListView* parent ) : gui::ValuedListItem( parent ) {}
        explicit MyListViewItem( QListViewItem* parent ) : gui::ValuedListItem( parent ) {}
        virtual QString key( int column, bool /*ascending*/ ) const
        {
            if( column == 1 )
                return text( IsDirectory( this ) ? 1 : 0 );
            return text( column );
        }
        virtual void paintCell( QPainter* painter, const QColorGroup& cg, int column, int width, int align )
        {
            QListViewItem::paintCell( painter, cg, column, width, align );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ExerciseList constructor
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
ExerciseList::ExerciseList( QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, bool showBrief /*= true*/, bool showProfile /*=true*/, bool showParams /*= true*/, bool enableParams /*= true*/ )
    : QVBox             ( parent )
    , config_           ( config )
    , fileLoader_       ( fileLoader )
    , controllers_      ( controllers )
    , filter_           ( 0 )
    , defaultFilter_    ( new frontend::LocalExerciseFilter() )
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
        profiles_ = new ProfileList( leftBox, config, fileLoader_ );
        leftBox->setStretchFactor( exercises_, 3 );
        leftBox->setStretchFactor( profiles_, 1 );
        label->setShown( showProfile );
        profiles_->setShown( showProfile );

        connect( profiles_ , SIGNAL( Select( const frontend::Profile& ) ), this, SLOT( SelectProfile( const frontend::Profile& ) ) );
        connect( exercises_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( SelectExercise( QListViewItem* ) ) );
    }
    {
        properties_ = new ExerciseProperties( box, config, fileLoader_, showBrief, showParams, enableParams );
    }
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList destructor
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
ExerciseList::~ExerciseList()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::NotifyCreated
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
void ExerciseList::NotifyCreated( const frontend::Exercise_ABC& exercise )
{
    if( !filter_ && defaultFilter_->Allows( exercise ) || filter_ && filter_->Allows( exercise ) )
        UpdateExerciseEntry( exercise );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::NotifyUpdated
// Created: SBO 2010-11-04
// -----------------------------------------------------------------------------
void ExerciseList::NotifyUpdated( const frontend::Exercise_ABC& exercise )
{
    UpdateExerciseEntry( exercise );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::NotifyDeleted
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
void ExerciseList::NotifyDeleted( const frontend::Exercise_ABC& exercise )
{
    DeleteExerciseEntry( exercise );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::SetFilter
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
void ExerciseList::SetFilter( const frontend::ExerciseFilter_ABC& filter )
{
    filter_ = &filter;
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::SelectExercise
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseList::SelectExercise( QListViewItem* item )
{
    const QString exercise( item ? item->text( 1 ) : "" );
    profiles_->Update( exercise.ascii() );
    if( const frontend::Exercise_ABC* selected = GetSelectedExercise() )
    {
        properties_->Select( selected );
        emit Select( *selected, frontend::Profile() );
    }
    else
    {
        properties_->Select( 0 );
        emit ClearSelection();
    }
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
        const std::string file = config_.GetExerciseFile( exercise.ascii() );
        if( boost::filesystem::exists( file ) )
        {
            std::auto_ptr< xml::xistream > xis = fileLoader_.LoadFile( file );
            *xis >> xml::start( "exercise" )
                    >> xml::optional >> xml::start( "meta" )
                        >> xml::optional >> xml::content( "name", displayName );
        }
    }
    catch( ... )
    {
        // $$$$ SBO 2008-10-07: error in exercise.xml meta, just show directory name
    }
    return displayName.c_str();
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::GetSelectedExercise
// Created: SBO 2008-10-31
// -----------------------------------------------------------------------------
const frontend::Exercise_ABC* ExerciseList::GetSelectedExercise() const
{
    QListViewItem* item = exercises_->currentItem();
    if( gui::ValuedListItem* value = static_cast< gui::ValuedListItem* >( item ) )
        if( value->IsA< const frontend::Exercise_ABC >() )
            return value->GetValue< const frontend::Exercise_ABC >();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::SelectProfile
// Created: SBO 2008-10-31
// -----------------------------------------------------------------------------
void ExerciseList::SelectProfile( const frontend::Profile& profile )
{
    if( const frontend::Exercise_ABC* exercise = GetSelectedExercise() )
        emit Select( *exercise, profile );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::Clear
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
void ExerciseList::Clear()
{
    QApplication::postEvent( this, new QCustomEvent( ::clearEvent ) );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::customEvent
// Created: SBO 2008-11-05
// -----------------------------------------------------------------------------
void ExerciseList::customEvent( QCustomEvent* e )
{
    if( e->type() == ::clearEvent )
    {
        profiles_->clear();
        exercises_->clear();
        properties_->Update();
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::AddExerciseEntry
// Created: SBO 2010-04-14
// -----------------------------------------------------------------------------
void ExerciseList::AddExerciseEntry( const frontend::Exercise_ABC& exercise )
{
    static const QImage directory( "resources/images/selftraining/directory.png" );
    static const QImage mission( "resources/images/selftraining/mission.png" );

    qApp->setOverrideCursor( QCursor::WaitCursor );
    QStringList path( QStringList::split( '/', exercise.GetName().c_str() ) );
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
    {
        parent->setPixmap( 0, mission );
        static_cast< gui::ValuedListItem* >( parent )->SetValue( &exercise );
    }
    qApp->restoreOverrideCursor();
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::UpdateExerciseEntry
// Created: SBO 2010-11-04
// -----------------------------------------------------------------------------
void ExerciseList::UpdateExerciseEntry( const frontend::Exercise_ABC& exercise )
{
    const bool isAllowed = !filter_ && defaultFilter_->Allows( exercise ) || filter_ && filter_->Allows( exercise );
    if( gui::ValuedListItem* item = gui::FindItem( &exercise, exercises_->firstChild() ) )
    {
        if( !isAllowed )
            DeleteExerciseEntry( exercise );
    }
    else if( isAllowed )
        AddExerciseEntry( exercise );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::DeleteExerciseEntry
// Created: SBO 2010-11-04
// -----------------------------------------------------------------------------
void ExerciseList::DeleteExerciseEntry( const frontend::Exercise_ABC& exercise )
{
    if( gui::ValuedListItem* item = gui::FindItem( &exercise, exercises_->firstChild() ) )
    {
        QListViewItem* parent = item->parent();
        delete item;
        if( parent && parent->childCount() == 0 )
            delete parent;
    }
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
// Name: ExerciseList::ChangeExerciceParameters
// Created: SLG 2010-07-08
// -----------------------------------------------------------------------------
void ExerciseList::ChangeExerciceParameters()
{
    if( const frontend::Exercise_ABC* exercise = GetSelectedExercise() )
        properties_->Commit( *exercise );
}
