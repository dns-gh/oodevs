//*****************************************************************************
//
// $Created: FBD 03-04-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Line.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 4 $
// $Workfile: MOS_Line.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: MOS_Line::GetLine
// Created: FBD 03-04-08
//-----------------------------------------------------------------------------
inline
MT_Line MOS_Line::GetLine()
{
    return MT_Line( pNode1_->GetPos(), pNode2_->GetPos() );
}


//-----------------------------------------------------------------------------
// Name: MOS_Line::GetPosStart
// Created: FBD 03-04-08
//-----------------------------------------------------------------------------
inline
const MOS_Node& MOS_Line::GetPosStart() const
{
    return *pNode1_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Line::GetPosEnd
// Created: FBD 03-04-08
//-----------------------------------------------------------------------------
inline
const MOS_Node& MOS_Line::GetPosEnd() const
{
    return *pNode2_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Line::GetPosStart
// Created: FBD 03-04-08
//-----------------------------------------------------------------------------
inline
MOS_Node& MOS_Line::GetPosStart()
{
    return *pNode1_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Line::GetPosEnd
// Created: FBD 03-04-08
//-----------------------------------------------------------------------------
inline
MOS_Node& MOS_Line::GetPosEnd()
{
    return *pNode2_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Line::GetFrame
// Created: FBD 03-04-10
//-----------------------------------------------------------------------------
inline
uint MOS_Line::GetFrame() const
{
    return nFrame_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Line::SetFrame
// Created: FBD 03-04-10
//-----------------------------------------------------------------------------
inline
void MOS_Line::SetFrame( uint nFrame )
{
    nFrame_ = nFrame;
}


//-----------------------------------------------------------------------------
// Name: MOS_Line::IsInside
// Created: FBD 03-04-15
//-----------------------------------------------------------------------------
inline
bool MOS_Line::IsInside( const MT_Vector2D& vPos, MT_Float rWeldValue ) const
{
    if ( MT_Line( pNode1_->GetPos() , pNode2_->GetPos() ).ClosestPointOnLine( vPos ).SquareDistance( vPos ) <= rWeldValue * rWeldValue )
        return true;
    return false;
}


