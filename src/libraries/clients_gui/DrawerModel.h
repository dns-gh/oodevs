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

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/Resolver.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Controllers;
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
class DrawerModel : public kernel::Observer_ABC
                  , public kernel::ElementObserver_ABC< Drawing_ABC >
                  , public kernel::Resolver< Drawing_ABC >
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
    void Save( const std::string& filename ) const;
    void Purge();

    Drawing_ABC* Create( const DrawingTemplate& style, const QColor& color ) const;
    void Delete( unsigned long id );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DrawerModel( const DrawerModel& );            //!< Copy constructor
    DrawerModel& operator=( const DrawerModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
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
