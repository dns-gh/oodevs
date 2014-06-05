// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectFactory_h_
#define __ObjectFactory_h_

#include "ObjectFactory_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace tools
{
    class IdManager;
}

class Model;
class StaticModel;
class ObjectAttributeFactory_ABC;

// =============================================================================
/** @class  ObjectFactory
    @brief  ObjectFactory
*/
// Created: AGE 2006-02-15
// =============================================================================
class ObjectFactory : public ObjectFactory_ABC
                    , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel, tools::IdManager& idManager );
    virtual ~ObjectFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Object_ABC* CreateObject( const kernel::ObjectType& type, const kernel::Team_ABC& team, const QString& name, const kernel::Location_ABC& location );
    virtual kernel::Object_ABC* CreateObject( xml::xistream& xis, const kernel::Team_ABC& team, const kernel::ObjectType& type );
    //@}

private:
    //! @name Helper
    //@{
    void Initialize();
    void ReadAttributes( const std::string& attr, xml::xistream& xis, kernel::Object_ABC& object, gui::PropertiesDictionary& dico );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& staticModel_;
    tools::IdManager& idManager_;
    std::unique_ptr< ObjectAttributeFactory_ABC > factory_;
    //@}
};

#endif // __ObjectFactory_h_
