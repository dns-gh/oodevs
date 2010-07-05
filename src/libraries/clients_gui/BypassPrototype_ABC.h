// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __BypassPrototype_ABC_h_
#define __BypassPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"

namespace gui
{

// =============================================================================
/** @class  BypassPrototype_ABC
    @brief  BypassPrototype_ABC
*/
// Created: JCR 2008-06-30
// =============================================================================
class BypassPrototype_ABC : public ObjectAttributePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit BypassPrototype_ABC( QWidget* parent );
    virtual ~BypassPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BypassPrototype_ABC( const BypassPrototype_ABC& );            //!< Copy constructor
    BypassPrototype_ABC& operator=( const BypassPrototype_ABC& ); //!< Assignment operator
    //@}
};

}

#endif // __BypassPrototype_ABC_h_
