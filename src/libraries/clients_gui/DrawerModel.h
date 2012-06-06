// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerModel_h_
#define __DrawerModel_h_

#include "tools/ElementObserver_ABC.h"
#include "tools/Resolver.h"
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class EntityResolver_ABC;
    class Drawing_ABC;
}

namespace tools
{
    class Loader_ABC;
    class SchemaWriter_ABC;
}

namespace gui
{
    class DrawingTemplate;
    class DrawingFactory_ABC;

// =============================================================================
/** @class  DrawerModel
    @brief  DrawerModel
*/
// Created: SBO 2007-03-22
// =============================================================================
class DrawerModel : public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< kernel::Drawing_ABC >
                  , public tools::Resolver< kernel::Drawing_ABC >
                  , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DrawerModel( kernel::Controllers& controllers, const DrawingFactory_ABC& factory, const kernel::EntityResolver_ABC& resolver );
    virtual ~DrawerModel();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::Loader_ABC& fileLoader, const std::string& file );
    void Serialize( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void Purge();

    kernel::Drawing_ABC* Create( const DrawingTemplate& style, const QColor& color, const kernel::Entity_ABC* entity ) const;
    void Delete( unsigned long id );
    //@}

private:
    //! @name Helpers
    //@{
    void Read( xml::xistream& xis );
    void ReadFormation( xml::xistream& xis );
    void ReadAutomat( xml::xistream& xis );
    void ReadShape( xml::xistream& xis, const kernel::Entity_ABC* diffusionEntity );
    void Serialize( xml::xostream& xos, const tools::SchemaWriter_ABC& schemaWriter ) const;
    virtual void NotifyCreated( const kernel::Drawing_ABC& );
    virtual void NotifyDeleted( const kernel::Drawing_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const DrawingFactory_ABC& factory_;
    const kernel::EntityResolver_ABC& resolver_;
    //@}
};

}

#endif // __DrawerModel_h_
