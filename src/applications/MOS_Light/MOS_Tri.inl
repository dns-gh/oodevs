//*****************************************************************************
// Name   : MOS_Tri.inl
// Created: FBD 02-07-01
//*****************************************************************************


//-----------------------------------------------------------------------------
//  Name  :  MOS_Tri::IsInside
// Created: FBD 02-09-11
//-----------------------------------------------------------------------------
inline
bool MOS_Tri::IsInside( const MT_Vector2D& vPos, MT_Float /*rWeldValue*/ ) const
{

    const MT_Vector2D& vec1 = pNode1_->GetPos();
    const MT_Vector2D& vec2 = pNode2_->GetPos();
    const MT_Vector2D& vec3 = pNode3_->GetPos();

    if ( (vec1.rX_ - vec2.rX_) * vPos.rY_ + (vec2.rY_ - vec1.rY_) * vPos.rX_ + (vec2.rX_ * vec1.rY_) - (vec1.rX_ * vec2.rY_) <= 0. )
    {
        if ( (vec2.rX_ - vec3.rX_) * vPos.rY_ + (vec3.rY_ - vec2.rY_) * vPos.rX_ + (vec3.rX_ * vec2.rY_) - (vec2.rX_ * vec3.rY_) <= 0. )
        {
            if ( (vec3.rX_ - vec1.rX_) * vPos.rY_ + ( vec1.rY_ - vec3.rY_ ) * vPos.rX_ + (vec1.rX_ * vec3.rY_) - (vec3.rX_ * vec1.rY_) <= 0. )
            {
                return true;
            }
        }
    }
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_Tri::Intersect2D
// Created: NLD 2003-01-14
//-----------------------------------------------------------------------------
inline
bool MOS_Tri::Intersect2D( const MT_Line& line ) const
{
    if( line.Intersect2D( MT_Line( pNode1_->GetPos(), pNode2_->GetPos() ) ) )
        return true;

    if( line.Intersect2D( MT_Line( pNode2_->GetPos(), pNode3_->GetPos() ) ) )
        return true;

    if( line.Intersect2D( MT_Line( pNode3_->GetPos(), pNode1_->GetPos() ) ) )
        return true;
    
    if( IsInside( line.GetPosStart() ) )
    {
        assert( IsInside( line.GetPosEnd() ) );
        return true;
    }

    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_Tri::GetCenMOS
// Created: FBD 02-12-09
//-----------------------------------------------------------------------------
inline
MT_Vector2D MOS_Tri::GetCenMOS() const
{
    return ( pNode1_->GetPos() + pNode2_->GetPos() + pNode3_->GetPos() ) / 3.0;
}

//-----------------------------------------------------------------------------
//  Name  :  MOS_Tri::GetPos1
// Created: FBD 02-07-02
//-----------------------------------------------------------------------------
inline
const MOS_Node& MOS_Tri::GetPos1() const
{
    assert( pNode1_ );
	return *pNode1_;
}


//-----------------------------------------------------------------------------
//  Name  :  MOS_Tri::GetPos2
// Created: FBD 02-07-02
//-----------------------------------------------------------------------------
inline
const MOS_Node& MOS_Tri::GetPos2() const
{
    assert( pNode2_ );
	return *pNode2_;
}


//-----------------------------------------------------------------------------
//  Name  :  MOS_Tri::GetPos3
// Created: FBD 02-07-02
//-----------------------------------------------------------------------------
inline
const MOS_Node& MOS_Tri::GetPos3() const
{
    assert( pNode3_ );
	return *pNode3_;
}


//-----------------------------------------------------------------------------
//  Name  :  MOS_Tri::GetPos1
// Created: FBD 02-07-02
//-----------------------------------------------------------------------------
inline
MOS_Node& MOS_Tri::GetPos1()
{
    assert( pNode1_ );
	return *pNode1_;
}


//-----------------------------------------------------------------------------
//  Name  :  MOS_Tri::GetPos2
// Created: FBD 02-07-02
//-----------------------------------------------------------------------------
inline
MOS_Node& MOS_Tri::GetPos2()
{
    assert( pNode2_ );
	return *pNode2_;
}


//-----------------------------------------------------------------------------
//  Name  :  MOS_Tri::GetPos3
// Created: FBD 02-07-02
//-----------------------------------------------------------------------------
inline
MOS_Node& MOS_Tri::GetPos3()
{
    assert( pNode3_ );
	return *pNode3_;
}



//-----------------------------------------------------------------------------
// Name: MOS_Tri::GetSize
// Created: FBD 03-04-10
//-----------------------------------------------------------------------------
inline
MT_Float MOS_Tri::GetSize() const
{
    return rSize_;
}



//-----------------------------------------------------------------------------
// Name: MOS_Tri::GetFrame
// Created: FBD 03-04-10
//-----------------------------------------------------------------------------
inline
uint MOS_Tri::GetFrame() const
{
    return nFrame_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Tri::SetFrame
// Created: FBD 03-04-10
//-----------------------------------------------------------------------------
inline
void MOS_Tri::SetFrame( uint nFrame )
{
    nFrame_ = nFrame;
}

