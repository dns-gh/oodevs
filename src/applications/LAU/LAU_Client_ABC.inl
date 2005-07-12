//*****************************************************************************
//
// Name     : LAU_Client_ABC.inl
//
// $Created : AML 03-04-01 $
//  $Archive: /MVW_v10/Build/SDK/LAU/src/LAU_Client_ABC.inl $
//   $Author: Aml $
//  $Modtime: 12/06/03 10:04 $
// $Revision: 5 $
// $Workfile: LAU_Client_ABC.inl $
//
//*****************************************************************************

//#include "LAU_Client_ABC.h" // Pour pouvoir le compiler tout seul...

//=============================================================================
// MAIN METHODS
//=============================================================================


//-----------------------------------------------------------------------------
// Name: LAU_Client_ABC::HasSession
// Created: AML 03-04-28
//-----------------------------------------------------------------------------
INLINE
bool32 LAU_Client_ABC::HasSession() const
{
    return pSession_ != 0;
}


//-----------------------------------------------------------------------------
// Name: LAU_Client_ABC::GetSession
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
INLINE
DIN::DIN_Link& LAU_Client_ABC::GetSession() const
{
    assert( HasSession() );
    return *pSession_;
}


//-----------------------------------------------------------------------------
// Name: LAU_Client_ABC::GetStationID
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
INLINE
DIN::DIN_StationID LAU_Client_ABC::GetStationID() const
{
    assert( HasSession() );
    return pSession_->GetStationID();
}


//-----------------------------------------------------------------------------
// Name: LAU_Client_ABC::Initialize
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
INLINE
void LAU_Client_ABC::Initialize( DIN::DIN_Link& link )
{
    pSession_ = &link;
}


//-----------------------------------------------------------------------------
// Name: LAU_Client_ABC::Terminate
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
INLINE
void LAU_Client_ABC::Terminate()
{
    if( HasSession() )
    {
        pSession_->SetUserData( 0 );
        pSession_ = 0;
    }
}


//-----------------------------------------------------------------------------
// Name: LAU_Client_ABC::SessionIsNoMoreValid
// Created: AML 03-06-12
//-----------------------------------------------------------------------------
INLINE
void LAU_Client_ABC::SessionIsNoMoreValid()
{
    pSession_ = 0;
}
