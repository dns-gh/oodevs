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

#include <qgroupbox.h>

namespace gui
{

// =============================================================================
/** @class  ObjectAttributePrototype_ABC
    @brief  Object prototype attributes definition
*/
// Created: JCR 2008-06-11
// =============================================================================
class ObjectAttributePrototype_ABC : public QGroupBox
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
    virtual void Clean() = 0;
    //@}
};

}

#endif // __ObjectAttributePrototype_ABC_h_
