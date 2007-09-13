// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetType
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
inline
const MIL_MissionType_ABC& MIL_Mission_ABC::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetDirDanger
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MIL_Mission_ABC::GetDirDanger() const
{
    return context_.GetDirDanger();
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetFuseau
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
const MIL_Fuseau& MIL_Mission_ABC::GetFuseau() const
{
    return context_.GetFuseau();
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::AffectFuseau
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
inline
void MIL_Mission_ABC::AffectFuseau( const MIL_Fuseau& fuseau )
{
    context_.AffectFuseau( fuseau );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetLimas
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
const T_LimaVector& MIL_Mission_ABC::GetLimas() const
{
    return context_.GetLimas();
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::FindLima
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
MIL_LimaOrder* MIL_Mission_ABC::FindLima( uint nID )
{
    return context_.FindLima( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::FindLima
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
MIL_LimaOrder* MIL_Mission_ABC::FindLima( const MIL_LimaFunction& function )
{
    return context_.FindLima( function );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::FindNextScheduledLima
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
MIL_LimaOrder* MIL_Mission_ABC::FindNextScheduledLima()
{
    return context_.FindNextScheduledLima();
}
