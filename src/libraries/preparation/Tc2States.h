// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Tc2States_h_
#define __Tc2States_h_

#include "Types.h"
#include "LogisticHierarchies.h"

// =============================================================================
/** @class  Tc2States
    @brief  Tc2States
*/
// Created: SBO 2006-10-26
// =============================================================================
class Tc2States : public LogisticHierarchies< TC2 >
{

public:
    //! @name Constructors/Destructor
    //@{
             Tc2States( kernel::Controller& controller, kernel::Entity_ABC& entity, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver, kernel::PropertiesDictionary& dico );
    virtual ~Tc2States();
    //@}

    //! @name Operations
    //@{
    void SetSuperior( const TC2& automat );
    virtual QString GetLinkType() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Tc2States( const Tc2States& );            //!< Copy constructor
    Tc2States& operator=( const Tc2States& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}
};

#endif // __Tc2States_h_
