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

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace kernel
{

// =============================================================================
/** @class  DotationType
    @brief  DotationType
*/
// Created: AGE 2006-02-21
// =============================================================================
class DotationType : private boost::noncopyable
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

    const std::string& GetName() const; // $$$$ AGE 2006-04-28: category and name are kind of mixed up
    const std::string& GetCategory() const;
    const std::string& GetType() const;

    bool IsGas() const;
    bool IsAmmunition() const;
    bool IsDType() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    unsigned long id_;
    unsigned long categoryId_;
    std::string category_;
    std::string type_;
    bool gaz_;
    bool ammunition_;
    bool dType_;
    //@}
};

}

#endif // __DotationType_h_
