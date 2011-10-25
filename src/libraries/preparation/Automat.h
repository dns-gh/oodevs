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
#include "clients_kernel/Serializable_ABC.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class AutomatType;
    class Controller;
}

namespace xml
{
    class xistream;
    class xostream;
}

class IdManager;

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: AGE 2006-10-06
// =============================================================================
class Automat : public kernel::EntityImplementation< kernel::Automat_ABC >
              , public kernel::Extension_ABC
              , public kernel::Drawable_ABC
              , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Automat( const kernel::AutomatType& type, kernel::Controller& controller, IdManager& idManager, const QString& name );
             Automat( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager, const kernel::AutomatType& type );
    virtual ~Automat();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::AutomatType& GetType() const;
    virtual const kernel::LogisticLevel& GetLogisticLevel() const;
    void Rename( const QString& name );
    //@}

    //! @name Extensions
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    void CreateDictionary( kernel::Controller& controller );
    void InitializeSymbol() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::AutomatType& type_;
    mutable std::string symbol_;
    mutable std::string level_;
    std::string color_;
    std::string nature_;
    //@}
};

#endif // __Automat_h_
