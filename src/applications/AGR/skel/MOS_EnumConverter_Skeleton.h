// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-10-22 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/MOS_EnumConverter_Skeleton.h $
// $Author: Age $
// $Modtime: 25/10/04 11:34 $
// $Revision: 1 $
// $Workfile: MOS_EnumConverter_Skeleton.h $
//
// *****************************************************************************

#ifndef __MOS_EnumConverter_h_
#define __MOS_EnumConverter_h_

enum E_AutomataMission
{
$AutomateMissionEnumList$
    eAutomataMissionNbr
};

enum E_UnitMission
{
$PionMissionEnumList$
    eUnitMissionNbr
};

enum E_PopulationMission
{
$PopulationMissionEnumList$
    ePopulationMissionNbr
};

enum E_FragOrder
{
$OrderConduiteEnumList$
    eFragOrderNbr
};

// =============================================================================
/** @class  MOS_EnumConverter
    @brief  Converts enumerated IDs from and to strings
*/
// Created: AGE 
// =============================================================================
class MOS_EnumConverter
{

public:
    //! @name Operations
    //@{
    static QString       AutomataMissionToString( unsigned int nAutomataMission );
    static unsigned int  AutomataMissionToId    ( const std::string& strMission );

    static QString       UnitMissionToString( unsigned int nUnitMission );
    static unsigned int  UnitMissionToId    ( const std::string& strMission );

    static QString       PopulationMissionToString( unsigned int nPopulationMission );
    static unsigned int  PopulationMissionToId    ( const std::string& strMission );

    static QString       FragOrderToString( unsigned int nFragOrder );
    static unsigned int  FragOrderToId    ( const std::string& strFragOrder );
    //@}
};

#endif // __MOS_EnumConverter_h_
