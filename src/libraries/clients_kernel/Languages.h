// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef _kernel_Languages_h
#define _kernel_Languages_h

#include "Language.h"
#include <boost/noncopyable.hpp>

namespace tools
{
    class Path;
}

namespace kernel
{

// =============================================================================
/** @class  Languages
    @brief  Languages
*/
// Created: ABR 2013-09-30
// =============================================================================
class Languages : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Languages( const tools::Path& xmlFile );
    explicit Languages( xml::xistream& xis );
    virtual ~Languages();
    //@}

    //! @name Operations
    //@{
    const Language& Get( const std::string& code ) const;
    const LanguagesVector& GetLanguages() const;
    //@}

private:
    //! @name Helpers
    //@{
    void Read( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    LanguagesVector languages_;
    //@}
};

} //! namespace kernel

#endif // _kernel_Languages_h
