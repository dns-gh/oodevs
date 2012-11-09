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
#include "ExerciseListView.h"
#include "clients_gui/tools.h"
#include "clients_kernel/Controllers.h"
#include "frontend/commands.h"
#include "frontend/ListViewHelper.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include "tools/Version.h"
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
ExportWidget::ExportWidget( QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers )
    : gui::LanguageChangeObserver_ABC< Q3GroupBox >( 2, Qt::Vertical, parent )
    , config_     ( config )
    , fileLoader_ ( fileLoader )
    , controllers_( controllers )
{
    setFrameShape( Q3GroupBox::DummyFrame::NoFrame );
    tabs_ = new QTabWidget( this );

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
            exerciseList_ = new ExerciseListView( box, config, fileLoader );
            connect( exerciseList_->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( OnSelectionChanged( const QModelIndex&, const QModelIndex& ) ) );
        }
        {
            packageContentLabel_ = new QLabel( box );
            exerciseContent_ = new QTreeView( box );
            exerciseContent_->setModel( &exerciseContentModel_ );
            exerciseContent_->setHeaderHidden( true );
            exerciseContent_->setEditTriggers( 0 );
            exerciseContent_->adjustSize();
            exerciseContent_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
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
            terrainList_ = new QListWidget( box );
            terrainList_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
            connect( terrainList_, SIGNAL( itemClicked( QListWidgetItem* ) ), SLOT( OnSelectionChanged( QListWidgetItem* ) ) );
        }
    }
    // eTabs_Models
    {
        Q3GroupBox* box = AddTab( this, tabs_ );
        {
            modelsNameLabel_ = new QLabel( box );
            modelName_ = new QLineEdit( box );
            modelName_->setMaximumHeight( 30 );
            connect( modelName_, SIGNAL( textEdited( const QString& ) ), SLOT( OnModelNameChanged( const QString& ) ) );
        }
        {
            modelsDescriptionLabel_ = new QLabel( box );
            modelDescription_ = new QTextEdit( box );
            modelDescription_->setMaximumHeight( 30 );
        }
        {
            modelsDecisionalLabel_ = new QLabel( box );
            decisionalCheckBox_ = new QCheckBox( box );
            decisionalCheckBox_->setEnabled( false );
        }
        {
            modelsPhysicalLabel_ = new QLabel( box );
            physicalList_ = new QListWidget( box );
            physicalList_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
            connect( physicalList_, SIGNAL( itemClicked( QListWidgetItem* ) ), SLOT( OnSelectionChanged( QListWidgetItem* ) ) );
        }
    }
    progress_ = new QProgressBar( this );
    progress_->hide();
    package_.first = config_.GetRootDir();
    controllers_.Register( *this );
    connect( tabs_, SIGNAL( currentChanged( QWidget* ) ), SLOT( OnButtonChanged() ) );
    Update();
}

