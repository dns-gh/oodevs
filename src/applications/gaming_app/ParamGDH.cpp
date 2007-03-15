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
#include "game_asn/Asn.h"

// -----------------------------------------------------------------------------
// Name: ParamGDH constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamGDH::ParamGDH( QObject* parent, const QString& name )
    : QObject( parent )
    , Param_ABC( name )
    , pDateTimeEdit_( 0 )
{
    // NOTHING
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
// Name: ParamGDH::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamGDH::BuildInterface( QWidget* parent )
{
    new QLabel( GetName(), parent );
    QHBox* box = new QHBox( parent );
    pDateTimeEdit_ = new QDateTimeEdit( QDateTime::currentDateTime(), box );
    pCheckbox_ = new QCheckBox( box );
    pDateTimeEdit_->setEnabled( false );
    pCheckbox_->setChecked( false );
    connect( pCheckbox_, SIGNAL( toggled( bool ) ), SLOT( OnCheckboxToogled( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamGDH::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamGDH::CommitTo( ASN1T_MissionParameter& asn ) const
{
    if( !pDateTimeEdit_ )
        InterfaceNotInitialized();
    asn.value.t = T_MissionParameter_value_gDH;
    ASN1T_GDH*& gdh = asn.value.u.gDH = new ASN1T_GDH();
    gdh->qualificatif = EnumGDH_Qualificatif::at;
    gdh->datation = 0;
    asn.null_value = pCheckbox_->isChecked() ? 0 : 1;
    if( pCheckbox_->isChecked() )
    {
        static QDateTime baseDateTime( QDate( 1901, 1, 1 ) );
        gdh->datation = baseDateTime.secsTo( pDateTimeEdit_->dateTime() );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamGDH::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamGDH::Clean( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.gDH;
}

// -----------------------------------------------------------------------------
// Name: ParamGDH::OnCheckboxToogled
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void ParamGDH::OnCheckboxToogled( bool b )
{
    pDateTimeEdit_->setEnabled( b );
}
