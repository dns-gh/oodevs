// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamDirection.h"
#include "moc_ParamDirection.cpp"
#include "gaming/ActionParameter.h"
#include "gaming/Action_ABC.h"

// -----------------------------------------------------------------------------
// Name: ParamDirection constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamDirection::ParamDirection( QObject* parent, const QString& name )
    : QObject( parent )
    , Param_ABC( name )
    , value_( 180 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamDirection destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamDirection::~ParamDirection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamDirection::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamDirection::BuildInterface( QWidget* parent )
{
    QHBox* box = new QHBox( parent ); // $$$$ SBO 2007-03-16: should be removed... but need some changes in order context interface
    new QLabel( GetName(), box );
    QDial* dial = new QDial( 0, 359, 1, 0, box );
    dial->setWrapping( true );
    dial->setMaximumSize( 50, 50 );
    dial->setValue( value_ );
    connect( dial, SIGNAL( valueChanged( int ) ), SLOT( OnValueChanged( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamDirection::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamDirection::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = 0;
    asn.value.t = T_MissionParameter_value_direction;
    asn.value.u.direction = value_;
}

// -----------------------------------------------------------------------------
// Name: ParamDirection::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamDirection::CommitTo( ASN1T_OrderContext& asn ) const
{
    asn.direction_dangereuse = value_;
}

// -----------------------------------------------------------------------------
// Name: ParamDirection::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ParamDirection::CommitTo( Action_ABC& action ) const
{
    std::auto_ptr< ActionParameter< float > > param( new ActionParameter< float >( GetName() ) );
    param->SetValue( value_ );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamDirection::OnValueChanged
// Created: SBO 2007-03-16
// -----------------------------------------------------------------------------
void ParamDirection::OnValueChanged( int value )
{
    value_ = value + ( value > 180 ? -180 : 180 );
}
