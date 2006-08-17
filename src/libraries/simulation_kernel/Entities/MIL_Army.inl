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
// Name: MIL_Army::GetRelationWith
// Created: NLD 2004-03-24
//-----------------------------------------------------------------------------
inline
MIL_Army::E_Diplomacy MIL_Army::GetRelationWith( const MIL_Army& otherArmy ) const
{
    if( &otherArmy == this )
        return eFriend;

    CIT_ArmyRelationMap itRelation = relations_.find( &otherArmy );
    if( itRelation == relations_.end() )
        return eUnknown;
    else
        return itRelation->second;
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
