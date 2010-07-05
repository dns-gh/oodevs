// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __FirePrototype_ABC_h_
#define __FirePrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"
#include "tools/Resolver_ABC.h"
#include "ValuedComboBox.h"

namespace kernel
{
    class FireClass;
}

namespace gui
{

// =============================================================================
/** @class  FirePrototype_ABC
    @brief  FirePrototype_ABC
*/
// Created: JCR 2008-06-30
// =============================================================================
class FirePrototype_ABC : public ObjectAttributePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             FirePrototype_ABC( QWidget* parent, const tools::Resolver_ABC< kernel::FireClass >& resolver );
    virtual ~FirePrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FirePrototype_ABC( const FirePrototype_ABC& );            //!< Copy constructor
    FirePrototype_ABC& operator=( const FirePrototype_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void FillTypes();
    virtual void showEvent( QShowEvent* );
    //@}

protected:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::FireClass >& resolver_;
    ValuedComboBox< const kernel::FireClass* >* fireClass_;
    //@}
};

}

#endif // __FirePrototype_ABC_h_
