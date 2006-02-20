// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationType_h_
#define __PopulationType_h_

#include "Resolver_ABC.h"

class DecisionalModel;
namespace xml { class xistream; };

// =============================================================================
/** @class  PopulationType
    @brief  PopulationType
*/
// Created: AGE 2006-02-20
// =============================================================================
class PopulationType
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationType( xml::xistream& xis, const Resolver_ABC< DecisionalModel, std::string >& modelResolver );
    virtual ~PopulationType();
    //@}

    //! @name Operations
    //@{
    std::string GetName() const;
    unsigned long GetId() const;

    MT_Float GetDensity() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationType( const PopulationType& );            //!< Copy constructor
    PopulationType& operator=( const PopulationType& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    std::string   name_;
    unsigned long id_;
    const DecisionalModel* model_;

    MT_Float density_;
    //@}
};

#endif // __PopulationType_h_
