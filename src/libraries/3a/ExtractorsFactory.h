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
#include <map>
#include <boost/bind.hpp>

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
    //! @name Types
    //@{
    typedef std::function< void( const std::string&, xml::xistream&, Task& ) > T_Extractor;
    typedef std::map< std::string, T_Extractor > T_Extractors;
    //@}

private:
    //! @name Member data
    //@{
    const T_Extractors extractors_;
    //@}
};

#endif // __ExtractorsFactory_h_
