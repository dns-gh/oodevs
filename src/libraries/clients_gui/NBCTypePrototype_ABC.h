// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCTypePrototype_ABC_h_
#define __NBCTypePrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "ValuedComboBox.h"

namespace kernel
{
    class NBCTypeAgent;
}

namespace gui
{

// =============================================================================
/** @class  NBCTypePrototype_ABC
    @brief  NBCTypePrototype_ABC
*/
// Created: RFT 2006-04-20
// =============================================================================
class NBCTypePrototype_ABC : public ObjectAttributePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             NBCTypePrototype_ABC( QWidget* parent, const kernel::Resolver_ABC< kernel::NBCTypeAgent >& resolver );
    virtual ~NBCTypePrototype_ABC();
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
    NBCTypePrototype_ABC( const NBCTypePrototype_ABC& );            //!< Copy constructor
    NBCTypePrototype_ABC& operator=( const NBCTypePrototype_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void FillTypes();
    virtual void showEvent( QShowEvent* );
    //@}

protected:
    //! @name Member data
    //@{
    const kernel::Resolver_ABC< kernel::NBCTypeAgent >& resolver_;
    ValuedComboBox< const kernel::NBCTypeAgent* >* nbcAgents_;
    QSpinBox*    sourceLifeDuration_;
    //@}
};

}

#endif // __NBCTypePrototype_ABC_h_
