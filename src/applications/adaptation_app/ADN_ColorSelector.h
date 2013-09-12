// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ColorSelector_h_
#define __ADN_ColorSelector_h_

#include "ADN_Gfx_ABC.h"

// =============================================================================
/** @class  ADN_ColorSelector
    @brief  ADN_ColorSelector
*/
// Created: JSR 2010-09-14
// =============================================================================
class ADN_ColorSelector : public QPushButton
                        , public ADN_Gfx_ABC
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_ColorSelector( QWidget* parent, bool fixedSize = true );
    virtual ~ADN_ColorSelector();
    //@}

    //! @name Operations
    //@{
    void setText( const QString& string );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void paintEvent( QPaintEvent *e );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClick();
    virtual void Warn( ADN_ErrorStatus errorStatus, const QString& errorMessage = "" );
    //@}

private:
    //! @name Member data
    //@{
    QColor color_;
    //@}
};

#endif // __ADN_ColorSelector_h_
