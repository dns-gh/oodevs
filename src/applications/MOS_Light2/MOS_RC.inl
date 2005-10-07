//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_RC.inl $
// $Author: Ape $
// $Modtime: 22/10/04 17:56 $
// $Revision: 4 $
// $Workfile: MOS_RC.inl $
//
//*****************************************************************************

// $$$$ HME 2005-10-05: ETRANGE: LE COMPILO PRETEND QUE CES FONCTIONS ONT DEJA ETE DEFINIES

//-----------------------------------------------------------------------------
// Name: MOS_RC::GetFragOrders
// Created: NLD 2003-06-05
//-----------------------------------------------------------------------------
inline
const MOS_RC::T_FragOrderVector& MOS_RC::GetFragOrders()const
{
    return fragOrderVector_;
}


// -----------------------------------------------------------------------------
// Name: MOS_RC::GetAgent
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
inline
MOS_Agent_ABC& MOS_RC::GetAgent()
{
    return agent_;
}
