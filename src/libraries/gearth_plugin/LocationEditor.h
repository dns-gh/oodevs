// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gearth_LocationEditor_h_
#define __gearth_LocationEditor_h_

#include "game_asn/Simulation.h"
#include "GeometryEditor.h"

namespace xml
{
    class xobufferstream;
}

namespace gearth
{

// =============================================================================
/** @class  LocationEditor
    @brief  LocationEditor
*/
// Created: JCR 2007-05-23
// =============================================================================
class LocationEditor
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LocationEditor( xml::xobufferstream& xobs );
    virtual ~LocationEditor();
    //@}

    //! @name Operations
    //@{
    void Write( const ASN1T_Location& asn );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LocationEditor( const LocationEditor& );            //!< Copy constructor
    LocationEditor& operator=( const LocationEditor& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    GeometryEditor       editor_;
    xml::xobufferstream& xobs_;
    //@}
};

}

#endif // __gearth_LocationEditor_h_
