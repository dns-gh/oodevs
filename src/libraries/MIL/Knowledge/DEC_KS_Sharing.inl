// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_Sharing.inl $
// $Author: Nld $
// $Modtime: 20/04/05 16:58 $
// $Revision: 2 $
// $Workfile: DEC_KS_Sharing.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::ShareFromSource
// Created: NLD 2005-04-20
// -----------------------------------------------------------------------------
inline
void DEC_KS_Sharing::ShareFromSource( const MIL_KnowledgeGroup& source, uint nShareTimeStep )
{
    shareSources_.insert( std::make_pair( nShareTimeStep, sShareSource( source ) ) );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_KS_Sharing::ShareFromSource
// Created: NLD 2005-04-20
// -----------------------------------------------------------------------------
inline
void DEC_KS_Sharing::ShareFromSource( const MIL_KnowledgeGroup& source, uint nShareTimeStep, const MT_Vector2D& vSharedCircleCenter, MT_Float rSharedCircleRadius )
{
    shareSources_.insert( std::make_pair( nShareTimeStep, sShareSource( source, vSharedCircleCenter, rSharedCircleRadius ) ) );
}
