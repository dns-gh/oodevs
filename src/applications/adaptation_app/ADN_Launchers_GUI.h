// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Launchers_GUI_h_
#define __ADN_Launchers_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_Launchers_Data.h"

class ADN_ListView_Launchers;
class ADN_Launchers_ModifPhs_GUI;
class ADN_HtmlBuilder;

// =============================================================================
/** @class  ADN_Launchers_GUI
    @brief  ADN_Launchers_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Launchers_GUI
: public ADN_GUI_ABC
{
public:
    enum E_GuiElements
    {
        eName,
        ePhModifiers,
        eDirect,
        eIndirect,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
     ADN_Launchers_GUI( ADN_Launchers_Data& data );
    virtual ~ADN_Launchers_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

    void ExportPHModifiers( ADN_HtmlBuilder& builder, ADN_Launchers_Data::LauncherInfos& infos );

private:
    ADN_Launchers_Data& data_;
    ADN_ListView_Launchers* pListView_;
    ADN_Launchers_ModifPhs_GUI* pModifPhs_;
};


#endif // __ADN_Launchers_GUI_h_
