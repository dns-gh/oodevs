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
#include "frontend/ListViewHelper.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <xeumeuleu/xml.h>
#include <zipstream/zipstream.h>

namespace bfs = boost::filesystem;
// -----------------------------------------------------------------------------
// Name: ExportWidget constructor
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
ExportWidget::ExportWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader )
    : Q3GroupBox( 2, Qt::Vertical, parent )
    , config_( config )
    , fileLoader_( fileLoader )
    , page_( page )
{
    setFrameShape( Q3GroupBox::DummyFrame::NoFrame );
    setMargin( 5 );
    setBackgroundOrigin( QWidget::WindowOrigin );
    Q3GroupBox* group = new Q3GroupBox( 2, Qt::Horizontal, tools::translate( "ExportWidget", "Create a package" ), this );
    group->setBackgroundOrigin( QWidget::WindowOrigin );
    {
        QLabel* label = new QLabel( tools::translate( "ExportWidget", "Exercise to package:" ), group );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        list_ = new Q3ListBox( group );
        list_->setBackgroundOrigin( QWidget::WindowOrigin );
        connect( list_, SIGNAL( clicked( Q3ListBoxItem* ) ), SLOT( OnSelectionChanged( Q3ListBoxItem* ) ) );
    }
    {
        QLabel* label = new QLabel(  tools::translate( "ExportWidget", "Package description:" ), group );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        description_ = new Q3TextEdit( group );
        description_->setBackgroundOrigin( QWidget::WindowOrigin );
        description_->setMaximumHeight( 30 );
        description_->setReadOnly( false );
    }
    {
        QLabel* label = new QLabel(  tools::translate( "ExportWidget", "Package content:" ), group );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        content_ = new Q3ListView( group );
        content_->setBackgroundOrigin( QWidget::WindowOrigin );
        content_->addColumn( "exercise features" );
        content_->setResizeMode( Q3ListView::AllColumns );
        content_->header()->hide();
        content_->adjustSize();
    }
    progress_ = new Q3ProgressBar( this );
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
    Q3ListBoxItem* item = list_->selectedItem();
    if( item )
    {
        std::string exercise( item->text().ascii() );
        content_->clear();
        content_->insertItem( frontend::BuildExerciseFeatures( exercise, config_, content_ ) );
        content_->insertItem( frontend::BuildExerciseData( exercise, config_, content_, fileLoader_ ) );
   }
    else
        UpdateExercises();
}

namespace
{
    struct Progress
    {
        Progress( Q3ProgressBar* progress ) : progress_( progress ), count_( 0 ) {}
        void operator()()
        {
            progress_->setProgress( ++count_ );
            qApp->processEvents();
        }
        Q3ProgressBar* progress_;
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

    void BrowseChildren( const std::string& base, Q3ListViewItem* item, zip::ozipfile& zos, boost::function0<void> callback, bool recursive )
    {
        while ( item != 0 && ! dynamic_cast< frontend::CheckListItem* >( item ) )
        {
            std::string file( item->text( 0 ).ascii() );
            Serialize( base, file, zos, recursive );
            callback();
            item = item->nextSibling();
        }
    }

    void BrowseFiles( const std::string& base, Q3ListViewItemIterator iterator, zip::ozipfile& zos, boost::function0<void> callback )
    {
        while ( iterator.current() )
        {
            frontend::CheckListItem* item = dynamic_cast< frontend::CheckListItem* >( iterator.current() );
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

    int ListViewSize( Q3ListViewItemIterator iterator )
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
            progress_->setProgress( 0, ListViewSize( Q3ListViewItemIterator( content_ ) ) );
            setCursor( Qt::waitCursor );
            {
                WriteContent( archive );
                BrowseFiles( config_.GetRootDir(), Q3ListViewItemIterator( content_ ), archive, Progress( progress_ ) );
            }
            setCursor( Qt::arrowCursor );
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
void ExportWidget::OnSelectionChanged( Q3ListBoxItem* item )
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

// Name: ExportWidget::BrowseClicked
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
bool ExportWidget::BrowseClicked()
{
    const QString filename = Q3FileDialog::getSaveFileName( package_.second.c_str(), "SWORD packages (*.otpak)", this, "", tools::translate( "ExportWidget", "Select a package" ) );
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
