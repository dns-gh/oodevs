// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-18 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Export/Communication.cpp $
// $Author: Sbo $
// $Modtime: 20/07/05 18:01 $
// $Revision: 6 $
// $Workfile: Communication.cpp $
//
// *****************************************************************************

#include "TIC/Types.h"
#include "Communication.h"
#include "TIC/Communications/Communication_ABC.h"
#include "TIC/Communications/CommunicationManager.h"

namespace TICExport
{

//-----------------------------------------------------------------------------
// Name: Communication::Communication
// Created: SBO 2005-05-25
//-----------------------------------------------------------------------------
Communication::Communication()
: pCom_ ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: Communication::Communication
// Created: SBO 2005-05-25
//-----------------------------------------------------------------------------
Communication::Communication( const TIC::Communication_ABC& com )
: pCom_ ( &com )
{
}

//-----------------------------------------------------------------------------
// Name: Communication::~Communication
// Created: SBO 2005-05-25
//-----------------------------------------------------------------------------
Communication::~Communication()
{
}

//-----------------------------------------------------------------------------
// Name: Communication::GetArity
// Created: SBO 2005-05-25
//-----------------------------------------------------------------------------
uint Communication::GetArity() const
{
    assert( pCom_ );
    return pCom_->GetArity();
}

//-----------------------------------------------------------------------------
// Name: Communication::GetType
// Created: SBO 2005-05-25
//-----------------------------------------------------------------------------
const std::string& Communication::GetType() const
{
    assert( pCom_ );
    return pCom_->GetType().GetType();
}

//-----------------------------------------------------------------------------
// Name: Communication::GetSubType
// Created: SBO 2005-07-12
//-----------------------------------------------------------------------------
const std::string& Communication::GetSubType() const
{
    assert( pCom_ );
    return pCom_->GetType().GetSubType();
}

//-----------------------------------------------------------------------------
// Name: Communication::GetMessage
// Created: SBO 2005-07-01
//-----------------------------------------------------------------------------
const std::string& Communication::GetMessage() const
{
    assert( pCom_ );
    return pCom_->GetMessage();
}

//-----------------------------------------------------------------------------
// Name: Communication::IsReport
// Created: SBO 2005-07-01
//-----------------------------------------------------------------------------
bool Communication::IsReport() const
{
    assert( pCom_ );
    return pCom_->IsReport();
}

} // namespace TICExport