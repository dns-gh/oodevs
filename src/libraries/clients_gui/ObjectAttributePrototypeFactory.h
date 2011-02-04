// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectAttributePrototypeFactory_h_
#define __ObjectAttributePrototypeFactory_h_

#include "ObjectAttributePrototypeFactory_ABC.h"
#include <boost/function.hpp>

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
    typedef boost::function3< void, xml::xistream&, T_AttributeContainer&, QWidget* > T_CallBack;
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
    virtual void Create( const std::string& capacity, xml::xistream& xis, T_AttributeContainer& resolver, QWidget* parent ) const;
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
    //@}
};

}

#endif // __ObjectPrototypeFactory_ABC_h_
