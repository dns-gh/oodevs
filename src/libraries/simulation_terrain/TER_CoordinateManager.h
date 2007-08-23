// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_CoordinateManager.h $
// $Author: Age $
// $Modtime: 25/05/05 13:30 $
// $Revision: 4 $
// $Workfile: TER_CoordinateManager.h $
//
// *****************************************************************************

#ifndef __TER_CoordinateManager_h_
#define __TER_CoordinateManager_h_

#include "geocoord/PlanarCartesian.h"
#include "geocoord/MGRS.h"
#include "MT_Tools/MT_Rect.h"

// =============================================================================
/** @class  TER_CoordinateManager
    @brief  TER_CoordinateManager
*/
// Created: AGE 2005-01-31
// =============================================================================
class TER_CoordinateManager
{

public:
    //! @name Constructors/Destructor
    //@{
             TER_CoordinateManager( float rMiddleLatitude, float rMiddleLongitude, const MT_Rect& extent );
    virtual ~TER_CoordinateManager();
    //@}

    //! @name Operations
    //@{
    void MosToSimMgrsCoord( const std::string& strMgrs, MT_Vector2D& pos ) const;
    void SimToMosMgrsCoord( const MT_Vector2D& pos, std::string& strMgrs ) const;

    MT_Float GetWeldValue() const;
    MT_Float GetWidth    () const;
    MT_Float GetHeight   () const;

    bool IsValidPosition( const MT_Vector2D& pos ) const;
    void ClipPointInsideWorld( MT_Vector2D& pos ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TER_CoordinateManager( const TER_CoordinateManager& );            //!< Copy constructor
    TER_CoordinateManager& operator=( const TER_CoordinateManager& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const   geocoord::PlanarCartesian::Parameters parameters_;
    mutable geocoord::PlanarCartesian             planar_;
    mutable geocoord::MGRS                        mgrs_;
    
    const MT_Rect     extent_;
    const MT_Vector2D translation_;

    std::string strDefaultMGRS_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: TER_CoordinateManager::GetWeldValue
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
inline
MT_Float TER_CoordinateManager::GetWeldValue() const
{
    return 10; // $$$$ AGE 2005-03-08: or what
}

// -----------------------------------------------------------------------------
// Name: TER_CoordinateManager::GetWidth
// Created: NLD 2005-04-11
// -----------------------------------------------------------------------------
inline
MT_Float TER_CoordinateManager::GetWidth() const
{
    return extent_.GetWidth();
}

// -----------------------------------------------------------------------------
// Name: TER_CoordinateManager::GetHeight
// Created: NLD 2005-04-11
// -----------------------------------------------------------------------------
inline
MT_Float TER_CoordinateManager::GetHeight() const
{
    return extent_.GetHeight();
}

#endif // __TER_CoordinateManager_h_
