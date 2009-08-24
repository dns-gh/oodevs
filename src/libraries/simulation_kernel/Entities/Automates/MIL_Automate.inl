// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/MIL_Automate.inl $
// $Author: Jvt $
// $Modtime: 24/03/05 14:03 $
// $Revision: 10 $
// $Workfile: MIL_Automate.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetKnowledge
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
inline
DEC_KnowledgeBlackBoard_Automate& MIL_Automate::GetKnowledge() const
{
    assert( pKnowledgeBlackBoard_ );
    return *pKnowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetTC2
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
MIL_AutomateLOG* MIL_Automate::GetTC2() const
{
    return pTC2_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetNominalTC2
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
inline
MIL_AutomateLOG* MIL_Automate::GetNominalTC2() const
{
    if( IsSurrendered() )
        return 0;
    return pNominalTC2_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::IsSurrendered
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
inline
bool MIL_Automate::IsSurrendered() const
{
    return pArmySurrenderedTo_ != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetArmySurrenderedTo
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
inline
const MIL_Army* MIL_Automate::GetArmySurrenderedTo() const
{
    return pArmySurrenderedTo_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetID
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
inline
uint MIL_Automate::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetType
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
inline
const MIL_AutomateType& MIL_Automate::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetPionPC
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
inline
MIL_AgentPion& MIL_Automate::GetPionPC() const
{
    assert( pPionPC_ );
    return *pPionPC_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetKnowledgeGroup
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
inline
MIL_KnowledgeGroup& MIL_Automate::GetKnowledgeGroup() const
{
    assert( pKnowledgeGroup_ );
    return *pKnowledgeGroup_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetOrderManager
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const MIL_AutomateOrderManager& MIL_Automate::GetOrderManager() const
{
    return orderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetOrderManager
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
MIL_AutomateOrderManager& MIL_Automate::GetOrderManager()
{
    return orderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetPions
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
const MIL_Automate::T_PionVector& MIL_Automate::GetPions() const
{
    return pions_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetAutomates
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
inline
const MIL_Automate::T_AutomateVector& MIL_Automate::GetAutomates() const
{
    return automates_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetParentAutomate
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
inline
MIL_Automate* MIL_Automate::GetParentAutomate() const
{
    return pParentAutomate_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::IsEngaged
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
bool MIL_Automate::IsEngaged() const
{
    return bEngaged_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::RegisterPion
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
inline
void MIL_Automate::RegisterPion( MIL_AgentPion& pion )
{
    assert( std::find( pions_.begin(), pions_.end(), &pion ) == pions_.end() );
    pions_.push_back( &pion );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UnregisterPion
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
inline
void MIL_Automate::UnregisterPion( MIL_AgentPion& pion )
{
    IT_PionVector it = std::find( pions_.begin(), pions_.end(), &pion );
    assert( it != pions_.end() );
    pions_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::RegisterAutomate
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
inline
void MIL_Automate::RegisterAutomate( MIL_Automate& automate )
{
    assert( std::find( automates_.begin(), automates_.end(), &automate ) == automates_.end() );
    automates_.push_back( &automate );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UnregisterAutomate
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
inline
void MIL_Automate::UnregisterAutomate( MIL_Automate& automate )
{
    IT_AutomateVector it = std::find( automates_.begin(), automates_.end(), &automate );
    assert( it != automates_.end() );
    automates_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > MIL_Automate::ApplyOnHierarchy
// Created: NLD 2007-05-09
// -----------------------------------------------------------------------------
template< typename T > 
void MIL_Automate::ApplyOnHierarchy( T& functor )
{
    functor(*this);

    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        (**it).ApplyOnHierarchy( functor );

    for( MIL_Automate::CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
        functor( **it );
}
