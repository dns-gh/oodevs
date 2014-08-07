// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectAttributePrototypeFactory_h_
#define __ObjectAttributePrototypeFactory_h_

#include "ObjectAttributePrototypeFactory_ABC.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class ObjectType;
}

namespace gui
{
// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeFactory
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
class ObjectAttributePrototypeFactory : public ObjectAttributePrototypeFactory_ABC
{
public:
    //! @name Types
    //@{
    typedef std::function< void( xml::xistream&, T_AttributeContainer&, QWidget* ) > T_CallBack;
    typedef std::function< void() >                                                  FinalizePrototype_CallBack;
    typedef std::vector< FinalizePrototype_CallBack >                                FinalizePrototype_CallBacks;
    //@}

public:
    //! @name Constructor/Destructor
    //@{
             ObjectAttributePrototypeFactory();
    virtual ~ObjectAttributePrototypeFactory();
    //@}

    //! @name Methods
    //@{
    void Register( const std::string& capacity, const T_CallBack& callback );
    void RegisterFinalizeCreate( const FinalizePrototype_CallBack& callback );
    virtual void Create( const std::string& capacity, xml::xistream& xis, T_AttributeContainer& resolver, QWidget* parent ) const;
    virtual void FinalizeCreate() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, T_CallBack > T_CallBacks;
    //@}

private:
    //! @name Member data
    //@{
    T_CallBacks callbacks_;
    FinalizePrototype_CallBacks finalizePrototypeCallbacks_;
    //@}
};

}

#endif // __ObjectPrototypeFactory_ABC_h_
