// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-14 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Missions_Data.h $
// $Author: Ape $
// $Modtime: 7/04/05 11:21 $
// $Revision: 3 $
// $Workfile: ADN_Missions_Data.h $
//
// *****************************************************************************

#ifndef __ADN_Missions_Data_h_
#define __ADN_Missions_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"

class ADN_XmlInput_Helper;


// =============================================================================
/** @class  ADN_Missions_Data
    @brief  ADN_Missions_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_Missions_Data
: public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_Missions_Data )
public:
    //! @name Constructors/Destructor
    //@{
    ADN_Missions_Data();
    ~ADN_Missions_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( T_StringList& vFiles ) const;
    void Reset();
    //@}

private:
    void ReadArchive( ADN_XmlInput_Helper& input );
    void WriteArchive( MT_OutputArchive_ABC& output );

public:
    ADN_Type_Enum<E_AutomataMission, eNbrAutomataMission> nSurrenderMission_;
    ADN_Type_Enum<E_AutomataMission, eNbrAutomataMission> nGoToRefugeeCampMission_;
};

#endif // __ADN_Missions_Data_h_
