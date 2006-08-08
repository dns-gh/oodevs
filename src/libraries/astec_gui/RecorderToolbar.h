// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RecorderToolbar_h_
#define __RecorderToolbar_h_

class MsgRecorder;
class Network;

// =============================================================================
/** @class  RecorderToolbar
    @brief  RecorderToolbar
*/
// Created: APE 2004-10-20
// =============================================================================
class RecorderToolbar : public QToolBar
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             RecorderToolbar( QMainWindow* pParent, Network& network );
    virtual ~RecorderToolbar();
    //@}

private slots:
    //! @name Slots
    //@{
    void Play();
    void Stop();
    void Record();
    //@}

private:
    //! @name Helpers
    //@{
    RecorderToolbar( const RecorderToolbar& );
    RecorderToolbar& operator=( const RecorderToolbar& );
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
