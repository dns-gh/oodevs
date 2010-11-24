// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __KernelFacadeType_h_
#define __KernelFacadeType_h_

#include <string>

namespace xml
{
    class xistream;
}

namespace kernel
{
// =============================================================================
/** @class  FacadeType
    @brief  FacadeType
*/
// Created: SLG 2010-11-17
// =============================================================================
class FacadeType
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FacadeType( xml::xistream& xis );
    virtual ~FacadeType();
    //@}

    //! @name Operations
    //@{
    unsigned int GetId() const;
    const std::string& GetName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FacadeType( const FacadeType& );            //!< Copy constructor
    FacadeType& operator=( const FacadeType& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    std::string name_;
    static unsigned int nNextID_;
    //@}
};

}

#endif // FacadeType