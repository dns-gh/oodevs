// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AltitudeModifierAttribute_h_
#define __AltitudeModifierAttribute_h_

#include "clients_kernel/ObjectExtensions.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
}

// =============================================================================
/** @class  AltitudeModifierAttribute
    @brief  AltitudeModifierAttribute
*/
// Created: JSR 2011-05-17
// =============================================================================
class AltitudeModifierAttribute : public kernel::AltitudeModifierAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AltitudeModifierAttribute( kernel::Controller& controller );
    virtual ~AltitudeModifierAttribute();
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
    AltitudeModifierAttribute( const AltitudeModifierAttribute& );            //!< Copy constructor
    AltitudeModifierAttribute& operator=( const AltitudeModifierAttribute& ); //!< Assignment operator
    //@}

    //! @name Updatable_ABC
    //@{
    virtual void DoUpdate( const sword::ObjectUpdate& message );
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );

    template< typename T >
    void UpdateData( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    unsigned int height_;
    //@}
};

#endif // __AltitudeModifierAttribute_h_
