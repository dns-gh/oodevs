// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_FolkEditor_h_
#define __crossbow_FolkEditor_h_

#include "ESRI.h"
#include "game_asn/Asn.h"
#include "FolkManager.h"

namespace crossbow
{    
    class ScopeEditor;

// =============================================================================
/** @class  FolkEditor
    @brief  FolkEditor
*/
// Created: JCR 2007-08-29
// =============================================================================
class FolkEditor
{	
public:
    //! @name Constructors/Destructor
    //@{
            FolkEditor( ScopeEditor& scope, const FolkManager& folk );
    virtual ~FolkEditor();
    //@}

    //! @name 
    //@{
    void Write( IFeaturePtr spFeature, const ASN1T_MsgFolkGraphEdgeUpdate& asn );
    void Write( IFeatureCursorPtr spCursor );
	//@}

private:
    //! @name Copy/Assignement
    //@{
    FolkEditor( const FolkEditor& );            //!< Copy constructor
    FolkEditor& operator=( const FolkEditor& ); //!< Assignement operator
    //@}

    //! @name 
    //@{
	void Write( IFeaturePtr spFeature, const FolkManager::PopulationInfo& info );
    //@}

private:
    //! @name Member data
    //@{
    ScopeEditor& scope_;
    const FolkManager& folk_;
    //@}
};

}

#endif // __crossbow_FolkEditor_h_
