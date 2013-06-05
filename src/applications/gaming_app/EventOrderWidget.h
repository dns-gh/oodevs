// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventOrderWidget_h_
#define __EventOrderWidget_h_

#include "EventWidget_ABC.h"

// =============================================================================
/** @class  EventOrderWidget
    @brief  EventOrderWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventOrderWidget : public EventWidget_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             EventOrderWidget();
    virtual ~EventOrderWidget();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name EventWidget_ABC implementation
    //@{
    virtual void Fill( const Event& event );
    virtual void Commit( timeline::Event& event ) const;
    virtual bool IsValid() const;
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __EventOrderWidget_h_
