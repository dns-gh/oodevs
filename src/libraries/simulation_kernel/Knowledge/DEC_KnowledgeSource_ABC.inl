// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeSource_ABC.inl $
// $Author: Jvt $
// $Modtime: 31/03/05 16:26 $
// $Revision: 2 $
// $Workfile: DEC_KnowledgeSource_ABC.inl $
//
// *****************************************************************************

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_KnowledgeSource_ABC::serialize
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_KnowledgeSource_ABC::serialize( Archive& archive, const uint )
{
    archive & nPriority_
            & pBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeSource_ABC::GetPriority
// Created: NLD 2006-01-19
// -----------------------------------------------------------------------------
inline
uint DEC_KnowledgeSource_ABC::GetPriority() const
{
    return nPriority_;
}
