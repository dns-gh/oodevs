// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __NBCPrototype_ABC_h_
#define __NBCPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"
#include "tools/Resolver_ABC.h"
#include "ValuedComboBox.h"

namespace kernel
{
    class NBCAgent;
}

namespace gui
{
    class RichLabel;

// =============================================================================
/** @class  NBCPrototype_ABC
    @brief  NBCPrototype_ABC
*/
// Created: SBO 2006-04-20
// =============================================================================
class NBCPrototype_ABC : public ObjectAttributePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             NBCPrototype_ABC( QWidget* parent, const tools::Resolver_ABC< kernel::NBCAgent >& resolver, int maxToxic );
    virtual ~NBCPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
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
    //! @name Helpers
    //@{
    unsigned GetAgentCount() const;
    //@}

    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::NBCAgent >& resolver_;
    int maxToxic_;
    
    RichLabel* nbcAgentsLabel_;
    QSpinBox* danger_;
    QListView* nbcAgents_;
    ValuedComboBox< std::string >* nbcStates_;
    //@}
};

}

#endif // __NBCPrototype_ABC_h_
