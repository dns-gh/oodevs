// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Automat_h_
#define __Automat_h_

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Extension_ABC.h"
#include "tools/Resolver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controller;
    class PropertiesDictionary;
    class LogisticLevel;
    class StaticModel;
}

namespace sword
{
    class AutomatCreation;
}

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: AGE 2006-10-06
// =============================================================================
class Automat : public kernel::EntityImplementation< kernel::Automat_ABC >
              , public kernel::Extension_ABC
              , public kernel::Drawable_ABC
              , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Automat( const sword::AutomatCreation& message, kernel::Controller& controller, const tools::Resolver_ABC< kernel::AutomatType >& resolver );
    virtual ~Automat();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::AutomatType& GetType() const;
    virtual const kernel::LogisticLevel& GetLogisticLevel() const;
    //@}

    //! @name Extensions
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    void InitializeSymbol() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::AutomatType& type_;
    mutable std::string symbol_; // $$$$ RC LDC: code duplication with preparation
    mutable std::string level_;
    const kernel::LogisticLevel* logisticLevel_;
    //@}
};

#endif // __Automat_h_
