// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_VirtualObjectType.inl $
// $Author: Nld $
// $Modtime: 26/10/04 18:25 $
// $Revision: 1 $
// $Workfile: MIL_VirtualObjectType.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_VirtualObjectType::FindObjectType
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
inline
const MIL_VirtualObjectType* MIL_VirtualObjectType::FindObjectType( const std::string& strName )
{
    for( CIT_ObjectTypeMap it = objectTypes_.begin(); it != objectTypes_.end(); ++it )
        if( sCaseInsensitiveEqual()( it->second->GetName(), strName ) )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_VirtualObjectType::GetName
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
inline
const std::string& MIL_VirtualObjectType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_VirtualObjectType::IsInitialized
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
inline
bool MIL_VirtualObjectType::IsInitialized() const
{
    return bInitialized_;
}