// -----------------------------------------------------------------------------
// Name: ExportWidget destructor
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
ExportWidget::~ExportWidget()
{
    controllers_.Unregister( *this );
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
    modelsNameLabel_->setText(          tools::translate( "ExportWidget", "Model name:" ) );
    modelsDescriptionLabel_->setText(   tools::translate( "ExportWidget", "Package description:" ) );
    modelsDecisionalLabel_->setText(    tools::translate( "ExportWidget", "Decisional model:" ) );
    modelsPhysicalLabel_->setText(      tools::translate( "ExportWidget", "Physical model:" ) );
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::GetCurrentSelection
// Created: ABR 2011-11-03
// -----------------------------------------------------------------------------
QString ExportWidget::GetCurrentSelection() const
{
    QString result;
    switch( tabs_->currentIndex() )
    {
    case eTabs_Exercise:
        if( const QStandardItem* item = exerciseList_->GetSelectedExerciseItem() )
            result = item->text();
        break;
    case eTabs_Terrain:
        if( QListWidgetItem* item = terrainList_->currentItem() )
            result = item->text();
        break;
    case eTabs_Models:
        if( !modelName_->text().isEmpty() )
            if( QListWidgetItem* item = physicalList_->currentItem() )
                result = item->text();
        break;
    default:
        break;
    }
    return result;
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

namespace
{
    std::pair< std::string, std::string > Extract( const QString& text )
    {
        std::string selectedText = text.toStdString();
        size_t separator = selectedText.find_first_of( '/' );
        std::string base = selectedText.substr( 0, separator );
        std::string physical = selectedText.substr( separator, std::string::npos );
        return std::make_pair( base, physical );
    }
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::GetCurrentPackage
// Created: LGY 2012-06-14
// -----------------------------------------------------------------------------
QString ExportWidget::GetCurrentPackage() const
{
    QString text = GetCurrentSelection();
    switch( tabs_->currentIndex() )
    {
    case eTabs_Exercise:
        return text;
    case eTabs_Terrain:
        return text;
    case eTabs_Models:
        return modelName_->text() + Extract( text ).second.c_str();
    default:
        break;
    }
    return text;
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::IsButtonEnabled
// Created: JSR 2010-07-19
// -----------------------------------------------------------------------------
bool ExportWidget::IsButtonEnabled()
{
    QString text = GetCurrentSelection();
    if( !text.isEmpty() )
        package_.second = std::string( text ) + ".otpak";
    return !text.isEmpty();
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::OnSelectionChanged
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
void ExportWidget::OnSelectionChanged( const QModelIndex& modelIndex, const QModelIndex& /*previous*/ )
{
    std::string exercise( exerciseList_->GetExerciseName( modelIndex ) );
    exerciseContentModel_.clear();
    if( !exercise.empty() )
    {
        frontend::BuildExerciseFeatures( exercise, config_, exerciseContentModel_ );
        frontend::BuildExerciseData( exercise, config_, exerciseContentModel_, fileLoader_ );
        exerciseContent_->expandAll();
    }
    OnButtonChanged();
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::OnSelectionChanged
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
void ExportWidget::OnSelectionChanged( QListWidgetItem* item )
{
    Update( item );
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::OnModelNameChanged
// Created: LGY 2012-06-14
// -----------------------------------------------------------------------------
void ExportWidget::OnModelNameChanged( const QString& /*text*/ )
{
    OnButtonChanged();
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::Update
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
void ExportWidget::Update( QListWidgetItem* item /*= 0*/ )
{
    if( !item ) // No item, refresh lists
    {
        // Terrain
        terrainList_->clear();
        terrainList_->addItems( frontend::commands::ListTerrains( config_ ) );
        // Physical
        decisionalCheckBox_->setEnabled( false );
        physicalList_->clear();
        QStringList physicalBase;
        QStringList decisionalModels = frontend::commands::ListModels( config_ );
        for( QStringList::const_iterator it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
        {
            const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).toStdString() );
            for( QStringList::const_iterator itP = physicalModels.begin(); itP != physicalModels.end(); ++itP )
                physicalBase << QString( "%1/%2" ).arg( *it ).arg( *itP );
        }
        physicalList_->addItems( physicalBase );
        modelName_->clear();
    }
    else if( tabs_->currentIndex() == eTabs_Models )
    {
        modelName_->setText( Extract( physicalList_->currentItem()->text() ).first.c_str() );
        decisionalCheckBox_->setEnabled( true );
    }
    OnButtonChanged();
}

namespace
{
    void AddProgress( QProgressBar* progress, int value )
    {
        progress->setValue( std::min( 98, progress->value() + value ) );
        qApp->processEvents();
    }

    void Copy( std::istream& file, std::ostream& output )
    {
        std::istreambuf_iterator< char > it( file );
        std::istreambuf_iterator< char > end;
        std::ostreambuf_iterator< char > out( output );
        std::copy( it, end, out );
    }

    void CopyFile( const bfs::path& root, const std::string& name, zip::ozipfile& zos, QProgressBar* progress )
    {
        std::string filename( root.string() );
        std::ifstream file( filename.c_str(), std::ifstream::in | std::ifstream::binary );
        if( file.good() )
        {
            zip::ozipstream stream( zos, name.c_str(), std::ios_base::out | std::ios_base::binary );
            Copy( file, stream );
            AddProgress( progress, 10 );
        }
    }

    void BrowseDirectory( const bfs::path& root, const std::string& name, zip::ozipfile& zos, bool recursive, QProgressBar* progress )
    {
        bfs::directory_iterator end;
        for( bfs::directory_iterator it( root ); it != end; ++it )
        {
            const bfs::path child = *it;
            if( bfs::is_regular_file( child ) )
            {
                const std::string& file( child.filename().string() );
                CopyFile( ( root / file.c_str() ), name + "/" + file, zos, progress );
            }
            else if( recursive && bfs::is_directory( child ) && child.filename() != ".svn" )
            {
                BrowseDirectory( child, name + "/" + child.filename().string(), zos, recursive, progress );
            }
            AddProgress( progress, 2 );
        }
    }

    void Serialize( const std::string& base, const std::string& name, zip::ozipfile& zos, bool recursive, QProgressBar* progress, const std::string& exportName = "" )
    {
        const bfs::path root = bfs::path( base ) / name;
        if( ! bfs::exists( root ) )
            return;
        if( ! bfs::is_directory( root ) )
            CopyFile( root, name, zos, progress );
        else
            BrowseDirectory( root, exportName != "" ? exportName : name, zos, recursive, progress );
    }

    void BrowseChildren( const std::string& base, QStandardItem* item, zip::ozipfile& zos, QProgressBar* progress, bool recursive )
    {
        int row = 0;
        while( row < item->rowCount() && !dynamic_cast< frontend::CheckListItem* >( item ) )
        {
            std::string file( item->child( row )->text().toStdString() );
            Serialize( base, file, zos, recursive, progress );
            AddProgress( progress, 2 );
            ++row;
        }
    }

    void BrowseFiles( const std::string& base, const QStandardItemModel& model, zip::ozipfile& zos, QProgressBar* progress )
    {
        for( int row = 0; row < model.rowCount(); ++row )
        {
            if( model.item( row )->hasChildren() )
            {
                QStandardItem* treeNode = model.item( row );
                for( int row2 = 0; row2 < treeNode->rowCount(); ++row2 )
                {
                    frontend::CheckListItem* item = dynamic_cast< frontend::CheckListItem* >( treeNode->child( row2 ) );
                    if( item && item->checkState() == Qt::Checked )
                    {
                        std::string file( item->text().toStdString() );
                        Serialize( base, file, zos, item->IsRecursive(), progress );
                        if( item->hasChildren() )
                            BrowseChildren( base, item, zos, progress, item->IsRecursive() );
                    }
                    AddProgress( progress, 2 );
                }
            }
        }
    }

    int ListViewSize( const QStandardItemModel& model, QStandardItem* parent  = 0 )
    {
        if( !parent )
            parent = model.invisibleRootItem();

        int itemCount = parent->rowCount();
        if( parent->hasChildren() )
            for( int row = 0; row < parent->rowCount(); ++row )
            itemCount += parent->hasChildren() ? ListViewSize( model, parent->child( row ) ) : 0 ;
        return itemCount;
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
    const bfs::path file = bfs::path( std::string( filename.toStdString() ) );
    package_.first = file.parent_path().string();
    package_.second = file.filename().string();
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
    QString text = GetCurrentSelection();
    if( !text.isEmpty() )
    {
        std::string description = GetCurrentDescription()->text().toStdString();
        QString package = GetCurrentPackage();

        if( description.empty() )
            description = "Packaged scenario of " + std::string( package.toStdString() ) + ".";
        xos << xml::start( "content" )
            << xml::content( "name", package.toStdString() )
            << xml::content( "description", description )
            << xml::content( "version", tools::AppProjectVersion() )
            << xml::end;
        {
            std::istringstream input( xos.str() );
            zip::ozipstream output( archive, "content.xml" );
            Copy( input, output );
        }
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
        progress_->setValue( 0 );
        progress_->setMaximum( ListViewSize( exerciseContentModel_ ) );
        BrowseFiles( config_.GetRootDir(), exerciseContentModel_, archive, progress_ );
        break;
    case eTabs_Terrain:
        {
            assert( terrainList_->currentItem() );
            progress_->setValue( 0 );
            progress_->setMaximum( 100 );
            bfs::path diffPath = GetDiffPath( config_.GetRootDir(), config_.GetTerrainDir( terrainList_->currentItem()->text().toStdString() ) );
            Serialize( config_.GetRootDir(), diffPath.string(), archive, true, progress_ );
            progress_->setValue( 100 );
        }
        break;
    case eTabs_Models:
        {
            assert( physicalList_->currentItem() );
            std::pair< std::string, std::string > content( Extract( physicalList_->currentItem()->text() ) );

            bfs::path diffPath = GetDiffPath( config_.GetRootDir(), config_.GetModelsDir() ) / content.first;
            bfs::path exportPath = GetDiffPath( config_.GetRootDir(), config_.GetModelsDir() ) / modelName_->text().toStdString();

            if( decisionalCheckBox_->isChecked() )
            {
                progress_->setValue( 0 );
                progress_->setMaximum( 100 );
                Serialize( config_.GetRootDir(), bfs::path( diffPath / "physical" / content.second ).string(), archive, true, progress_,
                                                 bfs::path( exportPath / "physical" / content.second ).string() );
                progress_->setValue( 50 );
                Serialize( config_.GetRootDir(), bfs::path( diffPath / "decisional" ).string(), archive, true, progress_,
                                                 bfs::path( exportPath / "decisional" ).string() );
                progress_->setValue( 100 );
            }
            else
            {
                progress_->setValue( 0 );
                progress_->setMaximum( 100 );
                Serialize( config_.GetRootDir(), bfs::path( diffPath / "physical" / content.second ).string(), archive, true, progress_,
                                                 bfs::path( exportPath / "physical" / content.second ).string() );
                progress_->setValue( 100 );
            }
            break;
        }
    default:
        break;
    }
    setCursor( Qt::arrowCursor );
    progress_->hide();
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::NotifyCreated
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
void ExportWidget::NotifyCreated( const frontend::Exercise_ABC& exercise )
{
    exerciseList_->AddExerciseEntry( exercise );
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::NotifyDeleted
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
void ExportWidget::NotifyDeleted( const frontend::Exercise_ABC& exercise )
{
    exerciseList_->DeleteExerciseEntry( exercise );
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::OnButtonChanged
// Created: BAX 2012-10-24
// -----------------------------------------------------------------------------
void ExportWidget::OnButtonChanged()
{
    emit ButtonChanged( IsButtonEnabled(), tools::translate( "Page_ABC", "Export" ) );
}