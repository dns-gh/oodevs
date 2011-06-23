// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StockPrototype_h_
#define __StockPrototype_h_

#include "clients_gui/StockPrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

// =============================================================================
/** @class  StockPrototype
    @brief  GUI class to create stock object
*/
// Created: BCI 2011-02-04
// =============================================================================
class StockPrototype : public gui::StockPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    StockPrototype( QWidget* parent, const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver, kernel::Object_ABC*& creation );
    virtual ~StockPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __StockPrototype_h_
