// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunction::Find
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
inline
const MIL_LimaFunction* MIL_LimaFunction::Find( const ASN1T_EnumLimaType& asn )
{
    CIT_LimaFunctionMap it = limaFunctions_.find( asn );
    if( it == limaFunctions_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunction::Find
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
inline
const MIL_LimaFunction* MIL_LimaFunction::Find( uint nID )
{
    for( CIT_LimaFunctionMap it = limaFunctions_.begin(); it != limaFunctions_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunction::GetID
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
inline
uint MIL_LimaFunction::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunction::GetAsnID
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
inline
ASN1T_EnumLimaType MIL_LimaFunction::GetAsnID() const
{
    return nAsnID_;
}
