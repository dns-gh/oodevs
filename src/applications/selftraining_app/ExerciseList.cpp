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
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

namespace
{
    static int clearEvent = 4242;


    class MyListViewItem : public gui::ValuedListItem
    {
    public:
        MyListViewItem( Q3ListView* parent, const std::string& fullpath )
            : gui::ValuedListItem( parent )
            , fullpath_( fullpath.c_str() ) {}
        MyListViewItem( Q3ListViewItem* parent, const std::string& fullpath )
            : gui::ValuedListItem( parent )
            , fullpath_( fullpath.c_str() ) {}
        virtual QString key( int column, bool /*ascending*/ ) const
        {
            if( column == 0 )
                return fullpath_.startsWith( "/" ) ? fullpath_ : text( 0 );
            return text( column );
        }
        const QString fullpath_;
    };
}

// -----------------------------------------------------------------------------
// Name: ExerciseList constructor
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
ExerciseList::ExerciseList( QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, bool showBrief /* = true*/, bool showProfile /* =true*/, bool showParams /* = true*/, bool enableParams /* = true*/ )
    : Q3VBox             ( parent )
    , config_           ( config )
    , fileLoader_       ( fileLoader )
    , controllers_      ( controllers )
    , filter_           ( 0 )
    , defaultFilter_    ( new frontend::LocalExerciseFilter() )
{
    Q3HBox* box = new Q3HBox( this );
    box->setMargin( 5 );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setSpacing( 50 );
    {
        Q3VBox* leftBox = new Q3VBox( box );
        leftBox->setSpacing( 5 );
        leftBox->setBackgroundOrigin( QWidget::WindowOrigin );
        QLabel* label = new QLabel( tools::translate( "ExerciseList", "Exercise:" ), leftBox );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        exercises_ = new Q3ListView( leftBox );
        exercises_->addColumn( "exercise" );
        exercises_->header()->hide();
        exercises_->setAllColumnsShowFocus( true );
        exercises_->setSortColumn( 0 );
        exercises_->setResizeMode( Q3ListView::LastColumn );

        label = new QLabel( tools::translate( "ExerciseList", "Profile:" ), leftBox );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        profiles_ = new ProfileList( leftBox, config, fileLoader_ );
        leftBox->setStretchFactor( exercises_, 3 );
        leftBox->setStretchFactor( profiles_, 1 );
        label->setShown( showProfile );
        profiles_->setShown( showProfile );

        connect( profiles_ , SIGNAL( Select( const frontend::Profile& ) ), this, SLOT( SelectProfile( const frontend::Profile& ) ) );
        connect( exercises_, SIGNAL( selectionChanged( Q3ListViewItem* ) ), this, SLOT( SelectExercise( Q3ListViewItem* ) ) );
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
void ExerciseList::SelectExercise( Q3ListViewItem* item )
{
    const QString exercise( item ? static_cast< MyListViewItem* >( item )->fullpath_ : "" );
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
    Q3ListViewItem* item = exercises_->currentItem();
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
    emit ClearSelection();
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
    QPixmap pxDir;
    QPixmap pxMiss;

    pxDir.fromImage( directory );
    pxMiss.fromImage( mission );

    qApp->setOverrideCursor( Qt::WaitCursor );
    QStringList path( QStringList::split( '/', exercise.GetName().c_str() ) );
    QStringList complete;
    Q3ListViewItem* parent = 0;
    for( QStringList::iterator it( path.begin() ); it != path.end(); ++it )
    {
        complete.append( *it );
        const QString current( complete.join( "/" ) );
        Q3ListViewItem* item = FindExerciseItem( "/" + current );
        if( !item )
        {
            const std::string fullpath( complete.size() < path.size() ? "/" + current : current );
            if( parent )
                item = new MyListViewItem( parent, fullpath );
            else
                item = new MyListViewItem( exercises_, fullpath );
            item->setText( 0, GetExerciseDisplayName( *it ) );
            QPixmap p;
            p.convertFromImage( directory );
            item->setPixmap( 0, p );
        }
        parent = item;
    }
    if( parent )
    {
        parent->setPixmap( 0, pxMiss );
        static_cast< gui::ValuedListItem* >( parent )->SetValue( &exercise );
    }
    qApp->restoreOverrideCursor();
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::FindExerciseItem
// Created: SBO 2011-05-27
// -----------------------------------------------------------------------------
Q3ListViewItem* ExerciseList::FindExerciseItem( const QString& path ) const
{
    for( Q3ListViewItemIterator it = exercises_->firstChild(); it.current(); ++it )
        if( static_cast< MyListViewItem* >( it.current() )->fullpath_ == path )
            return it.current();
    return 0;
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
        Q3ListViewItem* parent = item->parent();
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
    return exercises_->findItem( exercise, 0, Q3ListView::ExactMatch ) != 0;
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
