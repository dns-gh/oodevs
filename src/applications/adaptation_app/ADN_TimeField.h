// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_TimeField_h_
#define __ADN_TimeField_h_

#include "ADN_Connector_ABC.h"
#include "ADN_Gfx_ABC.h"
#include <QtGui/qwidget.h>

class ADN_TimeField_EditLine;
class QComboBox;
class ADN_IntValidator;

// =============================================================================
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
    QString text() const;
    ADN_IntValidator& GetValidator();
    void setText( const QString& strText );
    void SetMinimumValueInSecond( unsigned int value );
    //@}

signals:
    //! @name Signals
    //@{
    void ValueChanged();
    //@}

private slots:
    //! @name Internal events
    //@{
    void OnValueChanged( const QString& strValue );
    void OnUnitChanged ( const QString& strUnit  );
    void UpdateEnableState();
    //@}

private:
    //! @name Member data
    //@{
    ADN_TimeField_EditLine* pLineEdit_;
    QComboBox*              pComboBox_;
    unsigned int            nSecondsValue_;
    unsigned int            nMinimumSecondsValue_;
    unsigned int            nMinimumMinutesValue_;
    unsigned int            nMinimumHoursValue_;
    bool                    bFreezeSlot_;
    ADN_IntValidator*          pValidator_;
    //@}
};

#endif // __ADN_TimeField_h_
