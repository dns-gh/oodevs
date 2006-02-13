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
// $Archive: /MVW_v10/Build/SDK/Light2/src/RecorderToolbar.h $
// $Author: Ape $
// $Modtime: 21/10/04 10:40 $
// $Revision: 1 $
// $Workfile: RecorderToolbar.h $
//
// *****************************************************************************

#ifndef __RecorderToolbar_h_
#define __RecorderToolbar_h_

#ifdef __GNUG__
#   pragma interface
#endif

class MsgRecorder;


// =============================================================================
/** @class  RecorderToolbar
    @brief  RecorderToolbar
    @par    Using example
    @code
    RecorderToolbar;
    @endcode
*/
// Created: APE 2004-10-20
// =============================================================================
class RecorderToolbar : public QToolBar
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( RecorderToolbar );

public:
    //! @name Constructors/Destructor
    //@{
     RecorderToolbar( QMainWindow* pParent );
    ~RecorderToolbar();
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
    MsgRecorder& msgRecorder_;

    QToolButton* pPlayButton_;
    QToolButton* pStopButton_;
    QToolButton* pRecButton_;
    //@}
};

#endif // __RecorderToolbar_h_
