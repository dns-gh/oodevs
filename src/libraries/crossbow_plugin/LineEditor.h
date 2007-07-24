// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LineEditor_h_
#define __LineEditor_h_

#include "ESRI.h"
#include "game_asn/Asn.h"
#include "GeometryEditor.h"

namespace crossbow
{
    class ScopeEditor;

// =============================================================================
/** @class  LineEditor
    @brief  LineEditor
*/
// Created: JCR 2007-05-15
// =============================================================================
class LineEditor : public GeometryEditor
{

public:
    //! @name Constructors/Destructor
    //@{
             LineEditor( ScopeEditor& scope, ISpatialReferencePtr spSpatialReference );
    virtual ~LineEditor();
    //@}

    //! @name Operations
    //@{
    void CreateGeometry( IFeatureBufferPtr spBuffer, const ASN1T_Line& asnLine );
    void CreateGeometry( IFeatureBufferPtr spBuffer, const ASN1T_CoordUTMList& asnLine );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LineEditor( const LineEditor& );            //!< Copy constructor
    LineEditor& operator=( const LineEditor& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Update( IPointCollectionPtr spPointCollection, const ASN1T_CoordUTM& asnUTM );
    //@}

private:
    //! @name Member data
    //@{
    ScopeEditor& scope_;
    //@}
};

}

#endif // __LineEditor_h_
