// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gearth_LineEditor_h_
#define __gearth_LineEditor_h_

#include "game_asn/Asn.h"
#include "GeometryEditor.h"

namespace xml
{
    class xobufferstream;
}

namespace gearth
{
// =============================================================================
/** @class  LineEditor
    @brief  LineEditor
*/
// Created: JCR 2007-05-15
// =============================================================================
class LineEditor
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LineEditor( xml::xobufferstream& xobs );
    virtual ~LineEditor();
    //@}

    //! @name Operations
    //@{
    void Write( const ASN1T_Line& asnLine );
    void Write( const ASN1T_CoordUTMList& asnLine );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LineEditor( const LineEditor& );            //!< Copy constructor
    LineEditor& operator=( const LineEditor& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    GeometryEditor       editor_;
    xml::xobufferstream& xobs_;
    //@}
};

}

#endif // __gearth_LineEditor_h_
