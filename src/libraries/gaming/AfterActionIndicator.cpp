// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AfterActionIndicator.h"
#include "clients_kernel/Controller.h"

// -----------------------------------------------------------------------------
// Name: AfterActionIndicator constructor
// Created: JCR 2008-11-04
// -----------------------------------------------------------------------------
AfterActionIndicator::AfterActionIndicator( const std::string& name, kernel::Controller& controller )
    : name_ ( name.c_str() )
    , controller_ ( controller )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionIndicator destructor
// Created: JCR 2008-11-04
// -----------------------------------------------------------------------------
AfterActionIndicator::~AfterActionIndicator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterActionIndicator::GetName
// Created: JCR 2008-11-04
// -----------------------------------------------------------------------------
const QString& AfterActionIndicator::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: AfterActionIndicator::GetValue
// Created: JCR 2008-11-04
// -----------------------------------------------------------------------------
double AfterActionIndicator::GetValue() const
{
    return value_;
}

// -----------------------------------------------------------------------------
// Name: AfterActionIndicator::Update
// Created: JCR 2008-11-04
// -----------------------------------------------------------------------------
void AfterActionIndicator::Update( const ASN1T_MsgIndicator& asn )
{
    value_ = asn.value;
	controller_.Update( *this );
}
