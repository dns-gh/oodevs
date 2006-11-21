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
#include "clients_kernel/Resolver.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class DotationType;
    class PropertiesDictionary;
}

namespace xml
{
    class xostream;
    class xistream;
}

class Dotation;
class DotationsItem;

// =============================================================================
/** @class  LogisticHierarchies_ABC
    @brief  LogisticHierarchies_ABC
*/
// Created: AGE 2006-11-21
// =============================================================================
template< typename I >
class LogisticHierarchies_ABC : public kernel::EntityHierarchies< I > 
                              , public kernel::Resolver< Dotation >
                              , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticHierarchies_ABC( kernel::Controller& controller, kernel::Entity_ABC& entity, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver );
    virtual ~LogisticHierarchies_ABC();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Entity_ABC* GetSuperior() const = 0;
    void Load( xml::xistream& xis );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void SetSuperiorInternal( kernel::Entity_ABC* superior ) = 0;
    virtual void CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& owner, const QString& name );
    virtual void SerializeLogistics( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticHierarchies_ABC( const LogisticHierarchies_ABC& );            //!< Copy constructor
    LogisticHierarchies_ABC& operator=( const LogisticHierarchies_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadDotation( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver_;
    DotationsItem* item_;
    //@}
};

#include "LogisticHierarchies_ABC.inl"

#endif // __LogisticHierarchies_ABC_h_
