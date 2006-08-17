// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCPrototype_ABC_h_
#define __NBCPrototype_ABC_h_

#include "ObjectPrototypeAttributes_ABC.h"
#include "clients_kernel/Resolver.h"
#include "ValuedComboBox.h"

class NBCAgent;

// =============================================================================
/** @class  NBCPrototype_ABC
    @brief  NBCPrototype_ABC
*/
// Created: SBO 2006-04-20
// =============================================================================
class NBCPrototype_ABC : public ObjectPrototypeAttributes_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             NBCPrototype_ABC( QWidget* parent, const Resolver< NBCAgent >& resolver );
    virtual ~NBCPrototype_ABC();
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
    NBCPrototype_ABC( const NBCPrototype_ABC& );            //!< Copy constructor
    NBCPrototype_ABC& operator=( const NBCPrototype_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void FillTypes();
    virtual void showEvent( QShowEvent* );
    //@}

protected:
    //! @name Member data
    //@{
    const Resolver< NBCAgent >& resolver_;
    ValuedComboBox< const NBCAgent* >* nbcAgents_;
    //@}
};

#endif // __NBCPrototype_ABC_h_
