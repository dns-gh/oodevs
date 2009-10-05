// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_DotationTypeParameter.h"
#include "simulation_orders/MIL_ParameterType_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"

// -----------------------------------------------------------------------------
// Name: MIL_DotationTypeParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_DotationTypeParameter::MIL_DotationTypeParameter( const ASN1T_DotationType & asn )
    : pCategory_( PHY_DotationType::FindDotationCategory( asn ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationTypeParameter constructor
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
MIL_DotationTypeParameter::MIL_DotationTypeParameter( const PHY_DotationCategory* pCategory )
: pCategory_( pCategory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationTypeParameter destructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_DotationTypeParameter::~MIL_DotationTypeParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationTypeParameter::IsOfType
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_DotationTypeParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_DotationType*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationTypeParameter::ToDotationType
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_DotationTypeParameter::ToDotationType( ASN1T_DotationType& asn ) const
{
    if( !pCategory_ )
        return false;
    asn = pCategory_->GetMosID();
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationTypeParameter::ToDotationType
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_DotationTypeParameter::ToDotationType( const PHY_DotationCategory*& value ) const
{
    value = pCategory_;
    return true;
}
