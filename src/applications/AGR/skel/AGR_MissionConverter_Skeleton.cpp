// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: 
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionConverter_Skeleton.cpp $
// $Author: Agn $
// $Modtime: 23/04/04 16:42 $
// $Revision: 1 $
// $Workfile: AGR_MissionConverter_Skeleton.cpp $
//
// *****************************************************************************

MIL_Tools::converter< E_MissionID > MIL_Tools::missionConverter_[] = 
{
$MissionConverterList$
    converter< E_MissionID >( ""                                            , (E_MissionID)-1 )
};
