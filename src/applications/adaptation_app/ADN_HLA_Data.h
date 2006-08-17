// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_HLA_Data.h $
// $Author: Ape $
// $Modtime: 22/03/05 12:06 $
// $Revision: 1 $
// $Workfile: ADN_HLA_Data.h $
//
// *****************************************************************************

#ifndef __ADN_HLA_Data_h_
#define __ADN_HLA_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"

class ADN_XmlInput_Helper;


// =============================================================================
/** @class  ADN_HLA_Data
    @brief  ADN_HLA_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_HLA_Data
    : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_HLA_Data )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_HLA_Data();
    ~ADN_HLA_Data();
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
    ADN_Type_Bool   bUseHLA_;
    ADN_Type_String strFederation_;
    ADN_Type_String strFederate_;
};

#endif // __ADN_HLA_Data_h_
