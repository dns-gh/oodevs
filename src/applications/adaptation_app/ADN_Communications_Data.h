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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Communications_Data.h $
// $Author: Ape $
// $Modtime: 18/03/05 18:08 $
// $Revision: 2 $
// $Workfile: ADN_Communications_Data.h $
//
// *****************************************************************************

#ifndef __ADN_Communications_Data_h_
#define __ADN_Communications_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"

class ADN_XmlInput_Helper;


// =============================================================================
/** @class  ADN_Communications_Data
    @brief  ADN_Communications_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_Communications_Data : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_Communications_Data )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_Communications_Data();
    ~ADN_Communications_Data();
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
    ADN_Type_Double rSpeedModifier_;
    ADN_Type_Double rReloadModifier_;
};

#endif // __ADN_Communications_Data_h_
