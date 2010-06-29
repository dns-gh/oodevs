// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "CreatePackagePanel.h"
#include "moc_CreatePackagePanel.cpp"

#include "InfoBubble.h"
#include "ActionsContext.h"
#include "resources.h"
#include "frontend/commands.h"
#include "frontend/Config.h"
#include "zipstream/zipstream.h"
#include "clients_gui/Tools.h"
#include <xeumeuleu/xml.h>

#include <qaction.h>
#include <qcursor.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qprogressbar.h>
#include <qtextedit.h>
#include <qapplication.h>
#include <qlistview.h>

#pragma warning( push, 0 )
#pragma warning( disable: 4127 4244 4245 4996 )
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <zipstream/zipstream.h>
#pragma warning( pop )

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: CreatePackagePanel constructor
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
CreatePackagePanel::CreatePackagePanel( QWidgetStack* widget, QAction& action, const frontend::Config& config, ActionsContext& context )
    : Panel_ABC( widget, action, context, "CreatePackagePanel" )
    , config_( config )
{
    QVBox* box = new QVBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, action.text(), box );
    {
        new QLabel( tr( "Exercise to package:" ), group );
        list_ = new QListBox( group );
    }
    {
        new QLabel( tr( "Package description:" ), group );
        description_ = new QTextEdit( group );
        description_->setMaximumHeight( 30 );
        description_->setReadOnly( false );
    }
    {
        new QLabel( tr( "Package content:" ), group );
        content_ = new QListView( group );
        content_->addColumn( "exercise features" );
        content_->adjustSize();
    }

    progress_ = new QProgressBar( box );
    progress_->hide();

    bubble_ = new InfoBubble( box );
    QHBox* btnBox = new QHBox( box );
    btnBox->layout()->setAlignment( Qt::AlignRight );
    okay_ = new QPushButton( MAKE_PIXMAP( next ), action.text(), btnBox );
    QFont font( "Arial", 10, QFont::Bold );
    okay_->setFont( font );

    connect( okay_, SIGNAL( pressed() ), SLOT( CreatePackage() ) );
    connect( list_, SIGNAL( clicked( QListBoxItem * ) ), SLOT( OnSelectionChanged( QListBoxItem * ) ) );

    // package_->setText( config_.GetPackageFile().c_str() );
    package_.first = config_.GetRootDir();
    UpdateExercises();
}

