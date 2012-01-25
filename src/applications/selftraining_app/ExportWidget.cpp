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

namespace
{
    Q3GroupBox* AddTab( QWidget* parent, QTabWidget* tabs )
    {
        Q3GroupBox* importGroup = new Q3GroupBox( 2, Qt::Horizontal, parent );
        importGroup->setFrameShape( Q3GroupBox::DummyFrame::NoFrame );
        importGroup->setMargin( 0 );
        tabs->addTab( importGroup, "" );
        return importGroup;
    }
}

// -----------------------------------------------------------------------------
// Name: ExportWidget constructor
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
ExportWidget::ExportWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader )
    : gui::LanguageChangeObserver_ABC< Q3GroupBox >( 2, Qt::Vertical, parent )
    , config_    ( config )
    , fileLoader_( fileLoader )
    , page_      ( page )
{
    setFrameShape( Q3GroupBox::DummyFrame::NoFrame );
    tabs_ = new QTabWidget( this );
    connect( tabs_, SIGNAL( currentChanged( QWidget* ) ), &page, SLOT( UpdateEditButton( QWidget* ) ) );

    // eTabs_Exercise
    {
        Q3GroupBox* box = AddTab( this, tabs_ );
        {
            exerciseDescriptionLabel_ = new QLabel( box );
            exerciseDescription_ = new QTextEdit( box );
            exerciseDescription_->setMaximumHeight( 30 );
        }
        {
            exerciseLabel_ = new QLabel( box );
            exerciseList_ = new Q3ListBox( box );
            connect( exerciseList_, SIGNAL( clicked( Q3ListBoxItem* ) ), SLOT( OnSelectionChanged( Q3ListBoxItem* ) ) );
        }
        {
            packageContentLabel_ = new QLabel( box );
            exerciseContent_ = new Q3ListView( box );
            exerciseContent_->addColumn( "exercise features" );
            exerciseContent_->setResizeMode( Q3ListView::AllColumns );
            exerciseContent_->header()->hide();
            exerciseContent_->adjustSize();
        }
    }
    // eTabs_Terrain
    {
        Q3GroupBox* box = AddTab( this, tabs_ );
        {
            terrainDescriptionLabel_ = new QLabel( box );
            terrainDescription_ = new QTextEdit( box );
            terrainDescription_->setMaximumHeight( 30 );
        }
        {
            terrainLabel_ = new QLabel( box );
            terrainList_ = new Q3ListBox( box );
            connect( terrainList_, SIGNAL( clicked( Q3ListBoxItem* ) ), SLOT( OnSelectionChanged( Q3ListBoxItem* ) ) );
        }
    }
    // eTabs_Models
    {
        Q3GroupBox* box = AddTab( this, tabs_ ); 
        {
            modelsDescriptionLabel_ = new QLabel( box );
            modelDescription_ = new QTextEdit( box );
            modelDescription_->setMaximumHeight( 30 );
        }
        {
            modelsPhysicalLabel_ = new QLabel( box );
            physicalList_ = new Q3ListBox( box );
            connect( physicalList_, SIGNAL( clicked( Q3ListBoxItem* ) ), SLOT( OnSelectionChanged( Q3ListBoxItem* ) ) );
        }
        {
            decisionalCheckBox_ = new QCheckBox( box );
            decisionalCheckBox_->setEnabled( false );
            decisionalCheckBox_->setEnabled( false );
        }
    }
    progress_ = new Q3ProgressBar( this );
    progress_->hide();
    package_.first = config_.GetRootDir();
    Update();
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
// Name: ExportWidget::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void ExportWidget::OnLanguageChanged()
{
    tabs_->setTabText( eTabs_Exercise,  tools::translate( "ExportWidget", "Exercise" ) );
    tabs_->setTabText( eTabs_Terrain,   tools::translate( "ExportWidget", "Terrain" ) );
    tabs_->setTabText( eTabs_Models,    tools::translate( "ExportWidget", "Models" ) );

    exerciseDescriptionLabel_->setText( tools::translate( "ExportWidget", "Package description:" ) );
    exerciseLabel_->setText(            tools::translate( "ExportWidget", "Exercise:" ) );
    packageContentLabel_->setText(      tools::translate( "ExportWidget", "Package content:" ) );
    terrainDescriptionLabel_->setText(  tools::translate( "ExportWidget", "Package description:" ) );
    terrainLabel_->setText(             tools::translate( "ExportWidget", "Terrain:" ) );
    modelsDescriptionLabel_->setText(   tools::translate( "ExportWidget", "Package description:" ) );
    decisionalCheckBox_->setText(		tools::translate( "ExportWidget", "Include decisional model" ) );
    modelsPhysicalLabel_->setText(      tools::translate( "ExportWidget", "Physical model:" ) );
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::GetCurrentSelection
// Created: ABR 2011-11-03
// -----------------------------------------------------------------------------
Q3ListBoxItem* ExportWidget::GetCurrentSelection() const
{
    Q3ListBoxItem* item = 0;
    switch( tabs_->currentIndex() )
    {
    case eTabs_Exercise:
        item = exerciseList_->selectedItem();
        break;
    case eTabs_Terrain:
        item = terrainList_->selectedItem();
        break;
    case eTabs_Models:
        item = physicalList_->selectedItem();
        break;
    default:
        break;
    }
    return item;
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::GetCurrentDescription
// Created: ABR 2011-11-03
// -----------------------------------------------------------------------------
QTextEdit* ExportWidget::GetCurrentDescription() const
{
    switch( tabs_->currentIndex() )
    {
    case eTabs_Exercise:
        return exerciseDescription_;
    case eTabs_Terrain:
        return terrainDescription_;
    case eTabs_Models:
        return modelDescription_;
    default:
        break;
    }
    assert( false );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::EnableEditButton
// Created: JSR 2010-07-19
// -----------------------------------------------------------------------------
bool ExportWidget::EnableEditButton()
{
    Q3ListBoxItem* item = GetCurrentSelection();
    if( item )
        package_.second = std::string( item->text().ascii() ) + ".otpak";
    return item != 0;
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::OnSelectionChanged
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
void ExportWidget::OnSelectionChanged( Q3ListBoxItem* item )
{
    Update( item );
    page_.UpdateEditButton();
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::Update
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
void ExportWidget::Update( Q3ListBoxItem* item /*= 0*/ )
{
    if( !item ) // No item, refresh lists
    {
        // Exercise
        exerciseList_->clear();
        exerciseList_->insertStringList( frontend::commands::ListExercises( config_ ) );
        // Terrain
        terrainList_->clear();
        terrainList_->insertStringList( frontend::commands::ListTerrains( config_ ) );
        // Physical
        decisionalCheckBox_->setEnabled( false );
        physicalList_->clear();
        QStringList physicalBase;
        QStringList decisionalModels = frontend::commands::ListModels( config_ );
        for( QStringList::const_iterator it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
        {
            const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).ascii() );
            for( QStringList::const_iterator itP = physicalModels.begin(); itP != physicalModels.end(); ++itP )
                physicalBase << QString( "%1/%2" ).arg( *it ).arg( *itP );
        }
        physicalList_->insertStringList( physicalBase );
        exerciseContent_->clear();
    }
    else // Item, update content
    {
        switch( tabs_->currentIndex() )
        {
        case eTabs_Exercise:
            {
                std::string exercise( item->text().ascii() );
                exerciseContent_->clear();
                exerciseContent_->insertItem( frontend::BuildExerciseFeatures( exercise, config_, exerciseContent_ ) );
                exerciseContent_->insertItem( frontend::BuildExerciseData( exercise, config_, exerciseContent_, fileLoader_ ) );
            }
            break;
        case eTabs_Models:
            decisionalCheckBox_->setEnabled( true );
            break;
        default:
            break;
        }
    }
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

    void CopyFile( const bfs::path& root, const std::string& name, zip::ozipfile& zos )
    {
        std::string filename( root.file_string() );
        std::ifstream file( filename.c_str(), std::ifstream::in | std::ifstream::binary );
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

    bfs::path GetDiffPath( bfs::path basepath, bfs::path other )
    {
        bfs::path diff;
        while( other != basepath )
        {
            diff = other.stem() / diff;
            other = other.parent_path();
        }
        return diff;
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
            InternalExportPackage( archive );
    }
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
    Q3ListBoxItem* item = GetCurrentSelection();
    assert( item );
    std::string name = item->text().ascii();
    std::string description = GetCurrentDescription()->text().ascii();

    if( description.empty() )
        description = "Packaged scenario of " + name + ".";
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

// -----------------------------------------------------------------------------
// Name: ExportWidget::InternalExportPackage
// Created: ABR 2011-11-03
// -----------------------------------------------------------------------------
void ExportWidget::InternalExportPackage( zip::ozipfile& archive )
{
    progress_->show();
    setCursor( Qt::waitCursor );
    WriteContent( archive );
    switch( tabs_->currentIndex() )
    {
    case eTabs_Exercise:
        progress_->setProgress( 0, ListViewSize( Q3ListViewItemIterator( exerciseContent_ ) ) );
        BrowseFiles( config_.GetRootDir(), Q3ListViewItemIterator( exerciseContent_ ), archive, Progress( progress_ ) );
        break;
    case eTabs_Terrain:
        {
            assert( terrainList_->selectedItem() );
            progress_->setProgress( 0, 1 );
            bfs::path diffPath = GetDiffPath( config_.GetRootDir(), config_.GetTerrainDir( terrainList_->selectedItem()->text().ascii() ) );
            Serialize( config_.GetRootDir(), diffPath.string(), archive, true );
            progress_->setProgress( 1 );
        }
        break;
    case eTabs_Models:
        {
            assert( physicalList_->selectedItem() );
            std::string selectedText = physicalList_->selectedItem()->text().ascii();
            size_t separator = selectedText.find_first_of( '/' );
            std::string base = selectedText.substr( 0, separator );
            std::string physical = selectedText.substr( separator, std::string::npos );
            bfs::path diffPath = GetDiffPath( config_.GetRootDir(), config_.GetModelsDir() ) / base;
            if( decisionalCheckBox_->isChecked() )
            {
                progress_->setProgress( 0, 2 );
                Serialize( config_.GetRootDir(), bfs::path( diffPath / "physical" / physical ).string(), archive, true );
                progress_->setProgress( 1 );
                Serialize( config_.GetRootDir(), bfs::path( diffPath / "decisional" ).string(), archive, true );
                progress_->setProgress( 2 );
            }
            else
            {
                progress_->setProgress( 0, 1 );
                Serialize( config_.GetRootDir(), bfs::path( diffPath / "physical" / physical ).string(), archive, true );
                progress_->setProgress( 1 );
            }
            break;
        }
    default:
        break;
    }
    setCursor( Qt::arrowCursor );
    progress_->hide();
}
