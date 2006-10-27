// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticHierarchies_h_
#define __LogisticHierarchies_h_

#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/Resolver.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class DotationType;
}

namespace xml
{
    class xostream;
}

class Dotation;

// =============================================================================
/** @class  LogisticHierarchies
    @brief  LogisticHierarchies
*/
// Created: SBO 2006-10-24
// =============================================================================
template< typename Superior >
class LogisticHierarchies : public kernel::EntityHierarchies< kernel::TacticalHierarchies >
                          , public kernel::Resolver< Dotation >
                          , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticHierarchies( kernel::Controller& controller, kernel::Entity_ABC& entity, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver );
    virtual ~LogisticHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Entity_ABC* GetSuperior() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticHierarchies( const LogisticHierarchies& );            //!< Copy constructor
    LogisticHierarchies& operator=( const LogisticHierarchies& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual QString GetLinkType() const = 0;
    virtual void SerializeLogistics( xml::xostream& xos ) const;
    //@}

protected:
    //! @name Helpers
    //@{
    template< typename ConcreteHierarchy >
    void SetSuperior( const Superior& automat );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver_;
    //@}

protected:
    //! @name Member data
    //@{
    Superior tc2_;
    //@}
};

#include "LogisticHierarchies.inl"

#endif // __LogisticHierarchies_h_
