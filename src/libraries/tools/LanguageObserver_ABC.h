// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef LanguageObserver_ABC_h
#define LanguageObserver_ABC_h

#include <tools/Observer_ABC.h>

namespace tools
{
    class Language;

// =============================================================================
/** @class  LanguageObserver_ABC
    @brief  LanguageObserver_ABC definition
*/
// Created: ABR 2013-10-15
// =============================================================================
class LanguageObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LanguageObserver_ABC() {}
    virtual ~LanguageObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void OnLanguageChanged() = 0;
    //@}
};

} //! namespace tools

#endif // LanguageObserver_ABC_h
