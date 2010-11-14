// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DotationType_h_
#define __DotationType_h_

namespace xml { class xistream; };

namespace kernel
{

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
    explicit DotationType( xml::xistream& xis );
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
    bool IsDType() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DotationType( const DotationType& );            //!< Copy constructor
    DotationType& operator=( const DotationType& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::string   name_;
    unsigned long id_;
    unsigned long categoryId_;
    std::string   category_;
    bool          gaz_, ammunition_, dType_;
    //@}
};

}

#endif // __DotationType_h_
