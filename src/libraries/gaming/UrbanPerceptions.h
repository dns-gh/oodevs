// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanPerceptions_h_
#define __UrbanPerceptions_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Automat_ABC;
    class Controller;
}

namespace sword
{
    class UrbanKnowledgeUpdate;
}

// =============================================================================
/** @class  UrbanPerceptions
    @brief  UrbanPerceptions
*/
// Created: MGD 2009-12-09
// =============================================================================
class UrbanPerceptions : public kernel::Extension_ABC
                       , public kernel::Updatable_ABC< sword::UrbanKnowledgeUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanPerceptions( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& agentResolver );
    virtual ~UrbanPerceptions();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::UrbanKnowledgeUpdate& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanPerceptions( const UrbanPerceptions& );            //!< Copy constructor
    UrbanPerceptions& operator=( const UrbanPerceptions& ); //!< Assignment operator
    //@}

public:
    //! @name Types
    //@{
    typedef std::vector< const kernel::Automat_ABC* > T_Agents;
    typedef T_Agents::const_iterator                CIT_Agents;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& agentResolver_;
    //@}

public:
    //! @name Member data
    //@{
    T_Agents detectingAutomats_;
    //@}
};

#endif // __UrbanPerceptions_h_
