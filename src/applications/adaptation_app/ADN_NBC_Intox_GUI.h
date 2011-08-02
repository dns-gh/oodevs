// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_NBC_Intox_GUI_h_
#define __ADN_NBC_Intox_GUI_h_

#include "ADN_Gfx_ABC.h"
#include "ADN_Connector_ABC.h"
#include <Qt3Support/q3vbox.h>

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
    explicit ADN_NBC_Intox_GUI( QWidget* pParent );
    virtual ~ADN_NBC_Intox_GUI();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_NBC_Intox_GUI( const ADN_NBC_Intox_GUI& );            //!< Copy constructor
    ADN_NBC_Intox_GUI& operator=( const ADN_NBC_Intox_GUI& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    T_ConnectorVector vInfosConnectors_;
    //@}

};

#endif // __ADN_NBC_Intox_GUI_h_
