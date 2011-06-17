// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_HumanFactors_Data_h_
#define __ADN_HumanFactors_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"

namespace xml { class xistream; }

// =============================================================================
/** @class  ADN_HumanFactors_Data
    @brief  ADN_HumanFactors_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_HumanFactors_Data : public ADN_Data_ABC
{

public:
// *****************************************************************************
    class ModifiersInfo
    {
    public:
        explicit ModifiersInfo( const std::string& strName );

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
    virtual ~ADN_HumanFactors_Data();
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
    ModifiersInfo calmModifiers_;
    ModifiersInfo worriedModifiers_;
    ModifiersInfo stressedModifiers_;
};

#endif // __ADN_HumanFactors_Data_h_
