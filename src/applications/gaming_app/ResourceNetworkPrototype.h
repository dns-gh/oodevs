// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ResourceNetworkPrototype_h_
#define __ResourceNetworkPrototype_h_

#include "clients_gui/ResourceNetworkPrototype_ABC.h"

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

// =============================================================================
/** @class  ResourceNetworkPrototype
    @brief  ResourceNetworkPrototype
*/
// Created: JSR 2011-03-04
// =============================================================================
class ResourceNetworkPrototype  : public gui::ResourceNetworkPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworkPrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList );
    virtual ~ResourceNetworkPrototype();
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

#endif // __ResourceNetworkPrototype_h_
