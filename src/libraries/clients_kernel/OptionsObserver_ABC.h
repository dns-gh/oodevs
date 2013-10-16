// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OptionsObserver_ABC_h_
#define __OptionsObserver_ABC_h_

#include <tools/Observer_ABC.h>
#include <string>

namespace kernel
{
    class OptionVariant;

// =============================================================================
/** @class  OptionsObserver_ABC
    @brief  Options observer definition
*/
// Created: AGE 2006-02-13
// =============================================================================
class OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             OptionsObserver_ABC() {}
    virtual ~OptionsObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const OptionVariant& value ) = 0;
    //@}
};

}

#endif // __OptionsObserver_ABC_h_
