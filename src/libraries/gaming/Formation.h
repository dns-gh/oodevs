// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_h_
#define __Formation_h_

#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/EntityImplementation.h"
#include "tools/Resolver_ABC.h"

namespace sword
{
    class FormationCreation;
    class LogisticLevel;
}

namespace kernel
{
    class StaticModel;
}

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: AGE 2006-10-19
// =============================================================================
class Formation : public kernel::EntityImplementation< kernel::Formation_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Formation( const sword::FormationCreation& message, kernel::Controller& controller,
                 const tools::Resolver_ABC< kernel::HierarchyLevel_ABC >& resolver, const kernel::StaticModel& staticModel );
    virtual ~Formation();
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::HierarchyLevel_ABC& GetLevel() const;
    virtual const kernel::LogisticLevel& GetLogisticLevel() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Formation( const Formation& );            //!< Copy constructor
    Formation& operator=( const Formation& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::HierarchyLevel_ABC& level_;
    const kernel::LogisticLevel* logisticLevel_;
    //@}
};

#endif // __Formation_h_
