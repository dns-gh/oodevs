// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectAttributePrototypeContainer_h_
#define __ObjectAttributePrototypeContainer_h_

#include "clients_kernel/Resolver.h"
#include "ObjectAttributePrototypeFactory_ABC.h"

namespace boost
{
    template<typename T> class shared_ptr;
}

namespace kernel
{    
    class ObjectType;    
}

class QWidget;

namespace gui
{
    class ObjectAttributePrototype_ABC;
    
// =============================================================================
/** @class  ObjectPrototypeFactory_ABC
    @brief  Object prototype base class
*/
// Created: JCR 2008-04-18
// =============================================================================
class ObjectAttributePrototypeContainer 
{    
public:
    //! @name 
    //@{
    typedef ObjectAttributePrototypeFactory_ABC::T_AttributeContainer T_AttributeContainer;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
            ObjectAttributePrototypeContainer( const kernel::Resolver_ABC< kernel::ObjectType, std::string >& resolver, const ObjectAttributePrototypeFactory_ABC& factory, QWidget* parent );
    virtual ~ObjectAttributePrototypeContainer();
    //@}

    //! @name 
    //@{
    void Commit();
    void Clean();
    void Hide();
    void Show();    
    //@}

    //! @name 
    //@{
    const T_AttributeContainer* Select( const kernel::ObjectType& type );
    virtual void NotifyUpdated( /*const ModelLoaded& */); // Enregistrer aupres du controleur
    //@}

private:
    //! @name Types
    //@{    
    typedef std::map< std::string, boost::shared_ptr< T_AttributeContainer > >   T_AttributesPrototypes;
    typedef T_AttributesPrototypes::iterator                                     IT_AttributesPrototypes;
    typedef T_AttributesPrototypes::const_iterator                               CIT_AttributesPrototypes;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectAttributePrototypeContainer( const ObjectAttributePrototypeContainer& );            //!< Copy constructor
    ObjectAttributePrototypeContainer& operator=( const ObjectAttributePrototypeContainer& ); //!< Assignement operator
    //@}

    //! @name Helper
    //@{
    void Load( const kernel::ObjectType& type );    
    //@}

private:
    //! @name Data members
    //@{
    const ObjectAttributePrototypeFactory_ABC& factory_;
    const kernel::Resolver_ABC< kernel::ObjectType, std::string >& resolver_;
    T_AttributesPrototypes  attributes_;
    T_AttributeContainer*   select_;
    QWidget*                parent_;
    //@}
};

}

#endif // __ObjectAttributePrototypeContainer_h_
