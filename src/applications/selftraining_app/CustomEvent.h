// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CustomEvent_h_
#define __CustomEvent_h_

#include <QtCore/qcoreevent.h>

class CustomEvent : public QEvent
{
public:

    //! @name Constructors/Destructor
    //@{
             CustomEvent( int type, void *data = 0 );
    virtual ~CustomEvent();
    //@}

    //! @name Operations
    //@{
    void* GetData() const;
    void  SetData( void* aData );
    //@}

private:
    //! @name Member data
    //@{
    void* data_;
    //@}
};
#endif // __CustomEvent_h_
