// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gearth_PositionEditor_h_
#define __gearth_PositionEditor_h_

#include "game_asn/Asn.h"
#include "GeometryEditor.h"

namespace xml
{
    class xobufferstream;
}

namespace gearth
{    
// =============================================================================
/** @class  PositionEditor
    @brief  PositionEditor
*/
// Created: JCR 2007-05-15
// =============================================================================
class PositionEditor
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PositionEditor( xml::xobufferstream& xobs );             
    virtual ~PositionEditor();
    //@}

    //! @name Operations
    //@{    
    void Write( const ASN1T_CoordUTM& asnUTM );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PositionEditor( const PositionEditor& );            //!< Copy constructor
    PositionEditor& operator=( const PositionEditor& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    GeometryEditor       editor_;
    xml::xobufferstream& xobs_;
    //@}
};

}

#endif // __gearth_PositionEditor_h_
