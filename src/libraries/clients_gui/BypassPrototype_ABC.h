// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BypassPrototype_ABC_h_
#define __BypassPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "ValuedComboBox.h"

namespace kernel
{
    class NBCAgent;
}

namespace gui
{

// =============================================================================
/** @class  BypassPrototype_ABC
    @brief  BypassPrototype_ABC
*/
// Created: SBO 2006-04-20
// =============================================================================
class BypassPrototype_ABC : public ObjectAttributePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             BypassPrototype_ABC( QWidget* parent );
    virtual ~BypassPrototype_ABC();
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
    BypassPrototype_ABC( const BypassPrototype_ABC& );            //!< Copy constructor
    BypassPrototype_ABC& operator=( const BypassPrototype_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{    
    virtual void showEvent( QShowEvent* );
    //@}

protected:
    //! @name Member data
    //@{
    // SpinBox
    //@}
};

}

#endif // __BypassPrototype_ABC_h_
