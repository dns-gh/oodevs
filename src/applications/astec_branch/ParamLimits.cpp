// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ParamLimits.h"
#include "Limit.h"

// -----------------------------------------------------------------------------
// Name: ParamLimits constructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
ParamLimits::ParamLimits( QWidget* pParent, ASN1T_OID& id1, ASN1T_OID& id2, const std::string& label1, const std::string& label2, const std::string& menu1, const std::string& menu2 )
    : limit1_( new EntityParameter< Limit >( pParent, id1, label1, menu1 ) )
    , limit2_( new EntityParameter< Limit >( pParent, id2, label2, menu2 ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLimits destructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
ParamLimits::~ParamLimits()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::CheckValidity
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
bool ParamLimits::CheckValidity()
{
    bool bDummy = limit1_->CheckValidity(), bDenis = limit2_->CheckValidity();
    return bDummy && bDenis;
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::Commit
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void ParamLimits::Commit()
{
    limit1_->Commit();
    limit2_->Commit();
}
