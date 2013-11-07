// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __RichDateTimeEdit_h_
#define __RichDateTimeEdit_h_

#include "RichWarnWidget.h"

namespace gui
{

// =============================================================================
/** @class  RichDateTimeEdit
    @brief  RichDateTimeEdit
*/
// Created: ABR 2011-11-21
// =============================================================================
class RichDateTimeEdit : public RichWarnWidget< QWidget >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichDateTimeEdit( const QString& objectName, QWidget* parent = 0 );
    virtual ~RichDateTimeEdit();
    //@}

    //! @name Operations
    //@{
    virtual void Warn( int msec = 3000 );
    virtual void OnDone();

    QDate date() const;
    QDateTime dateTime() const;
    QTime time() const;

    void setMinimumDateTime( const QDateTime& dateTime );
    void setMaximumDateTime( const QDateTime& dateTime );
    void setDateTimeRange( const QDateTime& min, const QDateTime& max );
    //@}

public slots:
    //! @name Slots
    //@{
    void setDate( const QDate& date );
    void setDateTime( const QDateTime& dateTime );
    void setTime( const QTime& time );
    //@}

signals:
    //! @name 
    //@{
    void dateChanged( const QDate& date );
    void dateTimeChanged( const QDateTime& datetime );
    void timeChanged( const QTime& time );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnDateChanged( const QDate& date );
    void OnTimeChanged( const QTime& time );
    //@}


private:
    //! @name Member data
    //@{
    RichWarnWidget< QDateEdit >* date_;
    RichWarnWidget< QTimeEdit >* time_;
    QDateTime dateTime_;
    //@}
};

} //! namespace gui

#endif // __RichDateTimeEdit_h_
