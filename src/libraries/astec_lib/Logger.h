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

#include "Types.h"
#include "MT/MT_logger/MT_Logger_ABC.h"

class ItemFactory_ABC;

struct sLoggerLayer
{
    sLoggerLayer( E_DataFlow code, QColor color ) : code_ ( code ), color_ ( color ) {}
    E_DataFlow code_;
    QColor color_;
};

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

private:
    //! @name Copy constructor/assignement
    //@{
    Logger( const Logger &);
    Logger& operator=( const Logger& );
    //@}

    //! @name Types
    //@{
    typedef std::map< E_DataFlow, const sLoggerLayer* > T_Layers;
    typedef T_Layers::iterator                         IT_Layers;
    //@}

private:
    //! @name Member data
    //@{
    ItemFactory_ABC& factory_;
    QPopupMenu popupMenu_;
    T_Layers layers_;
    //@}
};

#endif // __Logger_h_
