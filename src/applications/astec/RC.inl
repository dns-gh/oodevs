//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/RC.inl $
// $Author: Ape $
// $Modtime: 22/10/04 17:56 $
// $Revision: 4 $
// $Workfile: RC.inl $
//
//*****************************************************************************

// $$$$ HME 2005-10-05: ETRANGE: LE COMPILO PRETEND QUE CES FONCTIONS ONT DEJA ETE DEFINIES

//-----------------------------------------------------------------------------
// Name: RC::GetFragOrders
// Created: NLD 2003-06-05
//-----------------------------------------------------------------------------
inline
const RC::T_FragOrderVector& RC::GetFragOrders() const
{
    return fragOrderVector_;
}


// -----------------------------------------------------------------------------
// Name: RC::GetAgent
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
inline
Agent_ABC& RC::GetAgent()
{
    return agent_;
}
