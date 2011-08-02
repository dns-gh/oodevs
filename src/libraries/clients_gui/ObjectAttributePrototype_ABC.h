// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectAttributePrototype_ABC_h_
#define __ObjectAttributePrototype_ABC_h_

#include <boost/noncopyable.hpp>

namespace gui
{
    class ObjectPrototypeLoader_ABC;

// =============================================================================
/** @class  ObjectAttributePrototype_ABC
    @brief  Object prototype attributes definition
*/
// Created: JCR 2008-06-11
// =============================================================================
class ObjectAttributePrototype_ABC : public Q3GroupBox
                                   , private boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
             ObjectAttributePrototype_ABC( QWidget* parent, const QString& name );
    virtual ~ObjectAttributePrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const = 0;
    virtual void Commit() = 0;
    virtual void SetLoader( ObjectPrototypeLoader_ABC* loader );
    //@}
};

}

#endif // __ObjectAttributePrototype_ABC_h_
