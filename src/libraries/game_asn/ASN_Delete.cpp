// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "ASN_Delete.h"

//=============================================================================
// MISC TOOLS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: ASN_Delete::Delete
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void ASN_Delete::Delete( ASN1T_MissionObjectiveList& asn )
{
    if( asn.n > 0 )
    {
        for( unsigned int i = 0; i < asn.n; ++i )
            Delete( asn.elem[i] );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Delete::Delete
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void ASN_Delete::Delete( ASN1T_MissionObjective& asn )
{
    ASN_Delete::Delete( asn.localisation );
}

// -----------------------------------------------------------------------------
// Name: ASN_Delete::Delete
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void ASN_Delete::Delete( ASN1T_PlannedWorkList& asn )
{
    if( asn.n > 0 )
    {
        for( unsigned int i = 0; i < asn.n; ++i )
            Delete( asn.elem[i] );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Delete::Delete
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void ASN_Delete::Delete( ASN1T_PlannedWork& asn )
{
    ASN_Delete::Delete( asn.position );
}

// -----------------------------------------------------------------------------
// Name: ASN_Delete::Delete
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
void ASN_Delete::Delete( ASN1T_LogMedicalPriorities& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Delete::Delete
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
void ASN_Delete::Delete( ASN1T_LogMaintenancePriorities& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Delete::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Delete::Delete( ASN1T_UnitList& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Delete::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Delete::Delete( ASN1T_AutomatList& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Delete::Delete
// Created: AGE 2007-07-06
// -----------------------------------------------------------------------------
void ASN_Delete::Delete( ASN1T_CoordLatLongList& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Delete::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Delete::Delete( ASN1T_Polygon& asn )
{
    if( asn.coordinates.n > 0 )
        delete [] asn.coordinates.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Delete::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Delete::Delete( ASN1T_PolygonList& asn )
{
    if( asn.n > 0 )
    {
        for( unsigned int i = 0; i < asn.n; ++i )
            Delete( asn.elem[ i ] );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Delete::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Delete::Delete( ASN1T_PointList& asn )
{
    if( asn.n > 0 )
    {
        for( unsigned int i = 0; i < asn.n; ++i )
            Delete( asn.elem[ i ] );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Delete::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void ASN_Delete::Delete( ASN1T_PathList& asn )
{
    if( asn.n > 0 )
    {
        for( unsigned int i = 0; i < asn.n; ++i )
            Delete( asn.elem[ i ] );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Delete::Delete
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Delete::Delete( ASN1T_LocationList& asn )
{
    if( asn.n > 0 )
    {
        for( unsigned int i = 0; i < asn.n; ++i )
            Delete( asn.elem[i] );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: ASN_Delete::Delete
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Delete::Delete( ASN1T_UnitKnowledgeList& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: ASN_Delete::Delete
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void ASN_Delete::Delete( ASN1T_ObjectKnowledgeList& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}
