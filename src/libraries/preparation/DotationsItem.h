// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DotationsItem_h_
#define __DotationsItem_h_

#include "tools/Resolver.h"

namespace kernel
{
    class Entity_ABC;
    class PropertiesDictionary;
    class DotationType;
    class Controller;
}

class Dotation;

// =============================================================================
/** @class  DotationsItem
    @brief  DotationsItem
*/
// Created: SBO 2006-11-10
// =============================================================================
class DotationsItem
{
public:
    //! @name Constructors/Destructor
    //@{
             DotationsItem( kernel::Controller& controller, kernel::Entity_ABC& owner, kernel::PropertiesDictionary& dico, const QString& propertyName, tools::Resolver< Dotation >& dotations );
    virtual ~DotationsItem();
    //@}

    //! @name Operations
    //@{
    tools::Iterator< const Dotation& > CreateIterator() const;
    void AddDotation( const kernel::DotationType& dotation, unsigned long quantity );
    void AddDotation( const Dotation& dotation );
    void Clear();
    void Update();
    unsigned long CountDotations() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DotationsItem( const DotationsItem& );            //!< Copy constructor
    DotationsItem& operator=( const DotationsItem& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Entity_ABC& owner_;
    kernel::PropertiesDictionary& dico_;
    QString propertyName_;
    tools::Resolver< Dotation >& dotations_;
    //@}
};

#endif // __DotationsItem_h_
