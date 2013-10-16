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

#include <tools/Resolver_ABC.h>

namespace xml { class xistream; };

namespace kernel
{
    class DecisionalModel;

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
             PopulationType( xml::xistream& xis, const  tools::Resolver_ABC< DecisionalModel, std::string >& modelResolver );
    virtual ~PopulationType();
    //@}

    //! @name Operations
    //@{
    std::string GetName() const;
    unsigned long GetId() const;

    float GetDensity() const;
    float GetArmedIndividuals() const;
    float GetMale() const;
    float GetFemale() const;
    float GetChildren() const;

    const DecisionalModel& GetDecisionalModel() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationType( const PopulationType& );            //!< Copy constructor
    PopulationType& operator=( const PopulationType& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    unsigned long id_;
    const DecisionalModel* model_;
    float density_;
    float armedIndividuals_;
    float male_;
    float female_;
    float children_;
    //@}
};

}

#endif // __PopulationType_h_
