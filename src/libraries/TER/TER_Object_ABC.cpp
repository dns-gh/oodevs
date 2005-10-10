// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_Object_ABC.cpp $
// $Author: Jvt $
// $Modtime: 23/03/05 18:25 $
// $Revision: 2 $
// $Workfile: TER_Object_ABC.cpp $
//
// *****************************************************************************

#include "ter_pch.h"

#include "TER_Object_ABC.h"

#include "TER_World.h"
#include "TER_ObjectManager.h"

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC constructor
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_Object_ABC::TER_Object_ABC()
    : hint_( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC destructor
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_Object_ABC::~TER_Object_ABC()
{
    RemoveFromWorld();
}


// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void TER_Object_ABC::load( boost::archive::binary_iarchive& file, const uint )
{
    file >> location_;
    InsertInWorld();
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void TER_Object_ABC::save( boost::archive::binary_oarchive& file, const uint ) const
{
    file << location_;
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::Initialize
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_Object_ABC::Initialize( const TER_Localisation& localisation )
{
    location_.Reset( localisation );
    InsertInWorld();
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::Terminate
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_Object_ABC::Terminate()
{
    RemoveFromWorld();
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::IsInside
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
bool TER_Object_ABC::IsInside( const MT_Vector2D& vPos ) const
{
    return location_.IsInside( vPos );
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::Intersect2D
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
bool TER_Object_ABC::Intersect2D( const MT_Line& orientedLine, T_PointSet& collisions ) const
{
    return location_.Intersect2D( orientedLine, collisions );
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::Intersect2DWithCircle
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
bool TER_Object_ABC::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius ) const
{
    return location_.Intersect2DWithCircle( vCircleCenter, rRadius );
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::GetLocalisation
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
const TER_Localisation& TER_Object_ABC::GetLocalisation() const
{
    return location_;
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::UpdateLocalisation
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_Object_ABC::UpdateLocalisation( const TER_Localisation& newLocalisation )
{
    RemoveFromWorld();
    location_.Reset( newLocalisation );
    InsertInWorld();
    
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::RemoveFromWorld
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_Object_ABC::RemoveFromWorld()
{
    TER_World::GetWorld().GetObjectManager().Remove( *this, hint_ );
    hint_ = 0;
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::InsertInWorld
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_Object_ABC::InsertInWorld()
{
    hint_ = TER_World::GetWorld().GetObjectManager().UpdatePosition( *this, hint_ );
}
    
