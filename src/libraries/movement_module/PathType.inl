//*****************************************************************************
//
// $Created: JDY 03-04-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/PathType.inl $
// $Author: Nld $
// $Modtime: 30/04/03 17:17 $
// $Revision: 1 $
// $Workfile: PathType.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: PathType::Find
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
const PathType* PathType::Find( const std::string& strName )
{
    CIT_PathTypeMap it = pathTypes_.find ( strName );
    if( it == pathTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PathType::Find
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
const PathType* PathType::Find( unsigned int nID )
{
    assert( pathTypesFromID_.size() > nID );
    return pathTypesFromID_[ nID ];
}

// -----------------------------------------------------------------------------
// Name: PathType::GetID
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
unsigned int PathType::GetID() const
{
    return nPathType_;
}

// -----------------------------------------------------------------------------
// Name: PathType::GetName
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
const std::string& PathType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PathType::GetPathTypes
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
PathType::T_PathTypeMap& PathType::GetPathTypes()
{
    return pathTypes_;
}
