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
#include "xeumeuleu/xml.h"

using namespace xml;

//-----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_OrderTypeParameter::MIL_OrderTypeParameter( const MIL_OrderType_ABC& orderType, xml::xistream& xis )
    : nDIAParameter_( 0 )
    , orderType_    ( orderType )
    , bIsOptional_  ( false )
    , strName_      ()
    , pParameter_   ( 0 )
{
    xis >> attribute( "name", strName_ );

    std::string strType;
    xis >> attribute( "type", strType );
    pParameter_ = MIL_ParameterType_ABC::Find( strType );
    if( !pParameter_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown parameter type" ); // $$$$ ABL 2007-07-25: error context

    xis >> optional() >> attribute( "optional", bIsOptional_ );

    std::string strDIAParameter;
    xis >> attribute( "dia-name", strDIAParameter );
    nDIAParameter_ = DEC_Tools::InitializeDIAField( strDIAParameter, orderType_.GetDIAType() );
    //$$$$ Checker type DIA si c possible
}
    
//-----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_OrderTypeParameter::~MIL_OrderTypeParameter()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

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
    if( from.GetNumberOfFields() <= nDIAParameter_ ||
        to  .GetNumberOfFields() <= nDIAParameter_ )
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
