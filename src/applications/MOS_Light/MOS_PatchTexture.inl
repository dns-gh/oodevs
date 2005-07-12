//*****************************************************************************
// Name   : MOS_PatchTexture.inl
// Created: FBD 02-02-27
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::GetRect
// Created: FBD 02-11-14
//-----------------------------------------------------------------------------
inline
const MT_Rect& MOS_PatchTexture::GetRect() const
{
    return rect_;
}

//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::GetIdxPatchX
// Created: FBD 02-11-12
//-----------------------------------------------------------------------------
inline
uint MOS_PatchTexture::GetIdxPatchX() const
{
    return nIdxPatchX_;
}

//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::GetIdxPatchY
// Created: FBD 02-11-12
//-----------------------------------------------------------------------------
inline
uint MOS_PatchTexture::GetIdxPatchY() const
{
    return nIdxPatchY_;
}

//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::IsInside
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
inline
bool MOS_PatchTexture::IsInside( const MT_Vector2D& point )
{
    return rect_.IsInside( point );
}


//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::Intersect2D
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
inline
bool MOS_PatchTexture::Intersect2D( const MT_Line& line )
{
    return rect_.Intersect2D( line );
}

//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::Intersect2D
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
inline
bool MOS_PatchTexture::Intersect2D( const MT_Triangle& triangle )
{
    return rect_.Intersect2D( triangle );
}

//-----------------------------------------------------------------------------
//  Name  :  MOS_PatchTexture::GetPatchUp
// Created: FBD 02-03-04
//-----------------------------------------------------------------------------
inline
MOS_PatchTexture* MOS_PatchTexture::GetPatchUp()
{
 	return pPatchUp_;
}


//-----------------------------------------------------------------------------
//  Name  :  MOS_PatchTexture::GetPatchRight
// Created: FBD 02-03-04
//-----------------------------------------------------------------------------
inline
MOS_PatchTexture* MOS_PatchTexture::GetPatchRight()
{
	return pPatchRight_;
}


//-----------------------------------------------------------------------------
//  Name  :  MOS_PatchTexture::GetPatchDown
// Created: FBD 02-03-04
//-----------------------------------------------------------------------------
inline
MOS_PatchTexture* MOS_PatchTexture::GetPatchDown()
{
	return pPatchDown_;
}


//-----------------------------------------------------------------------------
//  Name  :  MOS_PatchTexture::GetPatchLeft
// Created: FBD 02-03-04
//-----------------------------------------------------------------------------
inline
MOS_PatchTexture* MOS_PatchTexture::GetPatchLeft()
{
	return pPatchLeft_;
}


//=============================================================================
// PATH FINDER
//=============================================================================




//-----------------------------------------------------------------------------
// Name: GFX_World::MOS_PatchTexture
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
inline
MOS_GLWidget* MOS_PatchTexture::GetGLWidget() const
{
    return pGlWidget_;
}

//-----------------------------------------------------------------------------
// Name: MOS_World::MOS_PatchTexture
// Created: FBD 02-11-27
//-----------------------------------------------------------------------------
inline
void MOS_PatchTexture::SetGLWidget( MOS_GLWidget* pGlWidget )
{
    pGlWidget_ = pGlWidget;
}

//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::GetHandle
// Created: FBD 03-03-04
//-----------------------------------------------------------------------------
inline
MT_Float MOS_PatchTexture::GetHandle() const
{
    return rHandlePatch_;
}

//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::GetFrame
// Created: FBD 03-04-10
//-----------------------------------------------------------------------------
inline
uint32 MOS_PatchTexture::GetFrame() const
{
    return nFrame_;
}


//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::SetFrame
// Created: FBD 03-04-10
//-----------------------------------------------------------------------------
inline
void MOS_PatchTexture::SetFrame( uint nFrame )
{
    nFrame = nFrame_;
}


