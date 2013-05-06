// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_TimeEdit_h_
#define __ADN_TimeEdit_h_

#include "ADN_Gfx_ABC.h"

// =============================================================================
/** @class  ADN_TimeEdit
    @brief  ADN_TimeEdit
*/
// Created: ABR 2012-11-05
// =============================================================================
class ADN_TimeEdit : public QTimeEdit
                   , public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_TimeEdit( QWidget* parent = 0, const QString& format = "hh:mm" );
    virtual ~ADN_TimeEdit();
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
    void TimeChanged( const QTime& time );
    void UpdateEnableState();
    //@}

private:
    //! @name Member data
    //@{
    const QString format_;
    //@}
};

#endif // __ADN_TimeEdit_h_