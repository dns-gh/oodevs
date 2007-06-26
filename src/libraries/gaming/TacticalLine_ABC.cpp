// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TacticalLine_ABC.h"
#include "TacticalLinePositions.h"
#include "TacticalLineHierarchies.h"
#include "clients_kernel/GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( const QString& baseName, unsigned long id, Publisher_ABC& publisher )
    : publisher_    ( publisher )
    , id_           ( id )
{
    RegisterSelf( *this );
    name_ = ( baseName + " %1" ).arg( id_ & 0x3FFFFF );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC destructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC::~TacticalLine_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Create
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Create()
{
    UpdateToSim( eStateCreated );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Delete
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Delete()
{
    UpdateToSim( eStateDeleted );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::GetId
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
unsigned long TacticalLine_ABC::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::GetName
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
QString TacticalLine_ABC::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::WriteGeometry
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::WriteGeometry( ASN1T_Line& line ) const
{
    // $$$$ SBO 2006-11-06: visitor or something
    static_cast< const TacticalLinePositions& >( Get< kernel::Positions >() ).WriteGeometry( line );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::WriteGeometry
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void TacticalLine_ABC::WriteGeometry( kernel::Location_ABC& location ) const
{
    // $$$$ SBO 2006-11-06: visitor or something
    static_cast< const TacticalLinePositions& >( Get< kernel::Positions >() ).WriteGeometry( location );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::WriteDiffusion
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLine_ABC::WriteDiffusion( ASN1T_TacticalLinesDiffusion& diffusion ) const
{
    // $$$$ SBO 2006-11-06: visitor or something
    static_cast< const TacticalLineHierarchies& >( Get< kernel::TacticalHierarchies >() ).WriteTo( diffusion );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::DoUpdate
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLine_ABC::DoUpdate( const ASN1T_MsgLimaUpdate& message )
{
    name_ = message.tactical_line.name;
}
    
// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::DoUpdate
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLine_ABC::DoUpdate( const ASN1T_MsgLimitUpdate& message )
{
    name_ = message.tactical_line.name;
}
