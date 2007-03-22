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

namespace xml
{
    class xistream;
}

namespace gui
{
    class DrawerCategory;
    class DrawerFactory;

// =============================================================================
/** @class  DrawerModel
    @brief  DrawerModel
*/
// Created: SBO 2007-03-22
// =============================================================================
class DrawerModel : public kernel::Resolver< DrawerCategory, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DrawerModel( const DrawerFactory& factory );
    virtual ~DrawerModel();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& filename );
    void Purge();
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
    //@}

private:
    //! @name Member data
    //@{
    const DrawerFactory& factory_;
    //@}
};

}

#endif // __DrawerModel_h_
