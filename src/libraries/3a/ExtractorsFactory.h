// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ExtractorsFactory_h_
#define __ExtractorsFactory_h_

#include "ElementFactory_ABC.h"

namespace aar
{
    class StaticModel_ABC;
}

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
    explicit ExtractorsFactory( const aar::StaticModel_ABC& model );
    virtual ~ExtractorsFactory();
    //@}

    //! @name Operations
    //@{
    virtual void CreateElement( const std::string& type, xml::xistream& xis, Task& result ) const;
    //@}

private:
    //! @name Member data
    //@{
    const aar::StaticModel_ABC& model_;
    //@}
};

#endif // __ExtractorsFactory_h_
