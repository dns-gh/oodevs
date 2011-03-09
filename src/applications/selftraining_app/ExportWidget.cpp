// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ExportWidget.h"
#include "moc_ExportWidget.cpp"
#include "ScenarioEditPage.h"
#include "clients_gui/tools.h"
#include "frontend/commands.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <qfiledialog.h>
#include <qheader.h>
#include <qlistview.h>
#include <qprogressbar.h>
#include <xeumeuleu/xml.h>
#include <zipstream/zipstream.h>

namespace bfs = boost::filesystem;

namespace
{
    class CheckListItem : public QCheckListItem
    {
    public:
        CheckListItem( QCheckListItem *parent, const QString &text, bool recursive )
            : QCheckListItem( parent, text, QCheckListItem::CheckBox )
            , recursive_( recursive )
        {}

        CheckListItem( QListViewItem *parent, const QString &text, bool recursive )
            : QCheckListItem( parent, text, QCheckListItem::CheckBox )
            , recursive_( recursive )
        {}

        bool IsRecursive()
        {
            return recursive_;
        }

    private:
        bool recursive_;
    };
}

// -----------------------------------------------------------------------------
// Name: ExportWidget constructor
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
ExportWidget::ExportWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader )
    : QGroupBox( 2, Qt::Vertical, parent )
    , config_( config )
    , fileLoader_( fileLoader )
    , page_( page )
{
    setFrameShape( QFrame::NoFrame );
    setMargin( 5 );
    setBackgroundOrigin( QWidget::WindowOrigin );
    QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, tools::translate( "ExportWidget", "Create a package" ), this );
    group->setBackgroundOrigin( QWidget::WindowOrigin );
    {
        QLabel* label = new QLabel(  tools::translate( "ExportWidget", "Exercise to package:" ), group );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        list_ = new QListBox( group );
        list_->setBackgroundOrigin( QWidget::WindowOrigin );
        connect( list_, SIGNAL( clicked( QListBoxItem* ) ), SLOT( OnSelectionChanged( QListBoxItem* ) ) );
    }
    {
        QLabel* label = new QLabel(  tools::translate( "ExportWidget", "Package description:" ), group );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        description_ = new QTextEdit( group );
        description_->setBackgroundOrigin( QWidget::WindowOrigin );
        description_->setMaximumHeight( 30 );
        description_->setReadOnly( false );
    }
    {
        QLabel* label = new QLabel(  tools::translate( "ExportWidget", "Package content:" ), group );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        content_ = new QListView( group );
        content_->setBackgroundOrigin( QWidget::WindowOrigin );
        content_->addColumn( "exercise features" );
        content_->setResizeMode( QListView::AllColumns );
        content_->header()->hide();
        content_->adjustSize();
    }
    progress_ = new QProgressBar( this );
    progress_->hide();
    package_.first = config_.GetRootDir();
    UpdateExercises();
}

