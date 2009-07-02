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
#include "MIL_OrderContext.h"
#include "Decision/DEC_Tools.h"
#include "Network/NET_AsnException.h"
#include <xeumeuleu/xml.h>

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_OrderType_ABC::MIL_OrderType_ABC( uint nID, xml::xistream& xis )
    : nID_       ( nID )
    , strName_   ( xml::attribute< std::string >( xis, "name" ) )
    , pDIAType_  ( &DEC_Tools::GetDIAType( xml::attribute< std::string >( xis, "dia-type" ) ) )
{
    xis >> xml::list( "parameter", *this, &MIL_OrderType_ABC::ReadParameter );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::ReadParameter
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void MIL_OrderType_ABC::ReadParameter( xml::xistream& xis )
{
    parameters_.push_back( new MIL_OrderTypeParameter( *this, xis ) );
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_OrderType_ABC::~MIL_OrderType_ABC()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_OrderType_ABC::Copy( const ASN1T_MissionParameters& from, DIA_TypedObject& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_OrderContext& context ) const
{
    unsigned int index = context.Length();
    if( from.n - index != parameters_.size() || to.GetType() != *pDIAType_ )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    for( CIT_MissionParameterVector it = parameters_.begin(); it != parameters_.end(); ++it, ++index )
        (**it).Copy( from.elem[index], to, knowledgeResolver );
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
bool MIL_OrderType_ABC::Copy( const DIA_TypedObject& from, ASN1T_MissionParameters& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_OrderContext& context ) const
{
    if( from.GetNumberOfFields() < (int)parameters_.size() || from.GetType() != *pDIAType_ )
        return false;

    unsigned int index = context.Length();
    to.n    = parameters_.size() + index;
    to.elem = new ASN1T_MissionParameter[ parameters_.size() + index ];
    for( CIT_MissionParameterVector it = parameters_.begin(); it != parameters_.end(); ++it, ++index )
        if( !(**it).Copy( from, to.elem[index], knowledgeResolver ) )
        {   
            assert( false );
            delete[] to.elem;
            to.n = 0;
            return false;
        }
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_OrderType_ABC::CleanAfterSerialization( ASN1T_MissionParameters& to, const MIL_OrderContext& context ) const
{
    unsigned int index = context.Length();
    if( index + parameters_.size() != to.n )
        return;

    for( CIT_MissionParameterVector it = parameters_.begin(); it != parameters_.end(); ++it, ++index )
        (**it).CleanAfterSerialization( to.elem[index] );

    if( to.n > 0 )
        delete [] to.elem;
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::GetDIAType
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
const DIA_TypeDef& MIL_OrderType_ABC::GetDIAType() const
{
    assert( pDIAType_ );
    return *pDIAType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::GetID
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
uint MIL_OrderType_ABC::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::GetName
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
const std::string& MIL_OrderType_ABC::GetName() const
{
    return strName_;
}
