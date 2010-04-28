// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-07 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Weapons_GUI.h $
// $Author: Nld $
// $Modtime: 3/05/05 9:51 $
// $Revision: 9 $
// $Workfile: ADN_Weapons_GUI.h $
//
// *****************************************************************************

#ifndef __ADN_Weapons_GUI_h_
#define __ADN_Weapons_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Weapons_Data;
class ADN_Table;
class ADN_Weapons_ListView;

// =============================================================================
/** @class  ADN_Weapons_GUI
    @brief  ADN_Weapons_GUI
*/
// Created: APE 2004-12-07
// =============================================================================
class ADN_Weapons_GUI 
: public ADN_GUI_ABC
{
    Q_OBJECT

    MT_COPYNOTALLOWED( ADN_Weapons_GUI )

public:
    enum E_GuiElements
    {
        eName,
        eLauncher,
        eAmmo,
        eRoundsPerBurst,
        eBurstDuration,
        eRoundsPerReload,
        eReloadDuration,
        ePhsGraph,
        ePhs,
        eAverageSpeed,
        eMinRange,
        eMaxRange,
        eDirect,
        eIndirect,
        eSimulation,    // LTO
        eFirePosture,   // LTO
        eTargetPosture, // LTO
        eExperience,    // LTO
        eTiredness,     // LTO
        eNbrGuiElements
    };

    enum E_PhSizeGuiElements
    {
        ePhVector,
        eNbrPhSizeGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
     ADN_Weapons_GUI( ADN_Weapons_Data& data );
    virtual ~ADN_Weapons_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    void RegisterTable( ADN_MainWindow& mainWindow );

    ADN_Table* CreateWeaponsTable();
    ADN_Table* CreatePHTable();

    void UpdateModifiers();

    void ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const QString& strPath );
    //@}

private slots:
    //! @name Operations
    //@{
    void ModifiersChanged( bool ); // LTO
    void ModifiersChanged( int ); // LTO
    //@}

private:
    //! @name Member data
    //@{
    ADN_Weapons_Data& data_;
    ADN_Weapons_ListView* pWeaponList_;
    //@}
};


#endif // __ADN_Weapons_GUI_h_
