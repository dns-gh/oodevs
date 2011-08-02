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
#include "clients_gui/Tools.h"
#include <xeumeuleu/xml.hpp>
#include <zipstream/zipstream.h>



#pragma warning( push, 0 )
#include <Qt3Support/q3action.h>
#include <QtGui/qcursor.h>
#include <Qt3Support/q3filedialog.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <Qt3Support/q3listbox.h>
#include <QtGui/qpushbutton.h>
#include <Qt3Support/q3progressbar.h>
#include <Qt3Support/q3textedit.h>
#include <QtGui/qapplication.h>
#include <Qt3Support/q3listview.h>
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
CreatePackagePanel::CreatePackagePanel( Q3WidgetStack* widget, Q3Action& action, const frontend::Config& config, ActionsContext& context )
    : Panel_ABC( widget, action, context, "CreatePackagePanel" )
    , config_( config )
{
    Q3VBox* box = new Q3VBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    Q3GroupBox* group = new Q3GroupBox( 2, Qt::Horizontal, action.text(), box );
    {
        new QLabel( tr( "Exercise to package:" ), group );
        list_ = new Q3ListBox( group );
    }
    {
        new QLabel( tr( "Package description:" ), group );
        description_ = new Q3TextEdit( group );
        description_->setMaximumHeight( 30 );
        description_->setReadOnly( false );
    }
    {
        new QLabel( tr( "Package content:" ), group );
        content_ = new Q3ListView( group );
        content_->addColumn( "exercise features" );
        content_->adjustSize();
    }

    progress_ = new Q3ProgressBar( box );
    progress_->hide();

    bubble_ = new InfoBubble( box );
    Q3HBox* btnBox = new Q3HBox( box );
    btnBox->layout()->setAlignment( Qt::AlignRight );
    okay_ = new QPushButton( MAKE_PIXMAP( next ), action.text(), btnBox );
    QFont font( "Arial", 10, QFont::Bold );
    okay_->setFont( font );

    connect( okay_, SIGNAL( pressed() ), SLOT( CreatePackage() ) );
    connect( list_, SIGNAL( clicked( Q3ListBoxItem * ) ), SLOT( OnSelectionChanged( Q3ListBoxItem * ) ) );

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
    const QString filename = Q3FileDialog::getSaveFileName( package_.second.c_str(), "SWORD packages (*.otpak)", this, "", tr( "Select a package" ) );
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
void CreatePackagePanel::OnSelectionChanged( Q3ListBoxItem* item )
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
        Progress( Q3ProgressBar* progress ) : progress_( progress ), count_( 0 ) {}
        void operator()()
        {
            progress_->setProgress( ++count_ );
            qApp->processEvents();
        }
        Q3ProgressBar* progress_;
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

    void BrowseChildren( const std::string& base, Q3ListViewItem* item, zip::ozipfile& zos, boost::function0<void> callback )
    {
        while ( item != 0 && ! dynamic_cast< Q3CheckListItem* >( item ) )
        {
            std::string file( item->text( 0 ).ascii() );
            Serialize( base, file, zos );
            callback();
            item = item->nextSibling();
        }
    }

    void BrowseFiles( const std::string& base, Q3ListViewItemIterator iterator, zip::ozipfile& zos, boost::function0<void> callback )
    {
        while ( iterator.current() )
        {
            Q3CheckListItem* item = dynamic_cast< Q3CheckListItem* >( iterator.current() );
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

    int ListViewSize( Q3ListViewItemIterator iterator )
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
        << xml::end;
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
        progress_->setProgress( 0, ListViewSize( Q3ListViewItemIterator( content_ ) ) );
        setCursor( Qt::waitCursor );
        {
            WriteContent( archive );
            BrowseFiles( config_.GetRootDir(), Q3ListViewItemIterator( content_ ), archive, Progress( progress_ ) );
        }
        setCursor( Qt::arrowCursor );
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
void CreatePackagePanel::BuildCategory( Q3ListViewItem* parent, const QStringList& list, const QString& base, const std::string& category )
{
    if( !list.isEmpty() )
    {
        Q3ListViewItem* item = new Q3ListViewItem( parent, category.c_str() );
        item->setOpen( true );
        for ( QStringList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it )
            item->insertItem( new Q3CheckListItem( item, base + std::string( "/" + category + "/" ).c_str() + *it, Q3CheckListItem::CheckBox ) );
        parent->insertItem( item );
    }
}

// -----------------------------------------------------------------------------
// Name: CreatePackagePanel::BuildExerciseFeatures
// Created: JCR 2009-11-12
// -----------------------------------------------------------------------------
Q3ListViewItem* CreatePackagePanel::BuildExerciseFeatures( const std::string& exercise )
{
    QString base( std::string( "exercises/" + exercise ).c_str() );

    Q3ListViewItem* rootItem = new Q3ListViewItem( content_, "exercises" );
    rootItem->setOpen( true );

    Q3CheckListItem* exerciseItem = new Q3CheckListItem( rootItem, base, Q3CheckListItem::CheckBox );
    exerciseItem->setOpen( true );
    exerciseItem->setOn( true );
    exerciseItem->setEnabled( false );
    QStringList sessions( frontend::commands::ListSessions( config_, exercise ) );
    for ( QStringList::const_iterator it = sessions.constBegin(); it != sessions.constEnd(); ++it )
    {
        Q3CheckListItem* sessionItem = new Q3CheckListItem( exerciseItem, base + "/sessions/" + *it, Q3CheckListItem::CheckBox );
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
    Q3ListViewItem* InsertValidatedEntry( Q3ListViewItem* parent, const std::string& entry, const std::string& root )
    {
        Q3CheckListItem* item = new Q3CheckListItem( parent, entry.c_str(), Q3CheckListItem::CheckBox );
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
Q3ListViewItem* CreatePackagePanel::BuildExerciseData( const std::string& exercise )
{
    std::string terrain, population, dataset, physical;
    xml::xifstream xis( config_.GetExerciseFile( exercise ) );
    xis >> xml::start( "exercise" )
            >> xml::start( "terrain" ) >> xml::attribute( "name", terrain ) >> xml::end
            >> xml::optional >> xml::start( "population" ) >> xml::attribute( "name", population ) >> xml::end
            >> xml::start( "model" ) >> xml::attribute( "dataset", dataset ) >> xml::attribute( "physical", physical ) >> xml::end
        >> xml::end;

    Q3ListViewItem* dataItem = new Q3ListViewItem( content_, "data" );
    dataItem->setOpen( true );

    Q3ListViewItem* terrainItem = InsertValidatedEntry( dataItem, "data/terrains/" + terrain, config_.GetRootDir() );
    if( terrainItem )
    {
        terrainItem->insertItem( new Q3ListViewItem( terrainItem, std::string( "data/terrains/" + terrain + "/Detection" ).c_str() ) );
        terrainItem->insertItem( new Q3ListViewItem( terrainItem, std::string( "data/terrains/" + terrain + "/Graphics" ).c_str() ) );
        terrainItem->insertItem( new Q3ListViewItem( terrainItem, std::string( "data/terrains/" + terrain + "/Pathfind" ).c_str() ) );
    }

    QStringList propagations( frontend::commands::ListPropagations( config_ ) );
    BuildCategory( dataItem, propagations, "data", "propagations" );

    if( ! population.empty() )
    {
        Q3ListViewItem* populationItem = InsertValidatedEntry( dataItem, "data/population/" + population, config_.GetRootDir() );
        populationItem->insertItem( new Q3ListViewItem( populationItem, std::string( "data/population/" + population + "/model" ).c_str() ) );
        populationItem->insertItem( new Q3ListViewItem( populationItem, std::string( "data/population/" + population + "/navteq" ).c_str() ) );
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
    Q3ListBoxItem* item = list_->selectedItem();
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
