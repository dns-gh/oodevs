// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-17 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/PawnType.inl $
// $Author: Sbo $
// $Modtime: 13/07/05 15:06 $
// $Revision: 3 $
// $Workfile: PawnType.inl $
//
// *****************************************************************************

namespace TEST
{

// -----------------------------------------------------------------------------
// Name: PawnType::Find
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const PawnType* PawnType::Find( const std::string& strName )
{
    CIT_PawnTypeMap it = pawnTypes_.find( strName );
    return it == pawnTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PawnType::Find
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const PawnType* PawnType::Find( ASN1T_TypePion asnId )
{
    for( CIT_PawnTypeMap it = pawnTypes_.begin(); it != pawnTypes_.end(); ++it )
        if( ( *it ).second->GetId() == asnId )
            return ( *it ).second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PawnType::GetName
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const std::string& PawnType::GetName() const
{
    return strName_;
}
          
// -----------------------------------------------------------------------------
// Name: PawnType::GetId
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
uint PawnType::GetId() const
{
    return nId_;
}

} // end namespace TEST
