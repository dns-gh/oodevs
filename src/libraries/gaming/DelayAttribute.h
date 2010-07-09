// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __DelayAttribute_h_
#define __DelayAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace MsgsSimToClient
{
    class MsgObjectUpdate;
    class MsgObjectCreation;
}

namespace kernel
{
    class Controller;
    class Displayer_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  DelayAttribute
    @brief  DelayAttribute
*/
// Created: JSR 2010-07-06
// =============================================================================
class DelayAttribute : public kernel::DelayAttribute_ABC
                     , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectUpdate >
                     , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectCreation >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DelayAttribute( kernel::Controller& controller );
    virtual ~DelayAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DelayAttribute( const DelayAttribute& );            //!< Copy constructor
    DelayAttribute& operator=( const DelayAttribute& ); //!< Assignment operator
    //@}

    //! @name Updatable_ABC
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectUpdate& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectCreation& message );
    template< typename T >
    void UpdateData( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    unsigned int delay_;
    //@}
};

#endif // __DelayAttribute_h_
