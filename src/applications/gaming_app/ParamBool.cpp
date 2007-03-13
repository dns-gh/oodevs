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
ParamBool::ParamBool( QWidget* parent, ASN1BOOL& asn, const QString& label, bool defaultValue /*= false*/ )
    : asn_( asn )
{
    checkBox_ = new QCheckBox( label, parent );
    checkBox_->setChecked( defaultValue );
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
// Name: ParamBool::Commit
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void ParamBool::Commit()
{
    asn_ = checkBox_->isChecked();
}
