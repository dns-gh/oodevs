// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPerceptions_h_
#define __ObjectPerceptions_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include <tools/Resolver_ABC.h>

namespace sword
{
    class ObjectKnowledgeUpdate;
}

namespace kernel
{
    class Automat_ABC;
    class Controller;
}

// =============================================================================
/** @class  ObjectPerceptions
    @brief  ObjectPerceptions
*/
// Created: AGE 2006-10-16
// =============================================================================
class ObjectPerceptions : public kernel::Extension_ABC
                        , public kernel::Updatable_ABC< sword::ObjectKnowledgeUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectPerceptions( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& agentResolver );
    virtual ~ObjectPerceptions();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectPerceptions( const ObjectPerceptions& );            //!< Copy constructor
    ObjectPerceptions& operator=( const ObjectPerceptions& ); //!< Assignment operator
    //@}

public:
    //! @name Types
    //@{
    typedef std::vector< const kernel::Automat_ABC* > T_Agents;
    typedef T_Agents::const_iterator                CIT_Agents;
    //@}

    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& agentResolver_;
public: // $$$$ AGE 2006-10-16:
    T_Agents detectingAutomats_;
    //@}
};

#endif // __ObjectPerceptions_h_
