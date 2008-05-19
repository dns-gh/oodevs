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

#include "clients_kernel/Resolver.h"
#include "clients_kernel/ElementObserver_ABC.h"

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
    class DrawerCategory;
    class DrawerFactory;
    class DrawerShape;

// =============================================================================
/** @class  DrawerModel
    @brief  DrawerModel
*/
// Created: SBO 2007-03-22
// =============================================================================
class DrawerModel : public kernel::Resolver< DrawerCategory, QString >
                  , public kernel::Observer_ABC
                  , public kernel::ElementObserver_ABC< DrawerShape >
{

public:
    //! @name Constructors/Destructor
    //@{
             DrawerModel( kernel::Controllers& controllers, const DrawerFactory& factory );
    virtual ~DrawerModel();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& filename );
    void Purge();

    void LoadDrawings( const std::string& filename );
    void SaveDrawings( const std::string& filename ) const;
    void ClearDrawings();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DrawerModel( const DrawerModel& );            //!< Copy constructor
    DrawerModel& operator=( const DrawerModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadCategory( xml::xistream& xis );
    void ReadShape   ( xml::xistream& xis );
    virtual void NotifyCreated( const DrawerShape& );
    virtual void NotifyDeleted( const DrawerShape& );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const DrawerShape* > T_Shapes;
    typedef T_Shapes::iterator               IT_Shapes;
    
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const DrawerFactory& factory_;
    T_Shapes shapes_;
    //@}
};

}

#endif // __DrawerModel_h_
