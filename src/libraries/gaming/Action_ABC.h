// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Action_ABC_h_
#define __Action_ABC_h_

#include "game_asn/asn.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class Displayer_ABC;
}

class ActionParameter_ABC;

// =============================================================================
/** @class  Action_ABC
    @brief  Action_ABC
*/
// Created: SBO 2007-03-12
// =============================================================================
class Action_ABC : public kernel::Resolver< ActionParameter_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Action_ABC( kernel::Controller& controller );
    virtual ~Action_ABC();
    //@}

    //! @name Operations
    //@{
    virtual unsigned long GetId() const;
    virtual QString GetName() const = 0;
    virtual const kernel::Entity_ABC& GetEntity() const = 0;
    virtual void AddParameter( ActionParameter_ABC& parameter );
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Action_ABC( const Action_ABC& );            //!< Copy constructor
    Action_ABC& operator=( const Action_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    static unsigned long idManager_;
    unsigned long id_;
    //@}
};

#endif // __Action_ABC_h_
