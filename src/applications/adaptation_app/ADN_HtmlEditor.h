// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_HtmlEditor_h_
#define __ADN_HtmlEditor_h_

#include "clients_gui/HtmlEditor.h"
#include "ADN_Gfx_ABC.h"

// =============================================================================
/** @class  ADN_HtmlEditor
    @brief  ADN_HtmlEditor
*/
// Created: NPT 2012-07-27
// =============================================================================
class ADN_HtmlEditor : public gui::HtmlEditor
                     , public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    ADN_HtmlEditor( int optionFlags = eAllMask );
    virtual ~ADN_HtmlEditor();
    //@}

    //! @name Operations
    //@{
    QString text() const;
    void setText( const QString text );
    //@}

private slots:
    //! @name slots
    //@{
    void OnTextChanged();
    virtual void Warn( ADN_ErrorStatus errorStatus, const QString& errorMessage = "" );
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __ADN_HtmlEditor_h_
