// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UndergroundAttribute_h_
#define __UndergroundAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
}

// =============================================================================
/** @class  UndergroundAttribute
    @brief  UndergroundAttribute
*/
// Created: JSR 2011-07-07
// =============================================================================
class UndergroundAttribute : public kernel::UndergroundAttribute_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit UndergroundAttribute( kernel::Controller& controller );
    virtual ~UndergroundAttribute();
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetNetwork() const;
    virtual bool IsActivated() const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UndergroundAttribute( const UndergroundAttribute& );            //!< Copy constructor
    UndergroundAttribute& operator=( const UndergroundAttribute& ); //!< Assignment operator
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
    std::string network_;
    bool activated_;
    //@}
};

#endif // __UndergroundAttribute_h_
