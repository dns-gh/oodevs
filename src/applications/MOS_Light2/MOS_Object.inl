//*****************************************************************************
//
// $Created: FBD 02-11-22 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Object.inl $
// $Author: Ape $
// $Modtime: 19/07/04 16:24 $
// $Revision: 2 $
// $Workfile: MOS_Object.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: FER_Object_ABC::E_TypeTerrainObject 
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
MOS_INLINE
E_TerrainObjectTypeID MOS_Object::GetType() const
{
    return nType_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Object::GetId
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
MOS_INLINE
uint32 MOS_Object::GetId() const
{
    return nId_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Object::GetValue
// Created: FBD 03-04-24
//-----------------------------------------------------------------------------
MOS_INLINE
MT_Float MOS_Object::GetValue() const
{
    return rValue_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Object::GetName
// Created: FBD 03-04-24
//-----------------------------------------------------------------------------
MOS_INLINE
const std::string& MOS_Object::GetName() const
{
    return sName_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Object::IsArea
// Created: FBD 03-03-11
//-----------------------------------------------------------------------------
MOS_INLINE
bool MOS_Object::IsArea() const
{
    return ( nTypeShape_ == eTypeShapeTri );
}


//-----------------------------------------------------------------------------
// Name: MOS_Object::IsArea
// Created: FBD 03-03-11
//-----------------------------------------------------------------------------
MOS_INLINE
bool MOS_Object::IsPoint() const
{
    return ( nTypeShape_ == eTypeShapeNode );
}


//-----------------------------------------------------------------------------
// Name: MOS_Object::IsLine
// Created: FBD 03-03-11
//-----------------------------------------------------------------------------
MOS_INLINE
bool MOS_Object::IsLine() const
{
    return ( nTypeShape_ == eTypeShapeSegment );
}
