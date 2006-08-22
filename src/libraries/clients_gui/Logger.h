// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Logger_h_
#define __Logger_h_

#include "Types.h"
#include "MT/MT_logger/MT_Logger_ABC.h"

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  Logger
    @brief  Captures log messages and displays them in a sorted list. 
*/
// Created: APE 2004-06-02
// =============================================================================
class Logger : public QListView, public MT_Logger_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             Logger( QWidget* pParent, ItemFactory_ABC& factory );
    virtual ~Logger();
    //@}

protected:
    //! @name Operations
    //@{
    void LogString( const char* szLayerName, E_LogLevel nLevel, const char* szMsg, const char* szContext, int nCode );
    QSize sizeHint() const { return QSize( 400, 250 ); }
    //@}

private slots:
    //! @name Slots
    //@{
    void OnRequestPopup( QListViewItem* pItem, const QPoint& pos );
    //@}

signals:
    //! @name Signals
    //@{
    void Error();
    //@}

private:
    //! @name Copy constructor/assignement
    //@{
    Logger( const Logger &);
    Logger& operator=( const Logger& );
    //@}

private:
    //! @name Member data
    //@{
    ItemFactory_ABC& factory_;
    QPopupMenu popupMenu_;
    //@}
};

}

#endif // __Logger_h_
