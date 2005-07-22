// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-12 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/KnowledgeGroup.inl $
// $Author: Sbo $
// $Modtime: 21/07/05 17:04 $
// $Revision: 5 $
// $Workfile: KnowledgeGroup.inl $
//
// *****************************************************************************

namespace TIC
{

//-----------------------------------------------------------------------------
// Name: KnowledgeGroup::Find
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
KnowledgeGroup* KnowledgeGroup::Find( T_EntityId nId )
{
	CIT_KnowledgeGroupMap it;
	it = knowledgeGroups_.find( nId );
	if ( it != knowledgeGroups_.end() )
		return ( *it ).second;
	return 0;
}

//-----------------------------------------------------------------------------
// Name: KnowledgeGroup::Register
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
void KnowledgeGroup::Register( KnowledgeGroup& knowledgeGroup )
{
    bool bOut = knowledgeGroups_.insert( std::make_pair( knowledgeGroup.GetId(), &knowledgeGroup ) ).second;
    assert( bOut );
}

//-----------------------------------------------------------------------------
// Name: KnowledgeGroup::Unregister
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
void KnowledgeGroup::Unregister( KnowledgeGroup& knowledgeGroup )
{
    int nOut = knowledgeGroups_.erase( knowledgeGroup.GetId() );
    assert( nOut == 1 );
}

//-----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetId
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
T_EntityId KnowledgeGroup::GetId() const
{
	return nId_;
}

//-----------------------------------------------------------------------------
// Name: KnowledgeGroup::AddDisaggregation
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
inline
void KnowledgeGroup::AddDisaggregation( T_EntityId nId )
{
    bool bOut = disaggregKnowledgeGroups_.insert( nId ).second;
    assert( bOut );
}


//-----------------------------------------------------------------------------
// Name: KnowledgeGroup::MustBeDisaggregated
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
inline
bool KnowledgeGroup::MustBeDisaggregated() const
{
    return bConfigDisaggreg_;
}

} // end namespace TIC