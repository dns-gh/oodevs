// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __UrbanMagicAction_h_
#define __UrbanMagicAction_h_

#include "ActionWithTarget_ABC.h"

namespace kernel
{
    class Controller;
}

namespace simulation
{

    class UrbanMagicAction;
}
namespace actions
{

// =============================================================================
/** @class  UrbanMagicAction
    @brief  UrbanMagicAction
*/
// Created: JSR 2010-04-02
// =============================================================================
class UrbanMagicAction : public ActionWithTarget_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanMagicAction( const kernel::Entity_ABC& object, kernel::Controller& controller, bool registered = true );
             UrbanMagicAction( xml::xistream& xis, kernel::Controller& controller, const kernel::Entity_ABC& object );
    virtual ~UrbanMagicAction();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Polish();
    virtual void Publish( Publisher_ABC& publisher, int context ) const;

    void SetStructuralState( int state );
    void SetThreshold( float threshold );
    void SetEnabled( bool enabled );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    bool registered_;
    std::pair< bool, int > state_;
    std::pair< bool, float > threshold_;
    std::pair< bool, bool > active_;
    //@}
};

}

#endif // __UrbanMagicAction_h_
