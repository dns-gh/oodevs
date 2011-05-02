// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCPrototype_h_
#define __NBCPrototype_h_

#include "clients_gui/NBCPrototype_ABC.h"

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

struct ObjectAttributesNbcZone;
struct ObjectAttributesNbcCloud;

// =============================================================================
/** @class  NBCPrototype
    @brief  NBCPrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class NBCPrototype : public gui::NBCPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NBCPrototype( QWidget* parent, const tools::Resolver_ABC< kernel::NBCAgent >& resolver, int maxToxic, actions::parameters::ParameterList*& attributesList );
    virtual ~NBCPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private:
    //! @name Types
    //@{
    enum E_Form
    {
        eLiquid,
        eGas
    };
    //@}

private:
    //! @name Member data
    //@{
    actions::parameters::ParameterList*& attributesList_;
    //@}
};

#endif // __NBCPrototype_h_
