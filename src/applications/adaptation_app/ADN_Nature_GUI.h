// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Nature_GUI_h_
#define __ADN_Nature_GUI_h_

#include "ADN_Gfx_ABC.h"
#include "clients_gui/NatureEditionWidget.h"

// =============================================================================
/** @class  ADN_Nature_GUI
    @brief  ADN_Nature_GUI
    // $$$$ AGE 2006-10-24: Field. Whatever
*/
// Created: AGE 2006-10-24
// =============================================================================
class ADN_Nature_GUI : public gui::NatureEditionWidget
                     , public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Nature_GUI( QGridLayout* parent, int row = 0 );
    virtual ~ADN_Nature_GUI();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTextChanged( const QString& );
    void OnUnitSymbolChanged( const QString& nature );
    virtual void Warn( ADN_ErrorStatus errorStatus, const QString& errorMessage = "" );
    //@}
};

#endif // __ADN_Nature_GUI_h_
