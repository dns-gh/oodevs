// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_PopulationKnowledgeSynthetizer.inl $
// $Author: Jvt $
// $Modtime: 16/03/05 18:15 $
// $Revision: 2 $
// $Workfile: DEC_KS_PopulationKnowledgeSynthetizer.inl $
//
// *****************************************************************************

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_KS_PopulationKnowledgeSynthetizer::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_KS_PopulationKnowledgeSynthetizer::serialize( Archive& archive, const uint )
{
    archive & boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this )
            & pBlackBoard_;
}