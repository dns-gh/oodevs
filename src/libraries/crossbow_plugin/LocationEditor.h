// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationEditor_h_
#define __LocationEditor_h_

#include "ESRI.h"
#include "game_asn/Asn.h"
#include "GeometryEditor.h"

namespace crossbow
{
    class ScopeEditor;

// =============================================================================
/** @class  LocationEditor
    @brief  LocationEditor
*/
// Created: JCR 2007-05-23
// =============================================================================
class LocationEditor : public GeometryEditor
{

public:
    //! @name Constructors/Destructor
    //@{
             LocationEditor( ScopeEditor& scope, ISpatialReferencePtr spSpatialReference );
    virtual ~LocationEditor();
    //@}

    //! @name Operations
    //@{
    void CreateGeometry( IFeatureBufferPtr spBuffer, const ASN1T_Location& asn );
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
    ScopeEditor& scope_;
    //@}
};

}

#endif // __LocationEditor_h_
