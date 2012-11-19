// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_NBC_Intox_GUI_h_
#define __ADN_NBC_Intox_GUI_h_

#include "ADN_Gfx_ABC.h"
#include "ADN_Connector_ABC.h"

// =============================================================================
/** @class  ADN_NBC_Intox_GUI
    @brief  ADN_NBC_Intox_GUI
*/
// Created: SBO 2006-10-30
// =============================================================================
class ADN_NBC_Intox_GUI : public Q3VBox
                        , public ADN_Gfx_ABC
{
    Q_OBJECT
public:
    enum E_GuiElements
    {
        eIntoxPresent,
        eNbrOk,
        eNbrHurt1,
        eNbrHurt2,
        eNbrHurt3,
        eNbrHurt4,
        eNbrDead,
        eContaminationPresent,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_NBC_Intox_GUI( QWidget* pParent, const QString& objectName );
    virtual ~ADN_NBC_Intox_GUI();
    //@}

private:
    //! @name Member data
    //@{
    T_ConnectorVector vInfosConnectors_;
    //@}

};

#endif // __ADN_NBC_Intox_GUI_h_
