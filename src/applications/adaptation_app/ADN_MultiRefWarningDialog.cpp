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
ADN_MultiRefWarningDialog::ADN_MultiRefWarningDialog( QWidget* parent, ADN_Workspace::T_UsingElements& elementsToDelete, ADN_Workspace::T_UsingElements& usingElements )
    : QDialog( parent )
{
    setCaption( tr( "Multi references" ) );
    setMinimumSize( 500, 500 );

    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->setSpacing( 10 );
    mainLayout->setMargin( 5 );

    CreateTable( *mainLayout, tr( "The following items will be <b>deleted</b>:" ), elementsToDelete );
    CreateTable( *mainLayout, tr( "The following items will be <b>modified</b>:" ), usingElements );

    QLabel* label = new QLabel( tr( "Click \"Ok\" to continue." ) );
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
// Name: ADN_MultiRefWarningDialog::CreateTable
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
void ADN_MultiRefWarningDialog::CreateTable( QVBoxLayout& layout, const QString& title, const ADN_Workspace::T_UsingElements& elements )
{
    if( !elements.empty() )
    {
        QLabel* label = new QLabel( title );
        label->setFont( QFont( "Arial", 10 ) );
        layout.addWidget( label, 0, Qt::AlignCenter );

        QTableView* tableView = new QTableView();
        tableView->setSortingEnabled( true );
        tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
        tableView->setSelectionMode( QAbstractItemView::SingleSelection );
        tableView->setAlternatingRowColors( true );
        tableView->verticalHeader()->setVisible( false );
        layout.addWidget( tableView, 1 );

        QStandardItemModel* dataModel = new QStandardItemModel( this );
        tableView->setModel( dataModel );
        dataModel->setColumnCount( 2 );
        QStringList horizontalHeaders;
        horizontalHeaders << tr( "Tab" ) << tr( "Item" );
        dataModel->setHorizontalHeaderLabels( horizontalHeaders );
        tableView->horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
        tableView->horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );

        FillModel( *dataModel, elements );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_MultiRefWarningDialog::CreateItem
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
QStandardItem* ADN_MultiRefWarningDialog::CreateItem( const QString& text )
{
    QStandardItem* item = new QStandardItem( text );
    item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    return item;
}

// -----------------------------------------------------------------------------
// Name: ADN_MultiRefWarningDialog::FillModel
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
void ADN_MultiRefWarningDialog::FillModel( QStandardItemModel& model, const ADN_Workspace::T_UsingElements& elements )
{
    int row = 0;
    for( auto it = elements.begin(); it != elements.end(); ++it )
    {
        if( it->second.isEmpty() )
        {
            QString tabName = ADN_Tr::ConvertFromWorkspaceElement( it->first ).c_str();
            model.setItem( row, 0, CreateItem( tabName ) );
            if( it->first == eUrban )
                model.setItem( row, 1, CreateItem( tr( "All 'urban material'" ) ) );
            else
                model.setItem( row, 1, CreateItem( tr( "All '%1'" ).arg( tabName ) ) );
            ++row;
        }
        else
            for( QStringList::const_iterator textIt = it->second.constBegin(); textIt != it->second.constEnd(); ++textIt, ++row )
            {
                model.setItem( row, 0, CreateItem( ADN_Tr::ConvertFromWorkspaceElement( it->first ).c_str() ) );
                model.setItem( row, 1, CreateItem( *textIt ) );
            }
    }
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
