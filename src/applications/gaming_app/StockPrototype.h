// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __StockPrototype_h_
#define __StockPrototype_h_

#include "clients_gui/StockPrototype_ABC.h"

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

// =============================================================================
/** @class  StockPrototype
    @brief  StockPrototype
*/
// Created: BCI 2011-03-31
// =============================================================================
class StockPrototype  : public gui::StockPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             StockPrototype( QWidget* parent, const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver, actions::parameters::ParameterList*& attributesList );
    virtual ~StockPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private:
    //! @name Member data
    //@{
    actions::parameters::ParameterList*& attributesList_;
    //@}
};

#endif // __StockPrototype_h_
