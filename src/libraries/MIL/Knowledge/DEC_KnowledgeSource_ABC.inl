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

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeSource_ABC::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive > 
inline
void DEC_KnowledgeSource_ABC::serialize( Archive& file, const uint )
{
    file & pBlackBoard_;
}
