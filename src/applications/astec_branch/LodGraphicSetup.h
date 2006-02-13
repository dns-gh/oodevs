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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/LodGraphicSetup.h $
// $Author: Age $
// $Modtime: 24/06/05 16:58 $
// $Revision: 2 $
// $Workfile: LodGraphicSetup.h $
//
// *****************************************************************************

#ifndef __LodGraphicSetup_h_
#define __LodGraphicSetup_h_

#include "graphics/GraphicSetup.h"
class TerrainData;

// =============================================================================
/** @class  LodGraphicSetup
    @brief  LodGraphicSetup
*/
// Created: AGE 2005-02-28
// =============================================================================
class LodGraphicSetup : public GraphicSetup
{

public:
    //! @name Constructors/Destructor
    //@{
             LodGraphicSetup();
    virtual ~LodGraphicSetup();
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
    LodGraphicSetup( const LodGraphicSetup& );            //!< Copy constructor
    LodGraphicSetup& operator=( const LodGraphicSetup& ); //!< Assignement operator
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

#endif // __LodGraphicSetup_h_
