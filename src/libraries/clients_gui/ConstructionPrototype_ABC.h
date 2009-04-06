// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ConstructionPrototype_ABC_h_
#define __ConstructionPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"

namespace gui
{
    
// =============================================================================
/** @class  ConstructionPrototype_ABC
    @brief  ConstructionPrototype_ABC
*/
// Created: JCR 2008-06-30
// =============================================================================
class ConstructionPrototype_ABC : public ObjectAttributePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ConstructionPrototype_ABC( QWidget* parent );
    virtual ~ConstructionPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ConstructionPrototype_ABC( const ConstructionPrototype_ABC& );            //!< Copy constructor
    ConstructionPrototype_ABC& operator=( const ConstructionPrototype_ABC& ); //!< Assignement operator
    //@}
};

}

#endif // __ConstructionPrototype_ABC_h_
