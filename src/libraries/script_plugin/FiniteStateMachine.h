// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FiniteStateMachine_h_
#define __FiniteStateMachine_h_

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace directia
{
    class Brain;
    class ScriptRef;
}

namespace plugins
{
namespace script
{

class Condition_ABC;
class Event;

// =============================================================================
/** @class  FiniteStateMachine
    @brief  Finite state machine
*/
// Created: AGE 2008-06-12
// =============================================================================
class FiniteStateMachine
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit FiniteStateMachine( directia::Brain& brain );
    virtual ~FiniteStateMachine();
    //@}

    //! @name Operations
    //@{
    void UsedByDIA    () {};
    void ReleasedByDIA() {};
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FiniteStateMachine( const FiniteStateMachine& );            //!< Copy constructor
    FiniteStateMachine& operator=( const FiniteStateMachine& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void DeclareEvent( boost::shared_ptr< Condition_ABC >, const std::vector< std::string >& states, const directia::ScriptRef& function );
    std::string CurrentState() const;
    void ChangeState( const std::string& );
    void Deactivate();
    //@}

private:
    //! @name Member data
    //@{
    std::string state_;
    boost::ptr_vector< Event > events_;
    //@}
};

}
}

#endif // __FiniteStateMachine_h_