// -----------------------------------------------------------------------------
// Name: CreatePackagePanel destructor
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
CreatePackagePanel::~CreatePackagePanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CreatePackagePanel::BrowseClicked
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
bool CreatePackagePanel::BrowseClicked()
{
    const QString filename = QFileDialog::getSaveFileName( package_.second.c_str(), "SWORD packages (*.otpak)", this, "", tr( "Select a package" ) );
    if( filename.isEmpty() )
        return false;
    const bfs::path file = bfs::path( std::string( filename.ascii() ), bfs::native );
    package_.first = file.parent_path().directory_string();
    package_.second = file.leaf().c_str();
    if( bfs::exists( file ) )
        return QMessageBox::question( this, tr( "Overwrite File?" ),
                            tr( "A file called %1 already exists. Do you want to overwrite it?" ).arg( filename ),
                            QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes;
    return true;
}

// -----------------------------------------------------------------------------
// Name: CreatePackagePanel::OnSelectionChanged
// Created: JCR 2009-11-10
// -----------------------------------------------------------------------------
void CreatePackagePanel::OnSelectionChanged( QListBoxItem* item )
{
    if( item )
    {
        package_.second = std::string( item->text().ascii() ) + ".otpak";
        Update();
    }
}


namespace
{
    struct Progress
    {
        Progress( QProgressBar* progress ) : progress_( progress ), count_( 0 ) {}
        void operator()()
        {
            progress_->setProgress( ++count_ );
            qApp->processEvents();
        }
        QProgressBar* progress_;
        unsigned count_;
    };
}

namespace
{

    void Copy( std::istream& file, std::ostream& output )
    {
        std::istreambuf_iterator< char > it( file );
        std::istreambuf_iterator< char > end;
        std::ostreambuf_iterator< char > out( output );
        std::copy( it, end, out );
    }

    bool IsBinary( const bfs::path& root )
    {
        std::string ext = bfs::extension( root );
        return ext == ".dat" || ext == ".bin" || ext == ".texture" || ext == ".dump";
    }

    void CopyFile( const bfs::path& root, const std::string& name, zip::ozipfile& zos )
    {
        std::string filename( root.file_string() );
        std::ios_base::openmode openmode = std::ifstream::in;
        if( IsBinary( root ) )
            openmode |= std::ifstream::binary;

        std::ifstream file( filename.c_str(), openmode );
        if( file.good() )
        {
            zip::ozipstream stream( zos, name.c_str(), std::ios_base::out | std::ios_base::binary );
            Copy( file, stream );
        }
    }

    void BrowseDirectory( const bfs::path& root, const std::string& name, zip::ozipfile& zos )
    {
        bfs::directory_iterator end;
        for( bfs::directory_iterator it( root ); it != end; ++it )
        {
            const bfs::path child = *it;
            if( bfs::is_regular_file( child ) )
            {
                const std::string& file( child.filename() );
                CopyFile( ( root / file.c_str() ), name + "/" + file, zos );
            }
        }
    }

    void Serialize( const std::string& base, const std::string& name, zip::ozipfile& zos )
    {
        const bfs::path root = bfs::path( base, bfs::native ) / name;
        if( ! bfs::exists( root ) )
            return;
        if( ! bfs::is_directory( root ) )
            CopyFile( root, name, zos );
        else
            BrowseDirectory( root, name, zos );
    }

    void BrowseChildren( const std::string& base, QListViewItem* item, zip::ozipfile& zos, boost::function0<void> callback )
    {
        while ( item != 0 && ! dynamic_cast< QCheckListItem* >( item ) )
        {
            std::string file( item->text( 0 ).ascii() );
            Serialize( base, file, zos );
            callback();
            item = item->nextSibling();
        }
    }

    void BrowseFiles( const std::string& base, QListViewItemIterator iterator, zip::ozipfile& zos, boost::function0<void> callback )
    {
        while ( iterator.current() )
        {
            QCheckListItem* item = dynamic_cast< QCheckListItem* >( iterator.current() );
            if( item && item->isOn() )
            {
                std::string file( iterator.current()->text( 0 ).ascii() );
                Serialize( base, file, zos );
                if( item->childCount() > 0 )
                    BrowseChildren( base, item->firstChild(), zos, callback );
            }
            callback();
            ++iterator;
        }
    }

    int ListViewSize( QListViewItemIterator iterator )
    {
        int i = 0;
        for ( ; iterator.current(); ++iterator, ++i )
            ;
        return i;
    }
}

// -----------------------------------------------------------------------------
// Name: CreatePackagePanel::WriteContent
// Created: JCR 2009-11-12
// -----------------------------------------------------------------------------
void CreatePackagePanel::WriteContent( zip::ozipfile& archive ) const
{
    xml::xostringstream xos;
    std::string name( list_->selectedItem()->text().ascii() );
    std::string description( description_->text().ascii() );
    if( description.empty() )
        description = "Packaged scenario of " + name;
    xos << xml::start( "content" )
            << xml::content( "name", name )
            << xml::content( "description", description )
        << xml::end();
    {
        std::istringstream input( xos.str() );
        zip::ozipstream output( archive, "content.xml" );
        Copy( input, output );
    }
}

namespace
{
    template< typename Base >
    class scope_trigger
    {
    public:
        scope_trigger( Base* base )
            : base_( base )
        {
            base_->setDisabled( true );
        }
        ~scope_trigger()
        {
            base_->setDisabled( false );
        }
    private:
        Base* base_;
    };
}

// -----------------------------------------------------------------------------
// Name: CreatePackagePanel::InstallPackage
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
void CreatePackagePanel::CreatePackage()
{
    scope_trigger< QPushButton > trigger( okay_ );

    if( ! BrowseClicked() )
    {
        bubble_->ShowError( tr( "Package has not been created." ) );
        return;
    }
    std::string filename( std::string( package_.first.c_str() ) + "/" + package_.second.c_str() );
    zip::ozipfile archive( filename.c_str() );
    if( archive.isOk() )
    {
        progress_->show();
        progress_->setProgress( 0, ListViewSize( QListViewItemIterator( content_ ) ) );
        setCursor( QCursor::waitCursor );
        {
            WriteContent( archive );
            BrowseFiles( config_.GetRootDir(), QListViewItemIterator( content_ ), archive, Progress( progress_ ) );
        }
        setCursor( QCursor::arrowCursor );
    }
    bubble_->ShowInfo( tr( "Package successfully created." ) );
}

// -----------------------------------------------------------------------------
// Name: CreatePackagePanel::UpdateExercises
// Created: JCR 2009-11-10
// -----------------------------------------------------------------------------
void CreatePackagePanel::UpdateExercises()
{
    list_->clear();
    list_->insertStringList( frontend::commands::ListExercises( config_ ) );
    context_.Load( "exercise", list_ );
}

// -----------------------------------------------------------------------------
// Name: CreatePackagePanel::BuildCategory
// Created: JCR 2009-11-12
// -----------------------------------------------------------------------------
void CreatePackagePanel::BuildCategory( QListViewItem* parent, const QStringList& list, const QString& base, const std::string& category )
{
    if( !list.isEmpty() )
    {
        QListViewItem* item = new QListViewItem( parent, category.c_str() );
        item->setOpen( true );
        for ( QStringList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it )
            item->insertItem( new QCheckListItem( item, base + std::string( "/" + category + "/" ).c_str() + *it, QCheckListItem::CheckBox ) );
        parent->insertItem( item );
    }
}

// -----------------------------------------------------------------------------
// Name: CreatePackagePanel::BuildExerciseFeatures
// Created: JCR 2009-11-12
// -----------------------------------------------------------------------------
QListViewItem* CreatePackagePanel::BuildExerciseFeatures( const std::string& exercise )
{
    QString base( std::string( "exercises/" + exercise ).c_str() );

    QListViewItem* rootItem = new QListViewItem( content_, "exercises" );
    rootItem->setOpen( true );

    QCheckListItem* exerciseItem = new QCheckListItem( rootItem, base, QCheckListItem::CheckBox );
    exerciseItem->setOpen( true );
    exerciseItem->setOn( true );
    exerciseItem->setEnabled( false );
    QStringList sessions( frontend::commands::ListSessions( config_, exercise ) );
    for ( QStringList::const_iterator it = sessions.constBegin(); it != sessions.constEnd(); ++it )
    {
        QCheckListItem* sessionItem = new QCheckListItem( exerciseItem, base + "/sessions/" + *it, QCheckListItem::CheckBox );
        sessionItem->setOpen( true );

        std::string category( QString( "sessions/" + *it + "/checkpoints" ).ascii() );
        const QStringList checkpoints = frontend::commands::ListCheckpoints( config_, exercise, (*it).ascii() );
        BuildCategory( sessionItem, checkpoints, base, category );
    }
    {
        const QStringList scripts = frontend::commands::ListScripts( config_, exercise );
        BuildCategory( rootItem, scripts, base, "scripts" );
    }
    {
        const QStringList orders = frontend::commands::ListOrders( config_, exercise );
        BuildCategory( rootItem, orders, base, "orders" );
    }
    return rootItem;
}

namespace
{
    QListViewItem* InsertValidatedEntry( QListViewItem* parent, const std::string& entry, const std::string& root )
    {
        QCheckListItem* item = new QCheckListItem( parent, entry.c_str(), QCheckListItem::CheckBox );
        if( ! bfs::exists( bfs::path( root + "/" + entry ) ) )
        {
            item->setEnabled( false );
            item->setText( 0, std::string( entry + " -- Not present." ).c_str() );
        }
        parent->insertItem( item );
        return item;
    }
}

// -----------------------------------------------------------------------------
// Name: CreatePackagePanel::BuildExerciseData
// Created: JCR 2009-11-12
// -----------------------------------------------------------------------------
QListViewItem* CreatePackagePanel::BuildExerciseData( const std::string& exercise )
{
    std::string terrain, population, dataset, physical;
    xml::xifstream xis( config_.GetExerciseFile( exercise ) );
    xis >> xml::start( "exercise" )
            >> xml::start( "terrain" ) >> xml::attribute( "name", terrain ) >> xml::end()
            >> xml::optional() >> xml::start( "population" ) >> xml::attribute( "name", population ) >> xml::end()
            >> xml::start( "model" ) >> xml::attribute( "dataset", dataset ) >> xml::attribute( "physical", physical ) >> xml::end()
        >> xml::end();

    QListViewItem* dataItem = new QListViewItem( content_, "data" );
    dataItem->setOpen( true );

    QListViewItem* terrainItem = InsertValidatedEntry( dataItem, "data/terrains/" + terrain, config_.GetRootDir() );
    if( terrainItem )
    {
        terrainItem->insertItem( new QListViewItem( terrainItem, std::string( "data/terrains/" + terrain + "/Detection" ).c_str() ) );
        terrainItem->insertItem( new QListViewItem( terrainItem, std::string( "data/terrains/" + terrain + "/Graphics" ).c_str() ) );
        terrainItem->insertItem( new QListViewItem( terrainItem, std::string( "data/terrains/" + terrain + "/Pathfind" ).c_str() ) );
    }

    if( ! population.empty() )
    {
        QListViewItem* populationItem = InsertValidatedEntry( dataItem, "data/population/" + population, config_.GetRootDir() );
        populationItem->insertItem( new QListViewItem( populationItem, std::string( "data/population/" + population + "/model" ).c_str() ) );
        populationItem->insertItem( new QListViewItem( populationItem, std::string( "data/population/" + population + "/navteq" ).c_str() ) );
    }
    InsertValidatedEntry( dataItem, "data/models/" + dataset + "/physical/" + physical, config_.GetRootDir() );
    return dataItem;
}


// -----------------------------------------------------------------------------
// Name: CreatePackagePanel::Update
// @brief: List exercise data : exercise, scripts, orders, sessions, checkpoints
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
void CreatePackagePanel::Update()
{
    QListBoxItem* item = list_->selectedItem();
    if( item )
    {
        std::string exercise( item->text().ascii() );
        content_->clear();
        content_->setColumnText( 0, item->text() + " exercise features" );
        content_->insertItem( BuildExerciseFeatures( exercise ) );
        content_->insertItem( BuildExerciseData( exercise ) );
    }
    else
        UpdateExercises();
}
