// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/MIL_Army.inl $
// $Author: Jvt $
// $Modtime: 6/04/05 17:14 $
// $Revision: 3 $
// $Workfile: MIL_Army.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_Army::RegisterFormation
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
inline
void MIL_Army::RegisterFormation( MIL_Formation& formation )
{
    bool bOut = formations_.insert( &formation ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::UnregisterFormation
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
inline
void MIL_Army::UnregisterFormation( MIL_Formation& formation )
{
    int nOut = formations_.erase( &formation );
    assert( nOut == 1 );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::RegisterPopulation
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
inline
void MIL_Army::RegisterPopulation( MIL_Population& population )
{
    bool bOut = populations_.insert( &population ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::UnregisterPopulation
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
inline
void MIL_Army::UnregisterPopulation( MIL_Population& population )
{
    int nOut = populations_.erase( &population );
    assert( nOut == 1 );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::FindKnowledgeGroup
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
inline
MIL_KnowledgeGroup* MIL_Army::FindKnowledgeGroup( uint nID ) const
{
    CIT_KnowledgeGroupMap it = knowledgeGroups_.find( nID );
    if( it == knowledgeGroups_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::operator==
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
inline
bool MIL_Army::operator==( const MIL_Army& rhs ) const
{
    return nID_ == rhs.nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::operator!=
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
inline
bool MIL_Army::operator!=( const MIL_Army& rhs ) const
{
    return nID_ != rhs.nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::GetID
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
inline
uint MIL_Army::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::GetKnowledgeGroups
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
inline
const MIL_Army::T_KnowledgeGroupMap& MIL_Army::GetKnowledgeGroups() const
{
    return knowledgeGroups_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::GetKnowledge
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
inline
DEC_KnowledgeBlackBoard_Army& MIL_Army::GetKnowledge() const
{
    assert( pKnowledgeBlackBoard_ );
    return *pKnowledgeBlackBoard_;
}

//-----------------------------------------------------------------------------
// Name: MIL_Army::GetDiplomacy
// Created: NLD 2004-03-24
//-----------------------------------------------------------------------------
inline
MIL_Army::E_Diplomacy MIL_Army::GetDiplomacy( const MIL_Army& otherArmy ) const
{
    if( &otherArmy == this )
        return eFriend;

    CIT_DiplomacyMap it = diplomacies_.find( &otherArmy );
    if( it == diplomacies_.end() )
        return eUnknown;
    else
        return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::GetName
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const std::string& MIL_Army::GetName() const
{
    return strName_;
}
