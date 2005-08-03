// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/CommunicationType.inl $
// $Author: Sbo $
// $Modtime: 20/07/05 18:05 $
// $Revision: 3 $
// $Workfile: CommunicationType.inl $
//
// *****************************************************************************

namespace TIC
{

//-----------------------------------------------------------------------------
// Name: CommunicationType::Find
// Created: SBO 2005-07-12
//-----------------------------------------------------------------------------
inline
const CommunicationType& CommunicationType::Find( CommunicationType::EType eType, CommunicationType::ESubType eSubType )
{
    const CommunicationType  type( eType, eSubType );
    CIT_CommunicationTypeSet it = comTypes_.find( &type );
    if( it != comTypes_.end() )
        return **it;
    return notDefined;
}

//-----------------------------------------------------------------------------
// Name: CommunicationType::GetType
// Created: SBO 2005-07-12
//-----------------------------------------------------------------------------
inline
const std::string& CommunicationType::GetType() const
{
    CIT_TypeMap it = types_.find( eType_ );
    if( it != types_.end() )
        return it->second;
    return types_.find( eNoType )->second;
}

//-----------------------------------------------------------------------------
// Name: CommunicationType::GetSubType
// Created: SBO 2005-07-12
//-----------------------------------------------------------------------------
inline
const std::string& CommunicationType::GetSubType() const
{
    return subTypes_.find( eSubType_ )->second;
}

//-----------------------------------------------------------------------------
// Name: CommunicationType::IsReport
// Created: SBO 2005-07-12
//-----------------------------------------------------------------------------
inline
bool CommunicationType::IsReport() const
{
    return eType_ == eReport;
}

//-----------------------------------------------------------------------------
// Name: CommunicationType::operator==
// Created: SBO 2005-07-12
//-----------------------------------------------------------------------------
inline
bool CommunicationType::operator==( const CommunicationType& type ) const
{
    return eType_ == type.eType_ && eSubType_ == type.eSubType_;
}

} // end namespace TIC