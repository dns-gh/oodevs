// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE $TIME$ $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MOS_FragmentaryOrderInterface_Gen_Skeleton.cpp $
// $Author: Ape $
// $Modtime: 25/10/04 10:45 $
// $Revision: 4 $
// $Workfile: AGR_MOS_FragmentaryOrderInterface_Gen_Skeleton.cpp $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface destructor
// Created: AGR
// -----------------------------------------------------------------------------
MOS_FragmentaryOrderInterface::~MOS_FragmentaryOrderInterface()
{
    switch( pASNMsgOrder_->GetAsnMsg().order_conduite.t )
    {
$AsnOrderDeletion$
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateInterface
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateInterface()
{
    switch( nMissionId_ )
    {
$MosOrderCreation$
        default:
            assert( false );
    }

    this->CreateOkCancelButtons();
}


$OrderCreatorImplementations$