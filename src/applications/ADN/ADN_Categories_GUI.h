//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Categories_GUI.h $
// $Author: Nld $
// $Modtime: 27/04/05 15:56 $
// $Revision: 8 $
// $Workfile: ADN_Categories_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Categories_GUI_h_
#define __ADN_Categories_GUI_h_

#include "ADN_GUI_ABC.h"
#include <qobject.h>

class ADN_Categories_Data;
class ADN_ListView_Categories_Armor;
class ADN_ListView_Categories_Size;


// =============================================================================
/** @class  ADN_Categories_GUI
    @brief  ADN_Categories_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Categories_GUI 
: public ADN_GUI_ABC
{
    Q_OBJECT

public:
    enum E_ArmorGuiElements
    {
        eArmorName,
        eArmorType,
        eNeutralizationAverage,
        eNeutralizationVariance,
        eBreakdownEVA,
        eBreakdownNEVA,
        eWoundedEvac,
        eWondedNoEvac,
        eNbrArmorGuiElements
    };

    enum E_SizeGuiElements
    {
        eSizeName,
        eNbrSizeGuiElements
    };

public:
     ADN_Categories_GUI( ADN_Categories_Data& data );
    ~ADN_Categories_GUI();

    void Build();

public slots:    
    void IncArmor();
    void DecArmor();
    void IncSize();
    void DecSize();

private:
    ADN_Categories_Data& data_;

    ADN_ListView_Categories_Armor*  pListArmor_;
    ADN_ListView_Categories_Size*   pListSize_;
};


#endif // __ADN_Categories_GUI_h_