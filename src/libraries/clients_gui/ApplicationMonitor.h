// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef APPLICATION_MONITOR_H__
#define APPLICATION_MONITOR_H__

#include <QtGui/QApplication>

namespace gui
{
// =============================================================================
/** @class  ApplicationMonitor
    @brief  ApplicationMonitor
*/
// Created: BAX 2012-10-29
// =============================================================================
class ApplicationMonitor : public QApplication
{
    Q_OBJECT

public:
    //! @name Constructor/Destructor
    //@{
     ApplicationMonitor( int& argc, char** argv );
    ~ApplicationMonitor();
    //@}

protected:
    //! @name QApplication methods
    //@{
    virtual bool notify( QObject* receiver, QEvent* event );
    //@}

signals:
    //! @name Signals
    //@{
    virtual void Error( const QString& );
    //@}
};
}

#endif // APPLICATION_MONITOR_H__
