// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __clients_gui_UrbanDrawer_h_
#define __clients_gui_UrbanDrawer_h_

#include "urban/Drawer_ABC.h"

namespace kernel
{
    class GlTools_ABC;
}

namespace urban
{
    class TerrainObject_ABC;
    class DrawingToolbox_ABC;
}

namespace gui
{

// =============================================================================
/** @class  UrbanDrawer
@brief  UrbanDrawer declaration
*/
// Created: RPD 2009-09-30
// =============================================================================
class UrbanDrawer : public urban::Drawer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanDrawer( const kernel::GlTools_ABC& tools );
    virtual ~UrbanDrawer();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const urban::Block& object, bool selected ) const;
    virtual void Draw( const urban::City& object, bool selected ) const;
    virtual void Draw( const urban::District& object, bool selected ) const;
    virtual void Draw( const urban::ResourceNetwork& object, bool selected ) const;
    virtual void Draw( const urban::UndergroundNetwork& object, bool selected ) const;
    virtual void Draw( Location_ABC& location ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanDrawer( const UrbanDrawer& );            //!< Copy constructor
    UrbanDrawer& operator=( const UrbanDrawer& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    const kernel::GlTools_ABC& toolbox_;
    //@}
};

}

#endif // __clients_gui_Urban_Drawer_ABC_h_
