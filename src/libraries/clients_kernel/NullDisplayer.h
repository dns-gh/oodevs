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

#include "Displayer_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  NullDisplayer
    @brief  NullDisplayer
*/
// Created: AGE 2006-08-24
// =============================================================================
class NullDisplayer : public Displayer_ABC
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
    //@}

private:
    //! @name Copy/Assignement
    //@{
    NullDisplayer( const NullDisplayer& );            //!< Copy constructor
    NullDisplayer& operator=( const NullDisplayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual Displayer_ABC& SubItem( const QString& name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    //@}
};

}

#endif // __NullDisplayer_h_
