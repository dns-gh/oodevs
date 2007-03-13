// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamGDH.h"
#include "moc_ParamGDH.cpp"

// -----------------------------------------------------------------------------
// Name: ParamGDH constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamGDH::ParamGDH( QWidget* parent, ASN1T_GDH*& asn, const QString& label )
    : QObject( parent )
    , asn_( new ASN1T_GDH() )
{
    asn = asn_;

    QHBox* box = new QHBox( parent );
    box->setSpacing( 5 );
    box->setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );

    // Title
    QLabel* pLabel = new QLabel( label, box, "" );
    pDateTimeEdit_ = new QDateTimeEdit( QDateTime::currentDateTime(), box );
    pCheckbox_ = new QCheckBox( box );

    pDateTimeEdit_->setEnabled( false );
    pCheckbox_->setChecked( false );

    box->setStretchFactor( pLabel, 2 );
    box->setStretchFactor( pDateTimeEdit_, 0 );
    box->setStretchFactor( pCheckbox_, 0 );

    connect( pCheckbox_, SIGNAL( toggled( bool ) ), SLOT( OnCheckboxToogled( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamGDH destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamGDH::~ParamGDH()
{
    delete asn_;
}

// -----------------------------------------------------------------------------
// Name: ParamGDH::Commit
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void ParamGDH::Commit()
{
    asn_->qualificatif = EnumGDH_Qualificatif::at;
    if( pCheckbox_->isChecked() )
    {
        static QDateTime baseDateTime( QDate( 1901, 1, 1 ) );
        asn_->datation = baseDateTime.secsTo( pDateTimeEdit_->dateTime() );
    }
    else
        asn_->datation = 0;
}

// -----------------------------------------------------------------------------
// Name: ParamGDH::OnCheckboxToogled
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void ParamGDH::OnCheckboxToogled( bool b )
{
    pDateTimeEdit_->setEnabled( b );
}
