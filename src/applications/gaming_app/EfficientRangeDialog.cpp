// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EfficientRangeDialog.h"
#include "moc_EfficientRangeDialog.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/VolumeType.h"

// -----------------------------------------------------------------------------
// Name: EfficientRangeDialog constructor
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
EfficientRangeDialog::EfficientRangeDialog( QWidget* parent, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::VolumeType >& resolver, kernel::Options& options )
    : QDialog ( parent, 0, true, Qt::WStyle_Customize | Qt::WStyle_NormalBorder | Qt::WStyle_Title )
    , controllers_    ( controllers )
    , resolver_( resolver )
    , options_( options )
{
    setCaption( tr("Efficient Range") );

    Q3GridLayout* pMainLayout = new Q3GridLayout( this, 3, 2 );
    pMainLayout->setMargin( 10 );
    pMainLayout->setSpacing( 10 );

    Q3GroupBox* box = new Q3GroupBox( 2, Qt::Horizontal, tr( "Display Parameters" ), this );
    pMainLayout->addMultiCellWidget( box, 0, 1, 0, 2 );

    new QLabel( tr( "Percentage (PH):" ), box );
    phSpinbox_ = new QSpinBox( 0, 100, 1, box );

    new QLabel( tr( "Size:" ), box );
    volumeCombo_ = new QComboBox( box );

    QPushButton* pOKButton     = new QPushButton( tr( "Ok" )    , this );
    QPushButton* pCancelButton = new QPushButton( tr( "Cancel" ), this );
    pMainLayout->addWidget( pOKButton, 3, 0 );
    pMainLayout->addWidget( pCancelButton, 3, 1, Qt::AlignRight );
    pOKButton->setDefault( TRUE );

    connect( pCancelButton, SIGNAL( clicked() ), SLOT( reject() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EfficientRangeDialog destructor
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
EfficientRangeDialog::~EfficientRangeDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EfficientRangeDialog::NotifyUpdated
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
void EfficientRangeDialog::NotifyUpdated( const kernel::ModelLoaded& )
{
    volumeCombo_->clear();
    tools::Iterator< const kernel::VolumeType& > it = resolver_.CreateIterator();

    while( it.HasMoreElements() )
    {
        const kernel::VolumeType& volume = it.NextElement();
        volumeCombo_->insertItem( volume.GetName().c_str() );
    }

    phSpinbox_->setValue( options_.GetOption( "EfficientRangePh", 50 ).To< int >() );
    int comboValue = options_.GetOption( "EfficientRangeVolume", 0 ).To< int >();
    volumeCombo_->setCurrentItem( comboValue < volumeCombo_->count() ? comboValue : 0 );
}

// -----------------------------------------------------------------------------
// Name: EfficientRangeDialog::Validate
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
void EfficientRangeDialog::Validate()
{
    options_.Change( "EfficientRangePh", phSpinbox_->value() );
    options_.Change( "EfficientRangeVolume", volumeCombo_->currentItem() );
    accept();
}
