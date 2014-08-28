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

// =============================================================================
/** @class  ADN_HumanFactors_Data
    @brief  ADN_HumanFactors_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_HumanFactors_Data : public ADN_Data_ABC
{

public:
    // =============================================================================
    // ModifiersInfo
    // =============================================================================
    class ModifiersInfo
    {
    public:
        explicit ModifiersInfo( const std::string& strName );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        const std::string strName_;

        ADN_Type_Double rSpeedModifier_;
        ADN_Type_Double rPHModifier_;
        ADN_Type_Double rSensorsModifier_;
        ADN_Type_Double rReloadModifier_;
        ADN_Type_Double rStanceModifier_;
    };

    // =============================================================================
    // ThresholdsInfo
    // =============================================================================
    class ThresholdsInfo
    {
    public:
        ThresholdsInfo();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Int firstThreshold_;
        ADN_Type_Int secondThreshold_;
    };

// =============================================================================
// ADN_HumanFactors_Data
// =============================================================================
public:
    //! @name Constructors/Destructor
    //@{
             ADN_HumanFactors_Data();
    virtual ~ADN_HumanFactors_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( tools::Path::T_Paths& vFiles ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadArchive( xml::xistream& input );
    void ReadModifier( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;
    //@}

public:
    // Modifiers
    ModifiersInfo   veteranModifiers_;
    ModifiersInfo   xpModifiers_;
    ModifiersInfo   newbieModifiers_;
    ModifiersInfo   normalModifiers_;
    ModifiersInfo   tirednessModifiers_;
    ModifiersInfo   exhaustedModifiers_;
    ModifiersInfo   calmModifiers_;
    ModifiersInfo   worriedModifiers_;
    ModifiersInfo   stressedModifiers_;
    // Evolution
    ThresholdsInfo  tirednessThresholds_;
    ADN_Type_Int    tirednessMoving_;
    ADN_Type_Int    tirednessWorking_;
    ADN_Type_Int    tirednessEngineStopped_;
    ADN_Type_Int    tirednessEngineRunning_;
    ThresholdsInfo  stressThresholds_;
    ADN_Type_Int    stressIncPerShot_;
    ADN_Type_Int    stressDecPerHour_;
};

#endif // __ADN_HumanFactors_Data_h_
