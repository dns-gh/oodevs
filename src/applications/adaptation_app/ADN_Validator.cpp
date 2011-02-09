// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Validator.h"

// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::ADN_PercentageValidator
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
ADN_PercentageValidator::ADN_PercentageValidator( QObject* pParent, const char* szName )
    : QDoubleValidator( 0, 100, 2, pParent, szName )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::~ADN_PercentageValidator
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
ADN_PercentageValidator::~ADN_PercentageValidator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::AddLinkedValue
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
void ADN_PercentageValidator::AddLinkedValue( ADN_Type_Double& value )
{
    linkedValues_.push_back( &value );
}

// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::fixup
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
void ADN_PercentageValidator::fixup( QString& strInput ) const
{
    double rSum = 0;
    for( CIT_ValuesVector it = linkedValues_.begin(); it != linkedValues_.end(); ++it )
        rSum += ( *it )->GetData();
    if( rSum >= 100.0 )
    {
        strInput = QString::number( 0.f );
        return;
    }
    bool bOk = true;
    double rValue = strInput.toDouble( &bOk );
    if( ! bOk )
        return;
    if( rSum + rValue > 100.f )
        strInput = QString::number( 100.f - rSum );
}
