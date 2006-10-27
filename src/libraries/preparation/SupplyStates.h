// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyStates_h_
#define __SupplyStates_h_

#include "Types.h"
#include "LogisticHierarchies.h"

// =============================================================================
/** @class  SupplyStates
    @brief  SupplyStates
*/
// Created: SBO 2006-10-24
// =============================================================================
class SupplyStates : public LogisticHierarchies< SupplySuperior >
{

public:
    //! @name Constructors/Destructor
    //@{
             SupplyStates( kernel::Controller& controller, kernel::Entity_ABC& entity, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver, kernel::PropertiesDictionary& dico );
    virtual ~SupplyStates();
    //@}

    //! @name Operations
    //@
    virtual QString GetLinkType() const;
    void SetSuperior( const SupplySuperior& automat );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SupplyStates( const SupplyStates& );            //!< Copy constructor
    SupplyStates& operator=( const SupplyStates& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}
};

#endif // __SupplyStates_h_
