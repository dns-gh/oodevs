// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-22 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Composantes/PHY_ComposanteHLA.cpp $
// $Author: Age $
// $Modtime: 13/12/04 10:34 $
// $Revision: 3 $
// $Workfile: PHY_ComposanteHLA.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "PHY_ComposanteHLA.h"

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteHLA constructor
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
PHY_ComposanteHLA::PHY_ComposanteHLA( const PHY_ComposanteType_ABC& type, uint nElements )
    : type_        ( type )
    , nCurrentElements_   ( nElements )
    , nMaxElements_( nElements )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteHLA destructor
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
PHY_ComposanteHLA::~PHY_ComposanteHLA()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteHLA::Update
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
void PHY_ComposanteHLA::Update( uint nElements )
{
    nCurrentElements_ = nElements;
    if( nElements > nMaxElements_ )
        nMaxElements_ = nElements;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteHLA::GetType
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
const PHY_ComposanteType_ABC& PHY_ComposanteHLA::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteHLA::CanFire
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_ComposanteHLA::CanFire() const
{
    return IsActive();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteHLA::CanFireWhenUnloaded
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_ComposanteHLA::CanFireWhenUnloaded() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteHLA::GetMajorScore
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
uint PHY_ComposanteHLA::GetMajorScore() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteHLA::IsActive
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_ComposanteHLA::IsActive() const
{
    return nCurrentElements_ > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteHLA::CurrentElements
// Created: AGE 2004-12-13
// -----------------------------------------------------------------------------
uint PHY_ComposanteHLA::CurrentElements() const
{
    return nCurrentElements_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteHLA::MaxElements
// Created: AGE 2004-12-13
// -----------------------------------------------------------------------------
uint PHY_ComposanteHLA::MaxElements()
{
    return nMaxElements_;
}
