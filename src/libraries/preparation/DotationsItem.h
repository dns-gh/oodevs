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

#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
    class DotationType;
    class Controller;
}

namespace gui
{
    class PropertiesDictionary;
}

class Dotation;

// =============================================================================
/** @class  DotationsItem
    @brief  DotationsItem
*/
// Created: SBO 2006-11-10
// =============================================================================
class DotationsItem : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DotationsItem( kernel::Controller& controller, kernel::Entity_ABC& owner, gui::PropertiesDictionary& dico, const QString& propertyName, tools::Resolver< Dotation >& dotations, bool isStock );
    virtual ~DotationsItem();
    //@}

    //! @name Operations
    //@{
    tools::Iterator< const Dotation& > CreateIterator() const;
    void AddDotation( const kernel::DotationType& dotation, unsigned long quantity );
    void AddDotation( const Dotation& dotation );
    void RemoveDotation( const kernel::DotationType& dotation );
    void Clear();
    void Update();
    unsigned long CountDotations() const;
    bool IsStock() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Entity_ABC& owner_;
    gui::PropertiesDictionary& dico_;
    QString propertyName_;
    tools::Resolver< Dotation >& dotations_;
    bool isStock_;
    //@}
};

#endif // __DotationsItem_h_
