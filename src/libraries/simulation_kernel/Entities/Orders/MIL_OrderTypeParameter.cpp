// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_OrderTypeParameter.h"
#include "MIL_ParameterType_ABC.h"
#include "MIL_OrderType_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Network/NET_AsnException.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter constructor
// Created: SBO 2008-03-03
// -----------------------------------------------------------------------------
MIL_OrderTypeParameter::MIL_OrderTypeParameter( xml::xistream& xis )
    : nDIAParameter_( 0 )
    , bIsOptional_  ( xml::attribute< bool >( xis, "optional", false ) )
    , strName_      ( xml::attribute< std::string >( xis, "name" ) )
    , pParameter_   ( MIL_ParameterType_ABC::Find( xml::attribute< std::string >( xis, "type" ) ) )
{
    if( !pParameter_ )
        xis.error( "Unknown parameter type" );
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_OrderTypeParameter::MIL_OrderTypeParameter( const MIL_OrderType_ABC& orderType, xml::xistream& xis )
    : nDIAParameter_( DEC_Tools::InitializeDIAField( xml::attribute< std::string >( xis, "dia-name" ), orderType.GetDIAType() ) )
    , bIsOptional_  ( xml::attribute< bool >( xis, "optional", false ) )
    , strName_      ( xml::attribute< std::string >( xis, "name" ) )
    , pParameter_   ( MIL_ParameterType_ABC::Find( xml::attribute< std::string >( xis, "type" ) ) )
{
    if( !pParameter_ )
        xis.error( "Unknown parameter type" );
    //$$$$ Checker type DIA si c possible
}
    
//-----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_OrderTypeParameter::~MIL_OrderTypeParameter()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_OrderTypeParameter::Copy( const ASN1T_MissionParameter& from, DIA_TypedObject& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver ) const
{
    assert( pParameter_ );
    if( to.GetNumberOfFields() <= nDIAParameter_ )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    pParameter_->Copy( from, to.GetVariable( nDIAParameter_ ), knowledgeResolver, bIsOptional_ );
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_OrderTypeParameter::Copy( const DIA_TypedObject& from, DIA_TypedObject& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver ) const
{
    assert( pParameter_ );
    if( from.GetNumberOfFields() <= nDIAParameter_ || to.GetNumberOfFields() <= nDIAParameter_ )
        return false;
    return pParameter_->Copy( const_cast< DIA_TypedObject& >( from ).GetVariable( nDIAParameter_ ), to.GetVariable( nDIAParameter_ ), knowledgeResolver, bIsOptional_ );
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_OrderTypeParameter::Copy( const DIA_TypedObject& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver ) const
{
    assert( pParameter_ );
    if( from.GetNumberOfFields() <= nDIAParameter_ )
        return false;
    return pParameter_->Copy( const_cast< DIA_TypedObject& >( from ).GetVariable( nDIAParameter_ ), to, knowledgeResolver, bIsOptional_ );
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_OrderTypeParameter::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( pParameter_ );
    pParameter_->CleanAfterSerialization( to );
}
