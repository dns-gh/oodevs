// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __BreakdownType_h_
#define __BreakdownType_h_

namespace xml{ class xistream; };

namespace kernel
{

// =============================================================================
/** @class  BreakdownType
    @brief  BreakdownType
*/
// Created: AGE 2006-04-05
// =============================================================================
class BreakdownType
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit BreakdownType( xml::xistream& xis );
    virtual ~BreakdownType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    std::string   GetName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BreakdownType( const BreakdownType& );            //!< Copy constructor
    BreakdownType& operator=( const BreakdownType& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    std::string name_;
    //@}
};

}

#endif // __BreakdownType_h_
