// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPrototypeAttributes_ABC_h_
#define __ObjectPrototypeAttributes_ABC_h_

#include <qgroupbox.h>

namespace gui
{

// =============================================================================
/** @class  ObjectPrototypeAttributes_ABC
    @brief  Object prototype attributes definition
*/
// Created: SBO 2006-04-20
// =============================================================================
class ObjectPrototypeAttributes_ABC : public QGroupBox
{
public:
    //! @name Constructor/Destructor
    //@{
             ObjectPrototypeAttributes_ABC( QWidget* parent, const QString& name );
    virtual ~ObjectPrototypeAttributes_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const = 0;
    virtual void Commit() = 0;
    virtual void Clean() = 0;
    //@}
};

}

#endif // __ObjectPrototypeAttributes_ABC_h_
