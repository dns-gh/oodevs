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
    class xostream;
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
    std::string GetType() const;
    QString GetName() const;
    void Commit( xml::xostream& xos, const std::string& name ) const;
    void Set( const std::string& value );
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
    std::string value_;
    //@}
};

#endif // __AfterActionParameter_h_
