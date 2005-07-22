// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureCapacity.inl $
// $Author: Sbo $
// $Modtime: 13/07/05 15:02 $
// $Revision: 2 $
// $Workfile: NatureCapacity.inl $
//
// *****************************************************************************

namespace TIC
{

// -----------------------------------------------------------------------------
// Name: NatureCapacity::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const NatureCapacity* NatureCapacity::Find( const std::string& strName )
{
    CIT_NatureCapacityMap it = natureCapacity_.find( strName );
    return it == natureCapacity_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: NatureCapacity::Find
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const std::string* NatureCapacity::Find( uint asnId )
{
    for( CIT_NatureCapacityMap it = natureCapacity_.begin(); it != natureCapacity_.end(); ++it )
        if( ( *it ).second->GetAsnID() == asnId )
            return &( *it ).second->GetName();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: NatureCapacity::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const std::string& NatureCapacity::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: NatureCapacity::GetKey
// Created: SBO 2005-06-03
// -----------------------------------------------------------------------------
inline
const std::string& NatureCapacity::GetKey() const
{
    return strKey_;
}

// -----------------------------------------------------------------------------
// Name: NatureCapacity::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint NatureCapacity::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: NatureCapacity::GetAsnID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint NatureCapacity::GetAsnID() const
{
    return nAsnID_;
}

} // end namespace TIC