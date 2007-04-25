// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamBool.h"
#include "moc_ParamBool.cpp"
#include "game_asn/Asn.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameter.h"
#include "clients_kernel/OrderParameter.h"

// -----------------------------------------------------------------------------
// Name: ParamBool constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamBool::ParamBool( QObject* parent, const kernel::OrderParameter& parameter, bool defaultValue /*= false*/ )
    : QObject( parent )
    , Param_ABC( parameter.GetName() )
    , parameter_( parameter )
    , value_( defaultValue )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamBool destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamBool::~ParamBool()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamBool::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamBool::BuildInterface( QWidget* parent )
{
    QCheckBox* checkBox = new QCheckBox( GetName(), parent );
    checkBox->setChecked( value_ );
    connect( checkBox, SIGNAL( clicked() ), SLOT( OnClicked() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamBool::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamBool::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = 0;
    asn.value.t = T_MissionParameter_value_aBool;
    asn.value.u.aBool = value_;
}

// -----------------------------------------------------------------------------
// Name: ParamBool::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ParamBool::CommitTo( Action_ABC& action ) const
{
    std::auto_ptr< ActionParameter< bool > > param( new ActionParameter< bool >( parameter_ ) );
    param->SetValue( value_ );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamBool::OnClicked
// Created: SBO 2007-03-16
// -----------------------------------------------------------------------------
void ParamBool::OnClicked()
{
    value_ = !value_;
}
