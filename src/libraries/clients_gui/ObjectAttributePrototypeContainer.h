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

#include "ObjectAttributePrototypeFactory_ABC.h"
#include <tools/Resolver.h>

namespace boost
{
    template<typename T> class shared_ptr;
}

namespace kernel
{
    class ObjectType;
    class Team_ABC;
}

class QWidget;

namespace gui
{
    class ObjectAttributePrototype_ABC;
    class ObjectPrototypeLoader_ABC;

// =============================================================================
/** @class  ObjectPrototypeFactory_ABC
    @brief  Object prototype base class
*/
// Created: JCR 2008-04-18
// =============================================================================
class ObjectAttributePrototypeContainer
{
public:
    //! @name Types
    //@{
    typedef ObjectAttributePrototypeFactory_ABC::T_AttributeContainer T_AttributeContainer;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ObjectAttributePrototypeContainer( const tools::Resolver_ABC< kernel::ObjectType, std::string >& resolver,
                                                std::unique_ptr< ObjectAttributePrototypeFactory_ABC > factory, QWidget* parent );
    virtual ~ObjectAttributePrototypeContainer();
    //@}

    //! @name Operations
    //@{
    bool CheckValidity( const kernel::Team_ABC& team ) const;
    void Commit( const kernel::Team_ABC& );
    void Clean();
    void Hide();
    void Show();

    const T_AttributeContainer* Select( const kernel::ObjectType& type );
    void SetLoader( ObjectPrototypeLoader_ABC* loader );
    virtual void NotifyUpdated( /*const ModelLoaded& */); // $$$$ JCR 2009-04-03: Enregistrer aupres du controleur
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< T_AttributeContainer > > T_AttributesPrototypes;
    typedef T_AttributesPrototypes::iterator                                  IT_AttributesPrototypes;
    typedef T_AttributesPrototypes::const_iterator                           CIT_AttributesPrototypes;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectAttributePrototypeContainer( const ObjectAttributePrototypeContainer& );            //!< Copy constructor
    ObjectAttributePrototypeContainer& operator=( const ObjectAttributePrototypeContainer& ); //!< Assignment operator
    //@}

    //! @name Helper
    //@{
    void Load( const kernel::ObjectType& type );
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< ObjectAttributePrototypeFactory_ABC > factory_;
    const tools::Resolver_ABC< kernel::ObjectType, std::string >& resolver_;
    T_AttributesPrototypes attributes_;
    boost::shared_ptr< T_AttributeContainer > current_;
    QWidget* parent_;
    //@}
};

}

#endif // __ObjectAttributePrototypeContainer_h_
