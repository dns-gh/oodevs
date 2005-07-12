// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-02-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_GraphicSetup.h $
// $Author: Age $
// $Modtime: 24/06/05 16:58 $
// $Revision: 2 $
// $Workfile: MOS_GraphicSetup.h $
//
// *****************************************************************************

#ifndef __MOS_GraphicSetup_h_
#define __MOS_GraphicSetup_h_

#include "graphics/GraphicSetup.h"
class TerrainData;

// =============================================================================
/** @class  MOS_GraphicSetup
    @brief  MOS_GraphicSetup
*/
// Created: AGE 2005-02-28
// =============================================================================
class MOS_GraphicSetup : public GraphicSetup
{

public:
    //! @name Constructors/Destructor
    //@{
             MOS_GraphicSetup();
    virtual ~MOS_GraphicSetup();
    //@}
    
    //! @name Operations
    //@{
    virtual void SetupLineGraphics  ( const Data_ABC* pData );
    virtual void SetupBorderGraphics( const Data_ABC* pData );
    virtual void SetupAreaGraphics  ( const Data_ABC* pData );

    unsigned int GetLastLevelOfDetail() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MOS_GraphicSetup( const MOS_GraphicSetup& );            //!< Copy constructor
    MOS_GraphicSetup& operator=( const MOS_GraphicSetup& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    static unsigned int GetLodFor( const TerrainData& data );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int nLastLod_;
    //@}
};

#endif // __MOS_GraphicSetup_h_
