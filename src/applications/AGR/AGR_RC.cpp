// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-07 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "AGR_pch.h"
#include "AGR_RC.h"

#include "AGR_Workspace.h"
#include "AGR_TypeRC.h"

// -----------------------------------------------------------------------------
// Name: AGR_RC constructor
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
AGR_RC::AGR_RC( MT_XXmlInputArchive& input, const AGR_Workspace& workspace )
    : strAsnName_( )
    , pType_     ( 0 )
{
    input.Section( "xsd:element" );

    input.ReadAttribute( "name", strAsnName_ );
    
    std::string strType;
    input.ReadAttribute( "type", strType );
    pType_ = workspace.FindTypeRC( strType );
    if( !pType_ )
        throw std::exception( ( "Unknown RC type " + strType ).c_str() );

    input.EndSection(); // xsd:element
}

// =============================================================================
// GENERATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: AGR_RC::GenerateMILRCClassHeader
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_RC::GenerateMILRCClassHeader() const
{
    assert( pType_ );
    return pType_->GenerateMILRCClassHeader( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_RC::GenerateMILRCClassBodyInitialize
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_RC::GenerateMILRCClassBodyInitialize() const
{
    assert( pType_ );
    return pType_->GenerateMILRCClassBodyInitialize( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_RC::GenerateMILRCClassBodyIncludes
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_RC::GenerateMILRCClassBodyIncludes() const
{
    assert( pType_ );
    return pType_->GenerateMILRCClassBodyIncludes( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_RC::GenerateHALEnum
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_RC::GenerateHALEnum() const
{
    assert( pType_ );
    return pType_->GenerateHALEnum( *this );
}


// -----------------------------------------------------------------------------
// Name: AGR_RC::GenerateMOS2
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_RC::GenerateMOS2() const
{
    assert( pType_ );
    return pType_->GenerateMOS2( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_RC::GenerateTICRCClassBodyInitialize
// Created: SBO 2005-09-14
// -----------------------------------------------------------------------------
std::string AGR_RC::GenerateTICRCClassBodyInitialize() const
{
    assert( pType_ );
    return pType_->GenerateTIC( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_RC::GenerateXML
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
std::string AGR_RC::GenerateXML( uint nIdx ) const
{
    assert( pType_ );
    return pType_->GenerateXML( *this, nIdx );
}
