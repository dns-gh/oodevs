// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_MultiRefWarningDialog.h"
#include "moc_ADN_MultiRefWarningDialog.cpp"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_MultiRefWarningDialog constructor
// Created: ABR 2012-07-24
// -----------------------------------------------------------------------------
ADN_MultiRefWarningDialog::ADN_MultiRefWarningDialog( QWidget* parent, ADN_Workspace::T_UsingElements& usingElements )
    : QDialog( parent )
{
    assert( !usingElements.empty() );
    setCaption( tr( "Multi references" ) );
    setMinimumSize( 300, 300 );

    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->setSpacing( 10 );
    mainLayout->setMargin( 5 );
    QLabel* label = new QLabel( tr( "This item is referenced by all the following item:" ) );
    label->setFont( QFont( "Arial", 10 ) );
    mainLayout->addWidget( label, 0, Qt::AlignCenter );

    QTableView* tableView = new QTableView();
    tableView->setSortingEnabled( true );
    tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
    tableView->setSelectionMode( QAbstractItemView::SingleSelection );
    tableView->setAlternatingRowColors( true );
    tableView->verticalHeader()->setVisible( false );
    mainLayout->addWidget( tableView, 1 );

    QStandardItemModel* dataModel = new QStandardItemModel( this );
    tableView->setModel( dataModel );
    dataModel->setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Tab" ) << tr( "Item" );
    dataModel->setHorizontalHeaderLabels( horizontalHeaders );
    tableView->horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
    tableView->horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );

    int row = 0;
    for( ADN_Workspace::CIT_UsingElements it = usingElements.begin(); it != usingElements.end(); ++it )
    {
        for( QStringList::const_iterator textIt = it->second.constBegin(); textIt != it->second.constEnd(); ++textIt, ++row )
        {
            dataModel->setItem( row, 0, new QStandardItem( ADN_Tr::ConvertFromWorkspaceElement( it->first ).c_str() ) );
            dataModel->setItem( row, 1, new QStandardItem( *textIt ) );
        }
    }

    label = new QLabel( tr( "Click \"Ok\" to destroy it and all its references." ) );
    label->setFont( QFont( "Arial", 10 ) );
    mainLayout->addWidget( label, 0, Qt::AlignCenter );

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QDialogButtonBox* okButton = new QDialogButtonBox( QDialogButtonBox::Ok );
    connect( okButton, SIGNAL( accepted() ), this, SLOT( OnAccept() ) );
    okButton->setFocus();
    QDialogButtonBox* cancelButton = new QDialogButtonBox( QDialogButtonBox::Cancel );
    connect( cancelButton, SIGNAL( rejected() ), this, SLOT( OnReject() ) );
    buttonLayout->addWidget( okButton, 0, Qt::AlignCenter );
    buttonLayout->addWidget( cancelButton, 0, Qt::AlignCenter );
    mainLayout->addLayout( buttonLayout );
}

// -----------------------------------------------------------------------------
// Name: ADN_MultiRefWarningDialog destructor
// Created: ABR 2012-07-24
// -----------------------------------------------------------------------------
ADN_MultiRefWarningDialog::~ADN_MultiRefWarningDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_MultiRefWarningDialog::OnAccept
// Created: ABR 2012-07-24
// -----------------------------------------------------------------------------
void ADN_MultiRefWarningDialog::OnAccept()
{
    done( QMessageBox::Ok );
}

// -----------------------------------------------------------------------------
// Name: ADN_MultiRefWarningDialog::OnReject
// Created: ABR 2012-07-24
// -----------------------------------------------------------------------------
void ADN_MultiRefWarningDialog::OnReject()
{
    done( QMessageBox::Cancel );
}
