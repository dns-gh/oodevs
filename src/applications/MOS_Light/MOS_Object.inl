//*****************************************************************************
//
// $Created: FBD 02-11-22 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Object.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 6 $
// $Workfile: MOS_Object.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: FER_Object_ABC::E_TypeTerrainObject 
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
inline
E_TerrainObjectTypeID MOS_Object::GetType() const
{
    return nType_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Object::GetId
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
inline
uint32 MOS_Object::GetId() const
{
    return nId_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Object::GetSizeUrban
// Created: FBD 03-04-24
//-----------------------------------------------------------------------------
inline
MT_Float MOS_Object::GetValue() const
{
    return rValue_;
}


//-----------------------------------------------------------------------------
// Name: std::string
// Created: FBD 03-04-24
//-----------------------------------------------------------------------------
inline
const std::string& MOS_Object::GetName() const
{
    return sName_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Object::GetFrame
// Created: FBD 03-04-25
//-----------------------------------------------------------------------------
inline
uint MOS_Object::GetFrame() const
{
    return nFrame_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Object::SetFrame
// Created: FBD 03-04-25
//-----------------------------------------------------------------------------
inline
void MOS_Object::SetFrame( uint nFrame )
{
    nFrame_ = nFrame;
}



//-----------------------------------------------------------------------------
// Name: MOS_Object::IsitArea
// Created: FBD 03-03-11
//-----------------------------------------------------------------------------
inline
bool MOS_Object::IsItArea() const
{
    return ( nTypeShape_ == eTypeShapeTri );
}

//-----------------------------------------------------------------------------
// Name: MOS_Object::IsitArea
// Created: FBD 03-03-11
//-----------------------------------------------------------------------------
inline
bool MOS_Object::IsItPoint() const
{
    return ( nTypeShape_ == eTypeShapeNode );
}

//-----------------------------------------------------------------------------
// Name: MOS_Object::IsitLine
// Created: FBD 03-03-11
//-----------------------------------------------------------------------------
inline
bool MOS_Object::IsItLine() const
{
    return ( nTypeShape_ == eTypeShapeSegment );
}

//-----------------------------------------------------------------------------
// Name: MOS_Object::GetListShapePatchs
// Created: FBD 03-04-15
//-----------------------------------------------------------------------------
inline
MOS_Object::CT_ShapePatchMap& MOS_Object::GetListShapePatchs() const
{
    return shapePatchMap_;
}
