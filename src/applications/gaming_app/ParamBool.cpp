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

// -----------------------------------------------------------------------------
// Name: ParamBool constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamBool::ParamBool( ASN1BOOL& asn, const QString& name, bool defaultValue /*= false*/ )
    : Param_ABC( name )
    , asn_( asn )
    , default_( defaultValue )
    , checkBox_( 0 )
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
    checkBox_ = new QCheckBox( GetName(), parent );
    checkBox_->setChecked( default_ );
}

// -----------------------------------------------------------------------------
// Name: ParamBool::Commit
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void ParamBool::Commit()
{
    if( !checkBox_ )
        InterfaceNotInitialized();
    asn_ = checkBox_->isChecked();
}
