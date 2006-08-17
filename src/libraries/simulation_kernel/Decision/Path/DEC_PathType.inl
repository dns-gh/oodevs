//*****************************************************************************
//
// $Created: JDY 03-04-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathType.inl $
// $Author: Nld $
// $Modtime: 30/04/03 17:17 $
// $Revision: 1 $
// $Workfile: DEC_PathType.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_PathType::Find
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
const DEC_PathType* DEC_PathType::Find( const std::string& strName )
{
    CIT_PathTypeMap it = pathTypes_.find ( strName );
    if( it == pathTypes_.end() )
        return 0;
    return it->second;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PathType::Find
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
const DEC_PathType* DEC_PathType::Find( uint nID )
{
    assert( pathTypesFromID_.size() > nID );
    return pathTypesFromID_[ nID ];
}

// -----------------------------------------------------------------------------
// Name: DEC_PathType::GetID
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
uint DEC_PathType::GetID() const
{
    return nPathType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathType::GetName
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
const std::string& DEC_PathType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathType::GetPathTypes
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
DEC_PathType::T_PathTypeMap& DEC_PathType::GetPathTypes()
{
    return pathTypes_;
}
