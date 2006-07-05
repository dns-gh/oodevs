// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DotationType_h_
#define __DotationType_h_

namespace xml { class xistream; };

// =============================================================================
/** @class  DotationType
    @brief  DotationType
*/
// Created: AGE 2006-02-21
// =============================================================================
class DotationType
{

public:
    //! @name Constructors/Destructor
    //@{
             DotationType( const std::string& dotationName, xml::xistream& xis );
    virtual ~DotationType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    unsigned long GetFamily() const;

    std::string GetName() const; // $$$$ AGE 2006-04-28: category and name are kind of mixed up
    std::string GetCategory() const;

    bool IsGas() const;
    bool IsAmmunition() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DotationType( const DotationType& );            //!< Copy constructor
    DotationType& operator=( const DotationType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    unsigned long id_;
    unsigned long nameId_; // $$$$ AGE 2006-04-28: name == category
    std::string category_;
    bool gaz_, ammunition_;
    //@}
};

#endif // __DotationType_h_
