// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticHierarchies_ABC_h_
#define __LogisticHierarchies_ABC_h_

#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class PropertiesDictionary;
}

namespace xml
{
    class xostream;
    class xistream;
}

// =============================================================================
/** @class  LogisticHierarchies_ABC
    @brief  LogisticHierarchies_ABC
*/
// Created: AGE 2006-11-21
// =============================================================================
template< typename I >
class LogisticHierarchies_ABC : public kernel::EntityHierarchies< I > 
                              , public kernel::Serializable_ABC
                              , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticHierarchies_ABC( kernel::Controller& controller, kernel::Entity_ABC& entity );
    virtual ~LogisticHierarchies_ABC();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Load( xml::xistream& ) {};
    virtual void SerializeLogistics( xml::xostream& xos ) const;
    virtual void SerializeQuotas( xml::xostream& ) const {};
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticHierarchies_ABC( const LogisticHierarchies_ABC& );            //!< Copy constructor
    LogisticHierarchies_ABC& operator=( const LogisticHierarchies_ABC& ); //!< Assignement operator
    //@}
};

#include "LogisticHierarchies_ABC.inl"

#endif // __LogisticHierarchies_ABC_h_