// -----------------------------------------------------------------------------
// Name: ExportWidget destructor
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
ExportWidget::~ExportWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::Update
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
void ExportWidget::Update()
{
    QListBoxItem* item = list_->selectedItem();
    if( item )
    {
        std::string exercise( item->text().ascii() );
        content_->clear();
        content_->insertItem( BuildExerciseFeatures( exercise ) );
        content_->insertItem( BuildExerciseData( exercise ) );
   }
    else
        UpdateExercises();
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

    void BrowseDirectory( const bfs::path& root, const std::string& name, zip::ozipfile& zos, bool recursive )
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
            else if( recursive && bfs::is_directory( child ) && child.leaf() != ".svn" )
            {
                BrowseDirectory( child, name + "/" + child.filename(), zos, recursive );
            }
        }
    }

    void Serialize( const std::string& base, const std::string& name, zip::ozipfile& zos, bool recursive )
    {
        const bfs::path root = bfs::path( base, bfs::native ) / name;
        if( ! bfs::exists( root ) )
            return;
        if( ! bfs::is_directory( root ) )
            CopyFile( root, name, zos );
        else
            BrowseDirectory( root, name, zos, recursive );
    }

    void BrowseChildren( const std::string& base, QListViewItem* item, zip::ozipfile& zos, boost::function0<void> callback, bool recursive )
    {
        while ( item != 0 && ! dynamic_cast< CheckListItem* >( item ) )
        {
            std::string file( item->text( 0 ).ascii() );
            Serialize( base, file, zos, recursive );
            callback();
            item = item->nextSibling();
        }
    }

    void BrowseFiles( const std::string& base, QListViewItemIterator iterator, zip::ozipfile& zos, boost::function0<void> callback )
    {
        while ( iterator.current() )
        {
            CheckListItem* item = dynamic_cast< CheckListItem* >( iterator.current() );
            if( item && item->isOn() )
            {
                std::string file( iterator.current()->text( 0 ).ascii() );
                Serialize( base, file, zos, item->IsRecursive() );
                if( item->childCount() > 0 )
                    BrowseChildren( base, item->firstChild(), zos, callback, item->IsRecursive() );
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
// Name: ExportWidget::ExportPackage
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
void ExportWidget::ExportPackage()
{
    if( BrowseClicked() )
    {
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
            progress_->hide();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::EnableEditButton
// Created: JSR 2010-07-19
// -----------------------------------------------------------------------------
bool ExportWidget::EnableEditButton()
{
    return content_->childCount() != 0;
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::OnSelectionChanged
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
void ExportWidget::OnSelectionChanged( QListBoxItem* item )
{
    if( item )
    {
        package_.second = std::string( item->text().ascii() ) + ".otpak";
        Update();
        page_.UpdateEditButton();
    }
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::UpdateExercises
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
void ExportWidget::UpdateExercises()
{
    list_->clear();
    list_->insertStringList( frontend::commands::ListExercises( config_ ) );
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::BuildCategory
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
void ExportWidget::BuildCategory( QListViewItem* parent, const QStringList& list, const QString& base, const std::string& category )
{
    if( !list.isEmpty() )
    {
        QListViewItem* item = new QListViewItem( parent, category.c_str() );
        item->setOpen( true );
        for ( QStringList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it )
            item->insertItem( new CheckListItem( item,  base + std::string( "/" + category + "/" ).c_str() + *it, false ) );
        parent->insertItem( item );
    }
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::BuildExerciseFeatures
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
QListViewItem* ExportWidget::BuildExerciseFeatures( const std::string& exercise )
{
    QString base( std::string( "exercises/" + exercise ).c_str() );

    QListViewItem* rootItem = new QListViewItem( content_, "exercises" );
    rootItem->setOpen( true );

    CheckListItem* exerciseItem = new CheckListItem( rootItem, base, false );
    exerciseItem->setOpen( true );
    exerciseItem->setOn( true );
    exerciseItem->setEnabled( false );
    QStringList sessions( frontend::commands::ListSessions( config_, exercise ) );
    for ( QStringList::const_iterator it = sessions.constBegin(); it != sessions.constEnd(); ++it )
    {
        CheckListItem* sessionItem = new CheckListItem( exerciseItem, base + "/sessions/" + *it, false );
        sessionItem->setOpen( true );

        std::string category( QString( "sessions/" + *it + "/checkpoints" ).ascii() );
        const QStringList checkpoints = frontend::commands::ListCheckpoints( config_, exercise, ( *it ).ascii() );
        BuildCategory( sessionItem, checkpoints, base, category );
    }
    {
        const QStringList others = frontend::commands::ListOtherDirectories( config_, exercise );
        for ( QStringList::const_iterator it = others.constBegin(); it != others.constEnd(); ++it )
        {
            CheckListItem* dirItem = new CheckListItem( rootItem, base + "/" + *it, true );
            dirItem->setOpen( true );
            dirItem->setOn( false );
        }
    }
    return rootItem;
}

namespace
{
    QListViewItem* InsertValidatedEntry( QListViewItem* parent, const std::string& entry, const std::string& root )
    {
        CheckListItem* item = new CheckListItem( parent, entry.c_str(), false );
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
// Name: ExportWidget::BuildExerciseData
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
QListViewItem* ExportWidget::BuildExerciseData( const std::string& exercise )
{
    std::string terrain, population, dataset, physical;
    std::auto_ptr< xml::xistream > xis = fileLoader_.LoadFile( config_.GetExerciseFile( exercise ) );
    *xis >> xml::start( "exercise" )
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

    if( !population.empty() )
    {
        QListViewItem* populationItem = InsertValidatedEntry( dataItem, "data/population/" + population, config_.GetRootDir() );
        populationItem->insertItem( new QListViewItem( populationItem, std::string( "data/population/" + population + "/model" ).c_str() ) );
        populationItem->insertItem( new QListViewItem( populationItem, std::string( "data/population/" + population + "/navteq" ).c_str() ) );
    }
    InsertValidatedEntry( dataItem, "data/models/" + dataset + "/physical/" + physical, config_.GetRootDir() );
    return dataItem;
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::BrowseClicked
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
bool ExportWidget::BrowseClicked()
{
    const QString filename = QFileDialog::getSaveFileName( package_.second.c_str(), "SWORD packages (*.otpak)", this, "", tools::translate( "ExportWidget", "Select a package" ) );
    if( filename.isEmpty() )
        return false;
    const bfs::path file = bfs::path( std::string( filename.ascii() ), bfs::native );
    package_.first = file.parent_path().directory_string();
    package_.second = file.leaf().c_str();
    if( bfs::exists( file ) )
        return QMessageBox::question( this, tools::translate( "ExportWidget", "Overwrite File?" ),
                            tools::translate( "ExportWidget", "A file called %1 already exists. Do you want to overwrite it?" ).arg( filename ),
                            QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::WriteContent
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
void ExportWidget::WriteContent( zip::ozipfile& archive ) const
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
