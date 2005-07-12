//*****************************************************************************
//
// $Created: FBD 02-11-22 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Patch.inl $
// $Author: Ape $
// $Modtime: 4/03/04 16:37 $
// $Revision: 1 $
// $Workfile: MOS_Patch.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_Patch::RegisterShapePatch
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
MOS_INLINE
void MOS_Patch::RegisterShapePatch( MOS_ShapePatch& shapePatch )
{
    shapePatchVector_.push_back( &shapePatch );
}


//-----------------------------------------------------------------------------
// Name: MOS_Patch::UnregisterShapePatch
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
MOS_INLINE
void MOS_Patch::UnregisterShapePatch( MOS_ShapePatch& shapePatch )
{
    IT_ShapePatchVector itShapePatch = std::find( shapePatchVector_.begin(), shapePatchVector_.end(), &shapePatch );
    assert( itShapePatch != shapePatchVector_.end() );
    shapePatchVector_.erase( itShapePatch );
}
