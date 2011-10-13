// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_DateEdit_h_
#define __ADN_DateEdit_h_

#include "ADN_Gfx_ABC.h"

// =============================================================================
/** @class  ADN_DateEdit
    @brief  ADN_DateEdit
*/
// Created: JSR 2011-10-13
// =============================================================================
class ADN_DateEdit : public QDateEdit
                   , public ADN_Gfx_ABC
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_DateEdit( QWidget* parent );
    virtual ~ADN_DateEdit();
    //@}

    //! @name Operations
    //@{
    virtual void setEnabled( bool b );
    virtual QString text() const;
    virtual void setText( const QString& text );
    //@}

private slots:
    //! @name Slots
    //@{
    void DateChanged( const QDate& date );
    void UpdateEnableState();
    //@}

private:
    //! @name Member data
    //@{
    static const QString strFormat_;
    //@}
};

#endif // __ADN_DateEdit_h_
