// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InhabitantType_h_
#define __InhabitantType_h_

#include "tools/Resolver_ABC.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class PopulationType;

// =============================================================================
/** @class  InhabitantType
    @brief  InhabitantType
*/
// Created: SLG 2010-11-23
// =============================================================================
class InhabitantType
{
public:
    //! @name Constructors/Destructor
    //@{
             InhabitantType( xml::xistream& xis, const  tools::Resolver_ABC< PopulationType, std::string >& modelResolver );
    virtual ~InhabitantType();
    //@}

    //! @name Operations
    //@{
    const std::string& GetName() const;
    unsigned long GetId() const;
    float GetMalePercentage() const;
    float GetFemalePercentage() const;
    float GetChildrenPercentage() const;
    unsigned int GetHealthPeopleNumber() const;
    const PopulationType& GetCrowdType() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InhabitantType( const InhabitantType& );            //!< Copy constructor
    InhabitantType& operator=( const InhabitantType& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    unsigned long id_;
    const PopulationType* type_;
    float male_;
    float female_;
    float children_;
    unsigned int healthNeed_;
    //@}
};

}

#endif // __PopulationType_h_
