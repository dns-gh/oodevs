// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionParameter_h_
#define __AfterActionParameter_h_

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  AfterActionParameter
    @brief  AfterActionParameter
*/
// Created: AGE 2007-09-24
// =============================================================================
class AfterActionParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AfterActionParameter( xml::xistream& xis );
    virtual ~AfterActionParameter();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionParameter( const AfterActionParameter& );            //!< Copy constructor
    AfterActionParameter& operator=( const AfterActionParameter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    std::string type_;
    //@}
};

#endif // __AfterActionParameter_h_
