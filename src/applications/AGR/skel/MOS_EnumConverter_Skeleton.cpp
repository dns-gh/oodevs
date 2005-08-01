// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGR $TIME$ $
// $Archive: /MVW_v10/Build/SDK/AGR/src/MOS_EnumConverter_Skeleton.cpp $
// $Author: Age $
// $Modtime: 22/10/04 17:12 $
// $Revision: 1 $
// $Workfile: MOS_EnumConverter_Skeleton.cpp $
//
// *****************************************************************************

#include "MOS_pch.h"
#include "MOS_EnumConverter.h"

// -----------------------------------------------------------------------------
// Name: MOS_EnumConverter::AutomataMissionToString
// Created: AGR $TIME$
// -----------------------------------------------------------------------------
QString MOS_EnumConverter::AutomataMissionToString( unsigned int nAutomataMission )
{
    QObject a;
    switch( nAutomataMission )
    {
$AutomataMissionToString$
        default:
            return a.tr( "Unknown" );
    }
}
    
// -----------------------------------------------------------------------------
// Name: MOS_EnumConverter::AutomataMissionToId
// Created: AGR $TIME$
// -----------------------------------------------------------------------------
unsigned int MOS_EnumConverter::AutomataMissionToId( const std::string& strMission )
{
$AutomataMissionToId$
    return (unsigned int)( -1 );
}

// -----------------------------------------------------------------------------
// Name: MOS_EnumConverter::UnitMissionToString
// Created: AGR $TIME$
// -----------------------------------------------------------------------------
QString MOS_EnumConverter::UnitMissionToString( unsigned int nUnitMission )
{
    QObject a;
    switch( nUnitMission )
    {
$UnitMissionToString$
        default:
            return a.tr( "Unknown" );
    }
}
    
// -----------------------------------------------------------------------------
// Name: MOS_EnumConverter::UnitMissionToId
// Created: AGR $TIME$
// -----------------------------------------------------------------------------
unsigned int MOS_EnumConverter::UnitMissionToId( const std::string& strMission )
{
$UnitMissionToId$
    return (unsigned int)( -1 );
}

// -----------------------------------------------------------------------------
// Name: MOS_EnumConverter::FragOrderToString
// Created: AGR $TIME$
// -----------------------------------------------------------------------------
QString MOS_EnumConverter::FragOrderToString( unsigned int nFragOrder )
{
    QObject a;
    switch( nFragOrder )
    {
$FragOrderToString$
        default:
            return a.tr( "Unknown" );
    }
}
    
// -----------------------------------------------------------------------------
// Name: MOS_EnumConverter::FragOrderToId
// Created: AGR $TIME$
// -----------------------------------------------------------------------------
unsigned int MOS_EnumConverter::FragOrderToId( const std::string& strFragOrder )
{
$FragOrderToId$
    return (unsigned int)( -1 );
}
