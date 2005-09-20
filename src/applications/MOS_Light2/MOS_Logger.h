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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Logger.h $
// $Author: Ape $
// $Modtime: 21/07/04 15:32 $
// $Revision: 3 $
// $Workfile: MOS_Logger.h $
//
// *****************************************************************************

#ifndef __MOS_Logger_h_
#define __MOS_Logger_h_

#ifdef __GNUG__
#   pragma interface
#endif


// =============================================================================
/** @class  MOS_Logger
    @brief  Captures log messages and displays them in a sorted list. 
*/
// Created: APE 2004-06-02
// =============================================================================
class MOS_Logger : public QListView, public MT_Logger_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_Logger );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_Logger( QWidget* pParent );
    ~MOS_Logger();
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

#   include "MOS_Logger.inl"

#endif // __MOS_Logger_h_
