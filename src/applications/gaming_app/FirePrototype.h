// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FirePrototype_h_
#define __FirePrototype_h_

#include "clients_gui/FirePrototype_ABC.h"

struct ObjectAttributeFire;

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

// =============================================================================
/** @class  FirePrototype
    @brief  FirePrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class FirePrototype : public gui::FirePrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    FirePrototype( QWidget* parent, const tools::Resolver_ABC< kernel::FireClass, std::string >& resolver, actions::parameters::ParameterList*& attributesList );
    virtual ~FirePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit( const kernel::Team_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    actions::parameters::ParameterList*& attributesList_;
    //@}
};

#endif // __FirePrototype_h_
