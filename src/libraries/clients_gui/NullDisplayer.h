// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __NullDisplayer_h_
#define __NullDisplayer_h_

#include "clients_kernel/Displayer_ABC.h"

namespace gui
{

// =============================================================================
/** @class  NullDisplayer
    @brief  NullDisplayer
*/
// Created: AGE 2006-08-24
// =============================================================================
class NullDisplayer : public kernel::Displayer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NullDisplayer();
    virtual ~NullDisplayer();
    //@}

    //! @name Operations
    //@{
    virtual void Hide();
    virtual void Clear();
    //@}

private:
    //! @name Helpers
    //@{
    virtual kernel::Displayer_ABC& SubItem( const QString& name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    //@}
};

}

#endif // __NullDisplayer_h_