// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ColorNameSelector_h_
#define __ADN_ColorNameSelector_h_

#include "ADN_Gfx_ABC.h"

// =============================================================================
/** @class  ADN_ColorNameSelector
    @brief  ADN color name selector
*/
// Created: LGY 2011-09-22
// =============================================================================
class ADN_ColorNameSelector : public QPushButton
                            , public ADN_Gfx_ABC
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_ColorNameSelector( QWidget* parent );
    virtual ~ADN_ColorNameSelector();
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

#endif // __ADN_ColorNameSelector_h_
