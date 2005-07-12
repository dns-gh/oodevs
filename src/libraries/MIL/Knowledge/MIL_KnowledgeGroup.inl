// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/MIL_KnowledgeGroup.inl $
// $Author: Jvt $
// $Modtime: 24/03/05 13:04 $
// $Revision: 6 $
// $Workfile: MIL_KnowledgeGroup.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::operator==
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
inline
bool MIL_KnowledgeGroup::operator==( const MIL_KnowledgeGroup& rhs ) const
{
    return nID_ == rhs.nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::operator!=
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
bool MIL_KnowledgeGroup::operator!=( const MIL_KnowledgeGroup& rhs ) const
{
    return nID_ != rhs.nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetID
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
inline
uint MIL_KnowledgeGroup::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetAutomates
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
inline
const MIL_KnowledgeGroup::T_AutomateVector& MIL_KnowledgeGroup::GetAutomates() const
{
    return automates_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetKSQuerier
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
inline
const DEC_KS_KnowledgeGroupQuerier& MIL_KnowledgeGroup::GetKSQuerier() const
{
    assert( pKsQuerier_ );
    return *pKsQuerier_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetKSSharing
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
inline
DEC_KS_Sharing& MIL_KnowledgeGroup::GetKSSharing()
{
    assert( pKsSharing_ );
    return *pKsSharing_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetKnowledge
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
inline
const DEC_KnowledgeBlackBoard& MIL_KnowledgeGroup::GetKnowledge() const
{
    assert( pKnowledgeBlackBoard_ );
    return *pKnowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetArmy
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
MIL_Army& MIL_KnowledgeGroup::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetKnowledgeMaxLifeTime
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
inline
MT_Float MIL_KnowledgeGroup::GetKnowledgeMaxLifeTime() const
{
    assert( pType_ );
    return pType_->GetKnowledgeMaxLifeTime();
}
          
// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetKnowledgeMaxDistBtwKnowledgeAndRealUnit
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
inline
MT_Float MIL_KnowledgeGroup::GetKnowledgeMaxDistBtwKnowledgeAndRealUnit() const
{
    assert( pType_ );
    return pType_->GetKnowledgeMaxDistBtwKnowledgeAndRealUnit();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::RegisterAutomate
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
inline
void MIL_KnowledgeGroup::RegisterAutomate( MIL_Automate& automate )
{
    assert( std::find( automates_.begin(), automates_.end(), &automate ) == automates_.end() );
    automates_.push_back( &automate );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UnregisterAutomate
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
inline
void MIL_KnowledgeGroup::UnregisterAutomate( MIL_Automate& automate )
{
    IT_AutomateVector it = std::find( automates_.begin(), automates_.end(), &automate );
    assert( it != automates_.end() );
    automates_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetType
// Created: JVT 2004-11-29
// -----------------------------------------------------------------------------
inline
const MIL_KnowledgeGroupType& MIL_KnowledgeGroup::GetType() const
{
    assert( pType_ );
    return *pType_;
}

