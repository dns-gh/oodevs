// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-02 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Logger.h $
// $Author: Ape $
// $Modtime: 21/07/04 15:32 $
// $Revision: 3 $
// $Workfile: Logger.h $
//
// *****************************************************************************

#ifndef __Logger_h_
#define __Logger_h_

#ifdef __GNUG__
#   pragma interface
#endif


// =============================================================================
/** @class  Logger
    @brief  Captures log messages and displays them in a sorted list. 
*/
// Created: APE 2004-06-02
// =============================================================================
class Logger : public QListView, public MT_Logger_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( Logger );

public:
    //! @name Constructors/Destructor
    //@{
     Logger( QWidget* pParent );
    ~Logger();
    //@}

protected:
    //! @name Operations
    //@{
    void LogString( const char* szLayerName, E_LogLevel nLevel, const char* szMsg, const char* szContext, int nCode );

    QSize sizeHint() const;
    //@}

private slots:
    void OnRequestPopup( QListViewItem* pItem, const QPoint& pos );

private:
    QPopupMenu popupMenu_;
};

#   include "Logger.inl"

#endif // __Logger_h_
