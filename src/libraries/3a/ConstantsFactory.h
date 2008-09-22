// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ConstantsFactory_h_
#define __ConstantsFactory_h_

#include "ElementFactory_ABC.h"

// =============================================================================
/** @class  ConstantsFactory
    @brief  ConstantsFactory
*/
// Created: AGE 2008-08-04
// =============================================================================
class ConstantsFactory : public ElementFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ConstantsFactory();
    virtual ~ConstantsFactory();
    //@}

    //! @name Operations
    //@{
    virtual void CreateElement( const std::string& type, xml::xistream& xis, Task& result ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ConstantsFactory( const ConstantsFactory& );            //!< Copy constructor
    ConstantsFactory& operator=( const ConstantsFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void MakeConstant( const std::string& name, xml::xistream& xis, Task& result ) const;
    struct MakeConstantDispatcher;
    //@}
};

#endif // __ConstantsFactory_h_
