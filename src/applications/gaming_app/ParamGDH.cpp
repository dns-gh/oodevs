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
ParamGDH::ParamGDH( QWidget* parent, ASN1T_GDH& asn, const QString& label )
    : QHBox( parent )
    , asn_( asn )
{
    setSpacing( 5 );
    setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );

    // Title
    QLabel* pLabel = new QLabel( label, this, "" );
    pDateTimeEdit_ = new QDateTimeEdit( QDateTime::currentDateTime(), this );
    pCheckbox_ = new QCheckBox( this );

    pDateTimeEdit_->setEnabled( false );
    pCheckbox_->setChecked( false );

    setStretchFactor( pLabel, 2 );
    setStretchFactor( pDateTimeEdit_, 0 );
    setStretchFactor( pCheckbox_, 0 );

    connect( pCheckbox_, SIGNAL( toggled( bool ) ), SLOT( OnCheckboxToogled( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamGDH destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamGDH::~ParamGDH()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamGDH::Commit
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void ParamGDH::Commit()
{
    asn_.qualificatif = EnumGDH_Qualificatif::at;
    if( pCheckbox_->isChecked() )
    {
        static QDateTime baseDateTime( QDate( 1901, 1, 1 ) );
        asn_.datation = baseDateTime.secsTo( pDateTimeEdit_->dateTime() );
    }
    else
        asn_.datation = 0;
}

// -----------------------------------------------------------------------------
// Name: ParamGDH::OnCheckboxToogled
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void ParamGDH::OnCheckboxToogled( bool b )
{
    pDateTimeEdit_->setEnabled( b );
}
