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

struct ASN1T_MagicActionCreateObject;

class ObjectPrototypeAttributes_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             ObjectPrototypeAttributes_ABC() {};
    virtual ~ObjectPrototypeAttributes_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const = 0;
    virtual void Serialize( ASN1T_MagicActionCreateObject& msg ) const = 0;
    virtual void Show() const = 0;
    virtual void Hide() = 0;
    //@}
};

#endif // __ObjectPrototypeAttributes_ABC_h_
