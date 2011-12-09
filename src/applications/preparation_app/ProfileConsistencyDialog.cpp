// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ProfileConsistencyDialog.h"
#include "moc_ProfileConsistencyDialog.cpp"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Tools.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: ProfileConsistencyDialog constructor
// Created: LGY 2011-12-09
// -----------------------------------------------------------------------------
ProfileConsistencyDialog::ProfileConsistencyDialog( QWidget* parent )
    : QDialog( parent )
{
    setCaption( tools::translate( "ProfileConsistencyDialog", "Consistency analysis" ) );
    setMinimumSize( 500, 500 );

    QVBoxLayout* mainLayout = new QVBoxLayout( this );

    dataModel_ = new QStandardItemModel();
    dataModel_->setColumnCount( 1 );

    tableView_ = new QTableView();
    tableView_->setModel( dataModel_ );
    tableView_->setSortingEnabled( true );
    tableView_->setSelectionMode( QAbstractItemView::NoSelection );
    tableView_->setAlternatingRowColors( true );
    tableView_->horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
    tableView_->verticalHeader()->setVisible( false );
    tableView_->horizontalHeader()->setVisible( false );

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QDialogButtonBox* okButton = new QDialogButtonBox( QDialogButtonBox::Ok );
    connect( okButton, SIGNAL( accepted() ), this, SLOT( accept() ) );
    okButton->setFocus();
    QDialogButtonBox* cancelButton = new QDialogButtonBox( QDialogButtonBox::Cancel );
    connect( cancelButton, SIGNAL( rejected() ), this, SLOT( reject() ) );
    buttonLayout->addWidget( okButton, 0, Qt::AlignHCenter );
    buttonLayout->addWidget( cancelButton, 0, Qt::AlignHCenter );

    mainLayout->addWidget( new QLabel( "<font color=\"#FF0000\">" + tools::translate( "ProfileConsistencyDialog", "Do you want to proceed ?" ) + "</font>" ), 0, Qt::AlignHCenter );
    mainLayout->addWidget( tableView_ );
    mainLayout->addLayout( buttonLayout );
}

// -----------------------------------------------------------------------------
// Name: ProfileConsistencyDialog destructor
// Created: LGY 2011-12-09
// -----------------------------------------------------------------------------
ProfileConsistencyDialog::~ProfileConsistencyDialog()
{
    // NOTHING
}

namespace
{
    QStandardItem* CreateItem( const QString& text )
    {
        QStandardItem* item = new QStandardItem( text );
        item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        return item;
    }
}

// -----------------------------------------------------------------------------
// Name: ProfileConsistencyDialog::Exec
// Created: LGY 2011-12-09
// -----------------------------------------------------------------------------
int ProfileConsistencyDialog::Exec( const T_Errors& errors )
{
    BOOST_FOREACH( const T_Error& error, errors )
    {
        dataModel_->setItem( dataModel_->rowCount(), 0, CreateItem( tools::translate( "ProfileConsistencyDialog", "Unit '%1' is already controlled by '%2' profile.\n" )
                            .arg( error.second->GetName().ascii() ).arg( error.first.c_str() ) ) );
    }
    return exec();
}
