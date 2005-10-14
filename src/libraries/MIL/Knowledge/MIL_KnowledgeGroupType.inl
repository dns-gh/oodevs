// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/MIL_KnowledgeGroupType.inl $
// $Author: Nld $
// $Modtime: 21/04/05 19:07 $
// $Revision: 5 $
// $Workfile: MIL_KnowledgeGroupType.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::FindPionType
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
inline
const MIL_KnowledgeGroupType* MIL_KnowledgeGroupType::FindType( const std::string& strName )
{
    CIT_KnowledgeGroupTypeMap it = knowledgeGroupTypes_.find( strName );
    return it == knowledgeGroupTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::FindType
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const MIL_KnowledgeGroupType* MIL_KnowledgeGroupType::FindType( uint nID )
{
    CIT_KnowledgeGroupTypeMap it = std::find_if( knowledgeGroupTypes_.begin(), knowledgeGroupTypes_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &MIL_KnowledgeGroupType::GetID ), std::select2nd< T_KnowledgeGroupTypeMap::value_type >() ) ) );
    return it == knowledgeGroupTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetID
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
uint MIL_KnowledgeGroupType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetKnowledgePopulationMaxLifeTime
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
inline
MT_Float MIL_KnowledgeGroupType::GetKnowledgePopulationMaxLifeTime() const
{
    return rKnowledgePopulationMaxLifeTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetKnowledgeAgentMaxLifeTime
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
inline
MT_Float MIL_KnowledgeGroupType::GetKnowledgeAgentMaxLifeTime() const
{
    return rKnowledgeAgentMaxLifeTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
inline
MT_Float MIL_KnowledgeGroupType::GetKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit() const
{
    return rKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroupType::GetKnowledgeAgentExtrapolationTime
// Created: JVT 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float MIL_KnowledgeGroupType::GetKnowledgeAgentExtrapolationTime() const
{
    return rKnowledgeAgentExtrapolationTime_;
}

// -----------------------------------------------------------------------------
// Name: mil_knowledgegrouptype::GetName
// Created: JVT 2005-03-01
// -----------------------------------------------------------------------------
inline
const std::string& MIL_KnowledgeGroupType::GetName() const
{
    return strName_;
}
