// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Armors_GUI_h_
#define __ADN_Armors_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Armors_Data;
class ADN_Categories_AttritionEffect_Table;
class ADN_ComboBox;

// =============================================================================
/** @class  ADN_Armors_GUI
    @brief  ADN_Armors_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Armors_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    //! @name Types
    //@{
    enum E_ArmorGuiElements
    {
        eArmorName,
        eArmorType,
        eNeutralizationAverage,
        eNeutralizationVariance,
        eBreakdownEVA,
        eBreakdownNEVA,
        eAttritionEffects,
        eNbrArmorGuiElements
    };
    //@}

    //! @name Constructors/Destructor
    //@{
    explicit ADN_Armors_GUI( ADN_Armors_Data& data );
    virtual ~ADN_Armors_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

public slots:
    //! @name Slots
    //@{
    void OnTypeChanged( int index );
    void OnItemSelected( void* );
    //@}

private:
    //! @name Member data
    //@{
    ADN_Armors_Data& data_;

    ADN_Categories_AttritionEffect_Table* pAttritionEffects_;
    ADN_ComboBox* pComboType_;
    Q3GroupBox* pArmorBreakdownGroup_;
    Q3GroupBox* pAttritionEffectGroup_;
    //@}
};

#endif // __ADN_Armors_GUI_h_
