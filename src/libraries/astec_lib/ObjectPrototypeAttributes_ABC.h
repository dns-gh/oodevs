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

struct ASN1T_MagicActionCreateObject;

class ObjectPrototypeAttributes_ABC : public QGroupBox
{
public:
    //! @name Constructor/Destructor
    //@{
             ObjectPrototypeAttributes_ABC( QWidget* parent = 0, const char* name = 0 )
                 : QGroupBox( 2, Qt::Horizontal, name, parent ) {};
    virtual ~ObjectPrototypeAttributes_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const = 0;
    virtual void Serialize( ASN1T_MagicActionCreateObject& msg ) = 0;
    virtual void Clean() = 0;
    //@}
};

#endif // __ObjectPrototypeAttributes_ABC_h_
