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
/** @class  LogisticHierarchies
    @brief  LogisticHierarchies
*/
// Created: SBO 2006-10-24
// =============================================================================
template< typename Superior, typename ConcreteHierarchy >
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
    void Load( xml::xistream& xis, const Superior& superior );
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
    void ReadDotation( xml::xistream& xis );
    //@}

protected:
    //! @name Helpers
    //@{
    void SetSuperior( const Superior& automat );
    void CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& owner, const QString& name );
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
    DotationsItem* item_;
    Superior tc2_;
    //@}
};

#include "LogisticHierarchies.inl"

#endif // __LogisticHierarchies_h_
