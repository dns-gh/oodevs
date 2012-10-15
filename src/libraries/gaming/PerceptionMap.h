// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PerceptionMap_h_
#define __PerceptionMap_h_

#include "Perception.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class Controller;
}

namespace sword
{
    class UnitKnowledgeUpdate;
}

// =============================================================================
/** @class  PerceptionMap
    @brief  PerceptionMap
*/
// Created: AGE 2006-02-22
// =============================================================================
class PerceptionMap : public kernel::Extension_ABC
                    , public kernel::Updatable_ABC< sword::UnitKnowledgeUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionMap( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& resolver );
    virtual ~PerceptionMap();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PerceptionMap( const PerceptionMap& );            //!< Copy constructor
    PerceptionMap& operator=( const PerceptionMap& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitKnowledgeUpdate& message );
    //@}

public:
    //! @name Types
    //@{
    typedef std::vector< Perception >       T_Perceptions;
    typedef T_Perceptions::const_iterator CIT_Perceptions;
    //@}

    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& resolver_;
    T_Perceptions perceptions_;
    //@}
};

#endif // __PerceptionMap_h_
