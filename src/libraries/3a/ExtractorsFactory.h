// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExtractorsFactory_h_
#define __ExtractorsFactory_h_

#include "ElementFactory_ABC.h"

// =============================================================================
/** @class  ExtractorsFactory
    @brief  Extractors factory
*/
// Created: AGE 2008-08-04
// =============================================================================
class ExtractorsFactory : public ElementFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ExtractorsFactory();
    virtual ~ExtractorsFactory();
    //@}

    //! @name Operations
    //@{
    virtual void CreateElement( const std::string& type, xml::xistream& xis, Task& result ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExtractorsFactory( const ExtractorsFactory& );            //!< Copy constructor
    ExtractorsFactory& operator=( const ExtractorsFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    static void Error( const std::string& name );
    template< typename Value >
    void Extract( const std::string& name, xml::xistream& xis, Task& result ) const;
    //@}
};

#endif // __ExtractorsFactory_h_
