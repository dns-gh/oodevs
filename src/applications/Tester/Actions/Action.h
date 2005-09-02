// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-05 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Action_h_
#define __Action_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Action_ABC.h"
#include "Tools/IDManager.h"

namespace TEST {

    class Scheduler;

// =============================================================================
/** @class  Action
    @brief  Action
    @par    Using example
    @code
    Action;
    @endcode
*/
// Created: SBO 2005-08-05
// =============================================================================
template< typename T >
class Action : public Action_ABC
{

public:
    static IDManager idManager_;

public:
    //! @name Constructors/Destructor
    //@{
             Action();
             Action( T& target );
    virtual ~Action();
    //@}

    //! @name Operations
    //@{
    virtual bool Run( const Scheduler& scheduler );
    //@}

    //! @name Modifiers
    //@{
    virtual void SetTarget( T& target );
    //@}

protected:
    //! @name Action validation and commit
    //@{
    virtual void Serialize(                            ) = 0;
    virtual void Send     ( const Scheduler& scheduler ) = 0;
    virtual void Clean    (                            ) = 0;
    //@}

protected:
    //! @name Member data
    //@{
    T*   pTarget_;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Action( const Action& );            //!< Copy constructor
    Action& operator=( const Action& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#include "Action.inl"

#endif // __Action_h_
