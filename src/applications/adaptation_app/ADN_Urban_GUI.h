// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Urban_GUI_h_
#define __ADN_Urban_GUI_h_

#include "ADN_GUI_ABC.h"
#include <qobject.h>

class ADN_Table;
class ADN_Urban_Data;
class ADN_ListView_Urban_Type;
class ADN_ListView_UrbanMaterial_Type;
class ADN_Urban_AttritionTable;

// =============================================================================
/** @class  ADN_Categories_GUI
    @brief  ADN categories gui
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Urban_GUI
    : public ADN_GUI_ABC
{
    Q_OBJECT

public:

    enum E_UrbanGuiElements
    {
        eUrbanName,
        eNbrUrbanGuiElements
    };

    enum E_UrbanMaterialGuiElements
    {
        eUrbanMaterialName,
        eUrbanMaterialAttrition,
        eNbrUrbanMaterialGuiElements
    };

public:
    explicit ADN_Urban_GUI( ADN_Urban_Data& data );
    virtual ~ADN_Urban_GUI();

    void Build();

private:
    //! @name helpers
    //@{
    //ADN_Table* CreatePKTable();
    //@}

private:
    ADN_Urban_Data& data_;

    ADN_ListView_UrbanMaterial_Type*   pListMaterial_;
    ADN_ListView_Urban_Type*           pListFacade_;
    ADN_ListView_Urban_Type*           pListRoofShape_;

    ADN_Urban_AttritionTable*     pAttritionTable_;
//    ADN_Equipement_AttritionTable*     pAttritionTable_;
};


#endif // __ADN_Urban_GUI_h_
