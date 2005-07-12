//*****************************************************************************
//
// $Created: FBD 03-04-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Line.inl $
// $Author: Ape $
// $Modtime: 20/07/04 11:17 $
// $Revision: 2 $
// $Workfile: MOS_Line.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_Line::GetLine
// Created: FBD 03-04-08
//-----------------------------------------------------------------------------
MOS_INLINE
MT_Line MOS_Line::GetLine()
{
    assert( pNode1_ != 0 && pNode2_ != 0 );
    return MT_Line( pNode1_->GetPos(), pNode2_->GetPos() );
}


//-----------------------------------------------------------------------------
// Name: MOS_Line::GetPosStart
// Created: FBD 03-04-08
//-----------------------------------------------------------------------------
MOS_INLINE
const MOS_Node& MOS_Line::GetPosStart() const
{
    assert( pNode1_ != 0 );
    return *pNode1_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Line::GetPosEnd
// Created: FBD 03-04-08
//-----------------------------------------------------------------------------
MOS_INLINE
const MOS_Node& MOS_Line::GetPosEnd() const
{
    assert( pNode2_ != 0 );
    return *pNode2_;
}
