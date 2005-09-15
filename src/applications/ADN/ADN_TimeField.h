// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-09 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __ADN_TimeField_h_
#define __ADN_TimeField_h_

#include "ADN_Connector_ABC.h"
#include "ADN_Gfx_ABC.h"
#include <qwidget.h>

class QLineEdit;
class QComboBox;

// =============================================================================
/** @class  ADN_TimeField
    @brief  ADN_TimeField
    @par    Using example
    @code
    ADN_TimeField;
    @endcode
*/
// Created: SBO 2005-09-09
// =============================================================================
class ADN_TimeField 
    : public QWidget
    , public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_TimeField( QWidget* pParent, const char* szName = 0 );
    virtual ~ADN_TimeField();
    //@}

    //! @name Operations
    //@{
    QString text         () const;
    void    setText      ( const QString& strText );
    void    focusInEvent ( QFocusEvent* pEvent );
    void    focusOutEvent( QFocusEvent* pEvent );
    //@}

public slots:
    //! @name Internal events
    //@{
    void OnValueChanged   ( const QString& strValue );
    void OnUnitChanged    ( const QString& strUnit  );

private slots:
    void UpdateEnableState();
    //@}

private:
    //! @name Member data
    //@{
    QLineEdit*         pLineEdit_;
    QComboBox*         pComboBox_;
    uint               nSecondsValue_;
    bool               bFreezeSlot_;
    //@}
};

#endif // __ADN_TimeField_h_
