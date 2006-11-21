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

#include "LogisticHierarchies_ABC.h"
#include "clients_kernel/LogisticHierarchies.h"

// =============================================================================
/** @class  LogisticHierarchies
    @brief  LogisticHierarchies
*/
// Created: SBO 2006-10-24
// =============================================================================
template< typename Superior, typename I >
class LogisticHierarchies : public LogisticHierarchies_ABC< I >
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticHierarchies( kernel::Controller& controller, kernel::Entity_ABC& entity, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver );
    virtual ~LogisticHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual void Load( xml::xistream& xis, const kernel::Automat_ABC* superior );
    virtual const kernel::Entity_ABC* GetSuperior() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticHierarchies( const LogisticHierarchies& );            //!< Copy constructor
    LogisticHierarchies& operator=( const LogisticHierarchies& ); //!< Assignement operator
    //@}

protected:
    //! @name Helpers
    //@{
    void SetSuperior( const Superior& automat );
    virtual void SetSuperiorInternal( kernel::Entity_ABC* superior );
    //@}

protected:
    //! @name Member data
    //@{
    Superior tc2_;
    //@}
};

#include "LogisticHierarchies.inl"

#endif // __LogisticHierarchies_h_
