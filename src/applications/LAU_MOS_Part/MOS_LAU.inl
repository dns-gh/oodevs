//*****************************************************************************
//
// $Created : AML 03-04-01 $
// $Archive: /MVW_v10/Build/SDK/LAU_MOS_Part/src/MOS_LAU.inl $
// $Author: Aml $
// $Modtime: 18/04/03 10:25 $
// $Revision: 4 $
// $Workfile: MOS_LAU.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name   : MOS_LAU::SetDinEngine
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
INLINE
void MOS_LAU::SetDinEngine( DIN::DIN_Engine* pEngine )
{
    pEngine_ = pEngine;
}


//-----------------------------------------------------------------------------
// Name: MOS_LAU::GetHostName
// Created: AML 03-04-18
//-----------------------------------------------------------------------------
INLINE
const std::string& MOS_LAU::GetHostName() const
{
    return strHostName_;
}



//-----------------------------------------------------------------------------
// Name: MOS_LAU::SetHostName
// Created: AML 03-04-18
//-----------------------------------------------------------------------------
INLINE
void MOS_LAU::SetHostName( const std::string& strHostName )
{
    strHostName_ = strHostName;
}



//-------------------------------------------------------------------------
// Messages tools
//-------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name: DIN::DIN_BufferedMessage 
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
INLINE
DIN::DIN_BufferedMessage MOS_LAU::BuildMessage()
{
    assert( pMessageService_ );
    return DIN::DIN_BufferedMessage( *pMessageService_ );
}



//-----------------------------------------------------------------------------
// Name: MOS_LAU::GetTheOnlyMOS_LAU
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
INLINE
MOS_LAU* MOS_LAU::GetTheOnlyMOS_LAU( void )
{
    return static_pTheOnlyMOS_LAU;
}
