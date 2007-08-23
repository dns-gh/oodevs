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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_HumanFactors_Data.h $
// $Author: Ape $
// $Modtime: 18/03/05 18:08 $
// $Revision: 2 $
// $Workfile: ADN_HumanFactors_Data.h $
//
// *****************************************************************************

#ifndef __ADN_HumanFactors_Data_h_
#define __ADN_HumanFactors_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"

class xml::xistream;


// =============================================================================
/** @class  ADN_HumanFactors_Data
    @brief  ADN_HumanFactors_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_HumanFactors_Data
: public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_HumanFactors_Data )

public:
// *****************************************************************************
    class ModifiersInfo
    {
        MT_COPYNOTALLOWED( ModifiersInfo )
    public:
        ModifiersInfo( const std::string& strName );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        const std::string strName_;

        ADN_Type_Double rSpeedModifier_;
        ADN_Type_Double rPHModifier_;
        ADN_Type_Double rSensorsModifier_;
        ADN_Type_Double rReloadModifier_;
        ADN_Type_Double rStanceModifier_;
    };


// *****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
    ADN_HumanFactors_Data();
    ~ADN_HumanFactors_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( T_StringList& vFiles ) const;
    void Reset();
    //@}

private:
    void ReadArchive( xml::xistream& input );
    void ReadModifier( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

public:
    ModifiersInfo veteranModifiers_;
    ModifiersInfo xpModifiers_;
    ModifiersInfo newbieModifiers_;
    ModifiersInfo normalModifiers_;
    ModifiersInfo tiredModifiers_;
    ModifiersInfo exhaustedModifiers_;
};

#endif // __ADN_HumanFactors_Data_h_
