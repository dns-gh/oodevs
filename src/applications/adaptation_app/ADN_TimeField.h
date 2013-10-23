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
class ADN_DoubleValidator;
class QSpinBox;

// $$$$ ABR 2012-11-05: This class should be rename in ADN_DelayEdit

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
    void setText( const QString& strText );
    void SetMinimumValueInSecond( unsigned int value );
    virtual void setObjectName( const QString& name );
    //@}

private slots:
    //! @name Internal events
    //@{
    void OnValueChanged();
    virtual void Warn( ADN_ErrorStatus errorStatus, const QString& errorMessage = "" );
    //@}

private:
    //! @name Helpers
    //@{
    int GetCentiSeconds( const QString& strValue, const QString& strUnit ) const;
    //@}

private:
    //! @name Member data
    //@{
    QSpinBox* pSpinBox_;
    //@}
};

#endif // __ADN_TimeField_h_
