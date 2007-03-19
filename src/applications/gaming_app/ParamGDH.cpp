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
#include "gaming/ActionParameter.h"
#include "gaming/Action_ABC.h"

// -----------------------------------------------------------------------------
// Name: ParamGDH constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamGDH::ParamGDH( QObject* parent, const QString& name )
    : QObject( parent )
    , Param_ABC( name )
    , datetime_( QDateTime::currentDateTime() )
    , valueSet_( false )
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
    QDateTimeEdit* dateTimeEdit = new QDateTimeEdit( datetime_, box );
    dateTimeEdit->setEnabled( valueSet_ );
    QCheckBox* checkbox = new QCheckBox( box );
    checkbox->setChecked( valueSet_ );
    connect( checkbox, SIGNAL( toggled( bool ) ), dateTimeEdit, SLOT( setEnabled( bool ) ) );
    connect( checkbox, SIGNAL( toggled( bool ) ), SLOT( OnEnabled( bool ) ) );
    connect( dateTimeEdit, SIGNAL( valueChanged( const QDateTime& ) ), SLOT( OnValueChanged( const QDateTime& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamGDH::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamGDH::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_gDH;
    ASN1T_GDH*& gdh = asn.value.u.gDH = new ASN1T_GDH();
    gdh->qualificatif = EnumGDH_Qualificatif::at;
    gdh->datation = 0;
    asn.null_value = valueSet_ ? 0 : 1;
    if( valueSet_ )
    {
        static QDateTime baseDateTime( QDate( 1901, 1, 1 ) );
        gdh->datation = baseDateTime.secsTo( datetime_ );
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
// Name: ParamGDH::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ParamGDH::CommitTo( Action_ABC& action ) const
{
    if( valueSet_ )
    {
        std::auto_ptr< ActionParameter< QDateTime > > param( new ActionParameter< QDateTime >( GetName() ) );
        param->SetValue( datetime_ );
        action.AddParameter( *param.release() );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamGDH::OnEnabled
// Created: SBO 2007-03-16
// -----------------------------------------------------------------------------
void ParamGDH::OnEnabled( bool enabled )
{
    valueSet_ = enabled;
}

// -----------------------------------------------------------------------------
// Name: ParamGDH::OnValueChanged
// Created: SBO 2007-03-16
// -----------------------------------------------------------------------------
void ParamGDH::OnValueChanged( const QDateTime& datetime )
{
    datetime_ = datetime;
}
