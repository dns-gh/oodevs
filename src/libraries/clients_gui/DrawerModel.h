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
}

namespace tools
{
    class SchemaWriter_ABC;
}

namespace gui
{
    class Drawing_ABC;
    class DrawingTemplate;
    class DrawingFactory_ABC;

// =============================================================================
/** @class  DrawerModel
    @brief  DrawerModel
*/
// Created: SBO 2007-03-22
// =============================================================================
class DrawerModel : public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< Drawing_ABC >
                  , public tools::Resolver< Drawing_ABC >
                  , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DrawerModel( kernel::Controllers& controllers, const DrawingFactory_ABC& factory );
    virtual ~DrawerModel();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& filename );
    void Save( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void Purge();

    Drawing_ABC* Create( const DrawingTemplate& style, const QColor& color, const kernel::Entity_ABC* entity ) const;
    void Delete( unsigned long id );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadShapes( xml::xistream& xis );
    void ReadShape( xml::xistream& xis );
    virtual void NotifyCreated( const Drawing_ABC& );
    virtual void NotifyDeleted( const Drawing_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const DrawingFactory_ABC& factory_;
    //@}
};

}

#endif // __DrawerModel_h_
