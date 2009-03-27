// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FirePrototype_ABC_h_
#define __FirePrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
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
// Created: SBO 2006-04-20
// =============================================================================
class FirePrototype_ABC : public ObjectAttributePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             FirePrototype_ABC( QWidget* parent, const kernel::Resolver_ABC< kernel::FireClass >& resolver );
    virtual ~FirePrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void Commit() = 0;
    virtual void Clean() = 0;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FirePrototype_ABC( const FirePrototype_ABC& );            //!< Copy constructor
    FirePrototype_ABC& operator=( const FirePrototype_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void FillTypes();
    virtual void showEvent( QShowEvent* );
    //@}

protected:
    //! @name Member data
    //@{
    const kernel::Resolver_ABC< kernel::FireClass >& resolver_;
    ValuedComboBox< const kernel::FireClass* >* fireClass_;
    //@}
};

}

#endif // __FirePrototype_ABC_h_
