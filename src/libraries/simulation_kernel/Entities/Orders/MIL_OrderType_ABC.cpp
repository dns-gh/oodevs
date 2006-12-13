// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_OrderType_ABC.h"

#include "MIL_OrderTypeParameter.h"
#include "Decision/DEC_Tools.h"
#include "Network/NET_AsnException.h"

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_OrderType_ABC::MIL_OrderType_ABC( uint nID, MIL_InputArchive& archive )
    : nID_       ( nID )
    , strName_   ()
    , pDIAType_  ( 0 )
    , parameters_()
{
    archive.ReadAttribute( "name", strName_ );

    std::string strDIAType;
    archive.ReadAttribute( "dia-type", strDIAType );
    pDIAType_ = &DEC_Tools::GetDIAType( strDIAType );
    while( archive.NextListElement() )
    {
        archive.Section( "parameter" );
        parameters_.push_back( new MIL_OrderTypeParameter( *this, archive ) );
        archive.EndSection(); // parameter
    }
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_OrderType_ABC::~MIL_OrderType_ABC()
{
}

// =============================================================================
// COPY
// =============================================================================
    
//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_OrderType_ABC::Copy( const ASN1T_MissionParameters& from, DIA_TypedObject& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver ) const
{
    if( from.n != parameters_.size() || to.GetType() != *pDIAType_ )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    uint i = 0;
    for( CIT_MissionParameterVector it = parameters_.begin(); it != parameters_.end(); ++it, ++i )
        (**it).Copy( from.elem[i], to, knowledgeResolver );
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_OrderType_ABC::Copy( const DIA_TypedObject& from, DIA_TypedObject& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver ) const
{
    if( to.GetNumberOfFields() != from.GetNumberOfFields() || to.GetNumberOfFields() < (int)parameters_.size() )
        return false;
    if( from.GetType() != *pDIAType_  || to.GetType() != *pDIAType_ )
        return false;

    for( CIT_MissionParameterVector it = parameters_.begin(); it != parameters_.end(); ++it )
        if( !(**it).Copy( from, to, knowledgeResolver ) )
            return false;
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_OrderType_ABC::Copy( const DIA_TypedObject& from, ASN1T_MissionParameters& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver ) const
{
    if( from.GetNumberOfFields() < (int)parameters_.size() || from.GetType() != *pDIAType_ )
        return false;

    to.n    = parameters_.size();
    to.elem = new ASN1T_MissionParameter[ parameters_.size() ];
    uint i = 0;
    for( CIT_MissionParameterVector it = parameters_.begin(); it != parameters_.end(); ++it, ++i )
        if( !(**it).Copy( from, to.elem[i], knowledgeResolver ) )
        {   
            assert( false );
            delete [] to.elem;
            to.n = 0;
            return false;
        }
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_OrderType_ABC::CleanAfterSerialization( ASN1T_MissionParameters& to ) const
{
    if( parameters_.size() != to.n )
        return;

    uint i = 0;
    for( CIT_MissionParameterVector it = parameters_.begin(); it != parameters_.end(); ++it, ++i )
        (**it).CleanAfterSerialization( to.elem[i] );

    if( to.n > 0 )
        delete [] to.elem;
}

