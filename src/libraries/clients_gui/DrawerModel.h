// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerModel_h_
#define __DrawerModel_h_

#include "DashStyle.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Controllers;
    class Drawing_ABC;
    class EntityResolver_ABC;
    class Entity_ABC;
    class Location_ABC;
}

namespace tools
{
    class SchemaWriter_ABC;
    class Loader_ABC;
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
    void Load( const tools::Loader_ABC& fileLoader, const tools::Path& file );
    void Serialize( const tools::Path& filename, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void Purge();

    void Create( const DrawingTemplate& style, const QColor& color, const kernel::Entity_ABC* entity,
                 E_Dash_style dashStyle, kernel::Location_ABC& location ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadShapes( xml::xistream& xis, bool& failed );
    void ReadFormation( xml::xistream& xis, bool& failed );
    void ReadAutomat( xml::xistream& xis, bool& failed );
    void ReadShape( xml::xistream& xis, const kernel::Entity_ABC* diffusionEntity, bool& failed );
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
