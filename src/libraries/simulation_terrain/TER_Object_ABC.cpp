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

#include "simulation_terrain_pch.h"
#include "TER_Object_ABC.h"
#include "TER_World.h"
#include "TER_ObjectManager.h"

const double TER_Object_ABC::epsilon_ = 1e-8;

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC constructor
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_Object_ABC::TER_Object_ABC()
    : hint_( 0 )
{
    // NOTHING
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
// Name: TER_Object_ABC::Initialize
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_Object_ABC::Initialize( const TER_Localisation& localisation, double pointSize )
{
    location_.Reset( localisation, pointSize );
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
    return location_.IsInside( vPos, epsilon_ );
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::IsOnBorder
// Created: CMA 2012-03-14
// -----------------------------------------------------------------------------
bool TER_Object_ABC::IsOnBorder( const MT_Vector2D& vPos ) const
{
    return location_.IsOnBorder( vPos, epsilon_ );
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::Intersect2D
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
bool TER_Object_ABC::Intersect2D( const MT_Line& orientedLine, T_PointSet& collisions ) const
{
    return location_.Intersect2D( orientedLine, collisions, epsilon_ );
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::Intersect2DWithCircle
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
bool TER_Object_ABC::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius ) const
{
    return location_.Intersect2DWithCircle( vCircleCenter, rRadius );
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::Intersect2DWithLocalisation
// Created: NLD 2005-11-09
// -----------------------------------------------------------------------------
bool TER_Object_ABC::Intersect2DWithLocalisation( const TER_Localisation& localisation ) const
{
    return location_.IsIntersecting( localisation, epsilon_ );
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::IsContainedByLocalisation
// Created: BCI 2011-02-24
// -----------------------------------------------------------------------------
bool TER_Object_ABC::IsContainedByLocalisation( const TER_Localisation& localisation ) const
{
    return localisation.Contains( location_, epsilon_ );
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
// Name: TER_Object_ABC::GetLocalisation
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_Localisation TER_Object_ABC::GetLocalisation()
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
    if( hint_ )
    {
        TER_World::GetWorld().GetObjectManager().Remove( *this, hint_ );
        hint_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::InsertInWorld
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_Object_ABC::InsertInWorld()
{
    hint_ = TER_World::GetWorld().GetObjectManager().UpdatePosition( *this, hint_ );
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::GetPrecision
// Created: LDC 2013-11-06
// -----------------------------------------------------------------------------
double TER_Object_ABC::GetPrecision() const
{
    return epsilon_;
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::IsUniversal
// Created: JSR 2011-01-07
// -----------------------------------------------------------------------------
bool TER_Object_ABC::IsUniversal() const
{
    return false;
}
