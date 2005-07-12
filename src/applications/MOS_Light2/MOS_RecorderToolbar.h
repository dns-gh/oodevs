// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-10-20 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_RecorderToolbar.h $
// $Author: Ape $
// $Modtime: 21/10/04 10:40 $
// $Revision: 1 $
// $Workfile: MOS_RecorderToolbar.h $
//
// *****************************************************************************

#ifndef __MOS_RecorderToolbar_h_
#define __MOS_RecorderToolbar_h_

#ifdef __GNUG__
#   pragma interface
#endif

class MOS_MsgRecorder;


// =============================================================================
/** @class  MOS_RecorderToolbar
    @brief  MOS_RecorderToolbar
    @par    Using example
    @code
    MOS_RecorderToolbar;
    @endcode
*/
// Created: APE 2004-10-20
// =============================================================================
class MOS_RecorderToolbar : public QToolBar
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_RecorderToolbar );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_RecorderToolbar( QMainWindow* pParent );
    ~MOS_RecorderToolbar();
    //@}

    //! @name Operations
    //@{
    //@}

    //! @name Accessors
    //@{
    //@}

    //! @name Modifiers
    //@{
    //@}

private slots:
    //! @name Operators
    //@{
    void Play();
    void Stop();
    void Record();
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    MOS_MsgRecorder& msgRecorder_;

    QToolButton* pPlayButton_;
    QToolButton* pStopButton_;
    QToolButton* pRecButton_;
    //@}
};

#include "MOS_RecorderToolbar.inl"

#endif // __MOS_RecorderToolbar_h_
