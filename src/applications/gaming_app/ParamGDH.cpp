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
ParamGDH::ParamGDH( QObject* parent, ASN1T_GDH*& asn, const QString& name )
    : QObject( parent )
    , Param_ABC( name )
    , asn_( new ASN1T_GDH() )
    , pDateTimeEdit_( 0 )
{
    asn = asn_;
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
// Name: ParamGDH::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamGDH::BuildInterface( QWidget* parent )
{
    QHBox* box = new QHBox( parent );
    box->setSpacing( 5 );
    box->setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );
    QLabel* label = new QLabel( GetName(), box );
    pDateTimeEdit_ = new QDateTimeEdit( QDateTime::currentDateTime(), box );
    pCheckbox_ = new QCheckBox( box );
    pDateTimeEdit_->setEnabled( false );
    pCheckbox_->setChecked( false );
    box->setStretchFactor( label, 2 );
    box->setStretchFactor( pDateTimeEdit_, 0 );
    box->setStretchFactor( pCheckbox_, 0 );
    connect( pCheckbox_, SIGNAL( toggled( bool ) ), SLOT( OnCheckboxToogled( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamGDH::Commit
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void ParamGDH::Commit()
{
    if( !pDateTimeEdit_ )
        throw std::runtime_error( "'GDH' parameter interface not initialized" );
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
