//*****************************************************************************
//
// $Created: NLD 2002-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Value.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 10 $
// $Workfile: MOS_Value.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_Value::GetValueNetwork
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
inline
MT_Float MOS_Value::GetValueNetwork() const
{
    if ( nTypeValue_ == eTypeValueFloat)
        return rValueNetwork_ / 65536.;
    return rValueNetwork_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Value::GetValueWidget
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
inline
MT_Float MOS_Value::GetValueWidget() const
{
    if ( nTypeValue_ == eTypeValueFloat)
        return rValueWidget_ / 65536.;
    return rValueWidget_;
}

//-----------------------------------------------------------------------------
// Name: MOS_Value::GetName
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
inline
const std::string& MOS_Value::GetName() const
{
    return sName_;
}

//-----------------------------------------------------------------------------
// Name: MOS_Value::GetAgent
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
inline
MOS_Agent* MOS_Value::GetAgent() const
{
    return pAgent_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Value::SetAgent
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
inline
void MOS_Value::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;
}
//-----------------------------------------------------------------------------
// Name: MOS_Value::IsModifiedNetwork
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
inline
bool MOS_Value::IsModifiedNetwork() const
{
    return bModifiedNetwork_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Value::SetModifiedNetwork
// Created: NLD 2003-01-27
//-----------------------------------------------------------------------------
inline
void MOS_Value::SetModifiedNetwork( bool bModified )
{
    bModifiedNetwork_ = bModified;
}

//-----------------------------------------------------------------------------
// Name: MOS_Value::IsModifiedWidget
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
inline
bool MOS_Value::IsModifiedWidget() const
{
    return bModifiedWidget_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Value::SetModifiedWidget
// Created: NLD 2003-01-27
//-----------------------------------------------------------------------------
inline
void MOS_Value::SetModifiedWidget( bool bModified )
{
    bModifiedWidget_ = bModified;
}


//-----------------------------------------------------------------------------
// Name: MOS_Value::SetId
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
inline
void MOS_Value::SetId( int nType )
{
    nType_ = nType;
}

//-----------------------------------------------------------------------------
// Name: MOS_Value::GetId
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
inline
int MOS_Value::GetId() const
{
    return nType_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Value::GetSpeedo
// Created: FBD 03-01-22
//-----------------------------------------------------------------------------
inline
QDial* MOS_Value::GetSpeedo() const
{
    return pSpeedo_;
}

