// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RotaPrototype_ABC_h_
#define __RotaPrototype_ABC_h_

#include "ObjectPrototypeAttributes_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class NBCAgent;
}

namespace gui
{
    class RichLabel;

// =============================================================================
/** @class  RotaPrototype_ABC
    @brief  RotaPrototype_ABC
*/
// Created: SBO 2006-04-20
// =============================================================================
class RotaPrototype_ABC : public ObjectPrototypeAttributes_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             RotaPrototype_ABC( QWidget* parent, const kernel::Resolver_ABC< kernel::NBCAgent >& resolver );
    virtual ~RotaPrototype_ABC();
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
    RotaPrototype_ABC( const RotaPrototype_ABC& );            //!< Copy constructor
    RotaPrototype_ABC& operator=( const RotaPrototype_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* );
    void FillTypes();
    //@}

protected:
    //! @name Helpers
    //@{
    unsigned GetAgentCount() const;
    //@}

    //! @name Member data
    //@{
    const kernel::Resolver_ABC< kernel::NBCAgent >& resolver_;
    RichLabel* nbcAgentsLabel_;
    QSpinBox* danger_;
    QListView* nbcAgents_;
    //@}
};

}

#endif // __RotaPrototype_ABC_h_
