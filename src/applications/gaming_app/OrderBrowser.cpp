// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "OrderBrowser.h"
#include "moc_OrderBrowser.cpp"
#include "icons.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: OrderBrowser constructor
// Created: ZEBRE 2007-05-11
// -----------------------------------------------------------------------------
OrderBrowser::OrderBrowser( QMainWindow* parent, const QString& directory )
    : QDockWindow( parent )
    , directory_( directory )
    , saving_   ( false )
    , pixmap_   ( MAKE_PIXMAP( mission ) )
{
    bfs::create_directories( bfs::path( directory.ascii(), bfs::native ) );
    QVBox* box = new QVBox( this );
    box->setFrameStyle( QVBox::Panel + QVBox::Sunken );

    {
        QHBox* titleBox = new QHBox( box );
        titleBox->setPaletteBackgroundColor( Qt::white );
        titleBox->setFixedHeight( 32 );
        QLabel* label = new QLabel( tr( " Orders" ), titleBox );
        QFont font = label->font();
        font.setBold( true );
        font.setPixelSize( 16 );
        label->setFont( font );
        label = new QLabel( titleBox );
        label->setPixmap( MAKE_PIXMAP( mission_title ) );
        label->setAlignment( Qt::AlignRight );
    }

    files_ = new QListView( box );
    files_->setMargin( 5 );
    files_->setFrameStyle( QFrame::Plain );
    files_->setMinimumSize( 300, 200 );
    files_->addColumn( tr( "Orders" ) );
    files_->setResizeMode( QListView::LastColumn );
    files_->header()->hide();
    connect( files_, SIGNAL( doubleClicked   ( QListViewItem* , const QPoint&, int ) ), SLOT( OnDoubleClicked   ( QListViewItem* ) ) );
    connect( files_, SIGNAL( selectionChanged( QListViewItem* ) ),                      SLOT( OnSelectionChanged( QListViewItem* ) ) );

    QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, "", box );
    group->setFlat( true );
    group->setInsideMargin( 3 );
    group->setInsideSpacing( 2 );
    QHBox* fileBox = new QHBox( group );
    QLabel* fileLabel = new QLabel( tr( "File name: " ), fileBox );
    fileBox->setStretchFactor( fileLabel, 1 );
    fileLabel_ = new QComboBox( true, fileBox );
    fileLabel_->setAutoCompletion( true );
    fileBox->setStretchFactor( fileLabel_, 42 );
    connect( fileLabel_, SIGNAL( textChanged( const QString& ) ), SLOT( OnTextChanged( const QString& ) ) );
    ok_ = new QPushButton( tr( "Ok" ), group );
    group->addSpace( 0 ); 
    connect( ok_, SIGNAL( clicked() ), SLOT( OnOk() ) );
    QPushButton* cancel = new QPushButton( tr( "Cancel" ), group );
    connect( cancel, SIGNAL( clicked() ), SLOT( OnCancel() ) );

    setWidget( box );
    setResizeEnabled( true );
    setCloseMode( QDockWindow::Never );
    setCaption( tr( "Order files" ) );
    parent->moveDockWindow( this, Qt::DockTornOff );
    parent->setAppropriate( this, false );
    setMaximumSize( 400, 300 );
}

// -----------------------------------------------------------------------------
// Name: OrderBrowser destructor
// Created: ZEBRE 2007-05-11
// -----------------------------------------------------------------------------
OrderBrowser::~OrderBrowser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderBrowser::Save
// Created: ZEBRE 2007-05-11
// -----------------------------------------------------------------------------
void OrderBrowser::Save()
{
    setCaption( tr( "Save order file" ) );
    RefreshFileList();
    saving_ = true;
    fileLabel_->clearEdit();
    ok_->setDisabled( true );
    show();
}
    
// -----------------------------------------------------------------------------
// Name: OrderBrowser::Load
// Created: ZEBRE 2007-05-11
// -----------------------------------------------------------------------------
void OrderBrowser::Load()
{
    setCaption( tr( "Load order file" ) );
    RefreshFileList();
    saving_ = false;
    fileLabel_->clearEdit();
    ok_->setDisabled( false );
    show();
}

// -----------------------------------------------------------------------------
// Name: OrderBrowser::OnOk
// Created: ZEBRE 2007-05-11
// -----------------------------------------------------------------------------
void OrderBrowser::OnOk()
{
    EmitAndClose( Translate( fileLabel_->currentText() ) );
}

// -----------------------------------------------------------------------------
// Name: OrderBrowser::OnCancel
// Created: ZEBRE 2007-05-11
// -----------------------------------------------------------------------------
void OrderBrowser::OnCancel()
{
    EmitAndClose( QString::null );
}

// -----------------------------------------------------------------------------
// Name: OrderBrowser::OnSelectionChanged
// Created: ZEBRE 2007-05-11
// -----------------------------------------------------------------------------
void OrderBrowser::OnSelectionChanged( QListViewItem* item )
{
    fileLabel_->setEditText( item ? item->text( 0 ) : "" );
}

// -----------------------------------------------------------------------------
// Name: OrderBrowser::OnDoubleClicked
// Created: ZEBRE 2007-05-11
// -----------------------------------------------------------------------------
void OrderBrowser::OnDoubleClicked( QListViewItem* item )
{
    OnSelectionChanged( item );
    OnOk();
}

namespace
{
    bool IsValidOrder( const bfs::path& order )
    {
        return ! bfs::is_directory( order )
              && QString( order.native_file_string().c_str() ).endsWith( ".ord" );
    }

    bool IsValidNewOrder( const QString& order )
    {
        try
        {
            bfs::path orderPath( order.ascii(), bfs::native );
            return orderPath.has_leaf() && orderPath.leaf() == orderPath.string();
        }
        catch( ... )
        {
            return false;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: OrderBrowser::OnTextChanged
// Created: ZEBRE 2007-05-11
// -----------------------------------------------------------------------------
void OrderBrowser::OnTextChanged( const QString& text )
{
    if( saving_ )
        ok_->setDisabled( !IsValidNewOrder( text ) );
}

// -----------------------------------------------------------------------------
// Name: OrderBrowser::RefreshFileList
// Created: ZEBRE 2007-05-11
// -----------------------------------------------------------------------------
void OrderBrowser::RefreshFileList()
{
    fileLabel_->clear();
    files_->clear();
    bfs::path directory( directory_.ascii(), bfs::native );

    bfs::directory_iterator end;
    for( bfs::directory_iterator it( directory ); it != end; ++it )
    {
        const bfs::path child = *it;
        if( IsValidOrder( child ) )
        {
            std::string orderName = child.leaf();
            orderName = orderName.substr( 0, orderName.size() - 4 );
            QListViewItem* item = new QListViewItem( files_, orderName.c_str() );
            item->setPixmap( 0, pixmap_ );
            fileLabel_->insertItem( orderName.c_str() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: OrderBrowser::Translate
// Created: ZEBRE 2007-05-11
// -----------------------------------------------------------------------------
QString OrderBrowser::Translate( const QString& base )
{
    if( base.isEmpty() )
        return base;
    const bfs::path filePath = bfs::path( directory_.ascii(),        bfs::native )
                             / bfs::path( ( base + ".ord" ).ascii(), bfs::native );
    return filePath.native_file_string().c_str();
}

// -----------------------------------------------------------------------------
// Name: OrderBrowser::EmitAndClose
// Created: ZEBRE 2007-05-11
// -----------------------------------------------------------------------------
void OrderBrowser::EmitAndClose( const QString& file )
{
    if( saving_ )
        emit OrderSaved ( file );
    else
        emit OrderOpened( file );
    hide();
}
