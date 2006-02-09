// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: FragmentaryOrderInterface destructor
// Created: AGR
// -----------------------------------------------------------------------------
FragmentaryOrderInterface::~FragmentaryOrderInterface()
{
    switch( pASNMsgOrder_->GetAsnMsg().order_conduite.t )
    {
$AsnOrderDeletion$
    }
}


// -----------------------------------------------------------------------------
// Name: FragmentaryOrderInterface::CreateInterface
// Created: AGR
// -----------------------------------------------------------------------------
void FragmentaryOrderInterface::CreateInterface()
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