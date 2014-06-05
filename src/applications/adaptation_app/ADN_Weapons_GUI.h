// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Weapons_GUI_h_
#define __ADN_Weapons_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Connector_ABC;
class ADN_Weapons_Data;
class ADN_Table;
class ADN_Weapons_ListView;

// =============================================================================
/** @class  ADN_Weapons_GUI
    @brief  ADN_Weapons_GUI
*/
// Created: APE 2004-12-07
// =============================================================================
class ADN_Weapons_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

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
        eSimulation,
        eFirePosture,
        eTargetPosture,
        eExperience,
        eTiredness,
        eStress,
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
    explicit ADN_Weapons_GUI( ADN_Weapons_Data& data );
    virtual ~ADN_Weapons_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    void RegisterTable( ADN_MainWindow& mainWindow );

    QWidget* CreateWeaponsTable();
    QWidget* CreatePHTable();

    void UpdateModifiers();

    void ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const tools::Path& strPath );
    //@}

private slots:
    //! @name Operations
    //@{
    void ModifiersChanged( bool );
    void ModifiersChanged( int );
    //@}

private:
    //! @name Member data
    //@{
    ADN_Weapons_Data& data_;
    std::unique_ptr< ADN_Connector_ABC > graphConnector_;
    //@}
};

#endif // __ADN_Weapons_GUI_h_
