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
    std::string GetName() const;
    std::string GetCategory() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DotationType( const DotationType& );            //!< Copy constructor
    DotationType& operator=( const DotationType& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    std::string name_;
    std::string category_;
    //@}
};

#endif // __DotationType_h_
