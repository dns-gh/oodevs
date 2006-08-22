// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MT_SpinBox_h_
#define __MT_SpinBox_h_

#include <qspinbox.h>

namespace gui
{
// =============================================================================
// Created: SBO 2005-11-07
// =============================================================================
class MT_SpinBox : public QSpinBox
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             MT_SpinBox( QWidget * parent = 0, const char* name = 0 );
             MT_SpinBox ( int minValue, int maxValue, int step = 1, QWidget * parent = 0, const char * name = 0 );
    virtual ~MT_SpinBox();
    //@}

signals:
    //! @name Signals
    //@{
    void enterPressed();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MT_SpinBox( const MT_SpinBox& );
    MT_SpinBox& operator=( const MT_SpinBox& );
    //@}

protected:
    //! @name Event filter
    //@{
    virtual bool eventFilter( QObject* pSender, QEvent* pEvent );
    //@}
};

}

#endif // __MT_SpinBox_h_
