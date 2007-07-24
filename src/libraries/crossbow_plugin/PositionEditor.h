// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PositionEditor_h_
#define __PositionEditor_h_

#include "ESRI.h"
#include "GeometryEditor.h"

namespace crossbow
{
    class ScopeEditor;

// =============================================================================
/** @class  PositionEditor
    @brief  PositionEditor
*/
// Created: JCR 2007-05-15
// =============================================================================
class PositionEditor : public GeometryEditor
{
public:
    //! @name Constructors/Destructor
    //@{
             PositionEditor( ScopeEditor& scope, ISpatialReferencePtr spSpatialReference );
    virtual ~PositionEditor();
    //@}

    //! @name Operations
    //@{
    void CreateGeometry( IFeatureBufferPtr spBuffer );
    void CreateGeometry( IFeatureBufferPtr spBuffer, const ASN1T_CoordUTM& asnUTM );
    void Write( IFeaturePtr spFeature, const ASN1T_CoordUTM& asnUTM );
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
    ScopeEditor& scope_;
    //@}
};

}

#endif // __PositionEditor_h_
