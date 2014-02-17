// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerTesselator_h_
#define __DrawerTesselator_h_

#include "TesselatorBase.h"

// =============================================================================
/** @class  DrawerTesselator
    @brief  Drawer tesselator
*/
// Created: AGE 2005-01-10
// =============================================================================
class DrawerTesselator : public TesselatorBase
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DrawerTesselator( unsigned int windingRule );
    virtual ~DrawerTesselator();
    //@}

    //! @name Operations
    //@{
    virtual void Begin( unsigned int type );
    virtual void Vertex( const geometry::Point& point, unsigned int index );
    virtual void End();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DrawerTesselator( const DrawerTesselator& );            //!< Copy constructor
    DrawerTesselator& operator=( const DrawerTesselator& ); //!< Assignement operator
    //@}
};

#endif // __DrawerTesselator_h_
