// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __LanguageController_h_
#define __LanguageController_h_

#include "tools/SortedInterfaceContainer.h"
#include "tools/Observer_ABC.h"
#include "tools/LanguageObserver_ABC.h"
#include <boost/noncopyable.hpp>

namespace tools
{
    class Language;
}

namespace kernel
{

// =============================================================================
/** @class  LanguageController
    @brief  LanguageController
*/
// Created: ABR 2013-10-15
// =============================================================================
class LanguageController : private tools::SortedInterfaceContainer< tools::Observer_ABC >
                         , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LanguageController();
    virtual ~LanguageController();
    //@}

    //! @name Operations
    //@{
    void Register  ( tools::Observer_ABC& observer );
    void Unregister( tools::Observer_ABC& observer );

    void ChangeLanguage( const std::string& language );
    //@}
};

} //! namespace kernel

#endif // __LanguageController_h_
