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
{
    bfs::create_directories( bfs::path( directory.ascii(), bfs::native ) );
    QVBox* box = new QVBox( this );
    box->setFrameStyle( QVBox::Panel + QVBox::Sunken );

    files_ = new QListView( box );
    files_->setMinimumHeight( 240 );
    files_->addColumn( tr( "Orders" ) );
    connect( files_, SIGNAL( doubleClicked   ( QListViewItem* , const QPoint&, int ) ), SLOT( OnDoubleClicked   ( QListViewItem* ) ) );
    connect( files_, SIGNAL( selectionChanged( QListViewItem* ) ),                      SLOT( OnSelectionChanged( QListViewItem* ) ) );

    QHBox* fileBox = new QHBox( box );
    fileLabel_ = new QTextEdit( fileBox ); fileBox->setStretchFactor( fileLabel_, 42 );
    connect( fileLabel_, SIGNAL( textChanged() ), SLOT( OnTextChanged() ) );
    QVBox* buttons = new QVBox( fileBox ); fileBox->setStretchFactor( buttons,     1 );
    
    ok_ = new QPushButton( tr( "Ok" ), buttons );
    connect( ok_, SIGNAL( clicked() ), SLOT( OnOk() ) );

    QPushButton* cancel = new QPushButton( tr( "Cancel" ), buttons );
    connect( cancel, SIGNAL( clicked() ), SLOT( OnCancel() ) );
    fileBox->setMaximumHeight( 2 * ok_->height() + 5 );

    setWidget( box );
    setResizeEnabled( true );
    setCloseMode( QDockWindow::Never );
    setCaption( tr( "Order files" ) );
    parent->moveDockWindow( this, Qt::DockTornOff );
    parent->setAppropriate( this, false );
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
    fileLabel_->clear();
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
    fileLabel_->clear();
    ok_->setDisabled( false );
    show();
}

// -----------------------------------------------------------------------------
// Name: OrderBrowser::OnOk
// Created: ZEBRE 2007-05-11
// -----------------------------------------------------------------------------
void OrderBrowser::OnOk()
{
    EmitAndClose( Translate( fileLabel_->text() ) );
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
    fileLabel_->setText( item ? item->text( 0 ) : "" );
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
void OrderBrowser::OnTextChanged()
{
    if( saving_ )
        ok_->setDisabled( !IsValidNewOrder( fileLabel_->text() ) );
}

// -----------------------------------------------------------------------------
// Name: OrderBrowser::RefreshFileList
// Created: ZEBRE 2007-05-11
// -----------------------------------------------------------------------------
void OrderBrowser::RefreshFileList()
{
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
            new QListViewItem( files_, orderName.c_str() );
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
