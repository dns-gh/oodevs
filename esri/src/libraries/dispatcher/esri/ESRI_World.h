// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ESRI_World_h_
#define __ESRI_World_h_

#include "MT/MT_Tools/MT_Types.h"
#include "MT_Tools/MT_Vector2D.h"

class CoordinateManager;

namespace dispatcher 
{
namespace esri 
{
    class Config;
// =============================================================================
/** @class  ESRI_World
    @brief  ESRI_World
*/
// Created: JCR 2007-05-03
// =============================================================================
class World
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit World( const Config& config );
    virtual ~World();
    //@}

    //! @name Operations
    //@{    
    void MosToSimMgrsCoord( const std::string& strMgrs, MT_Vector2D& pos ) const;
    void SimToMosMgrsCoord( const MT_Vector2D& pos, std::string& strMgrs ) const;    
    //@}

private:
    //! @name Copy/Assignement
    //@{
    World( const World& );            //!< Copy constructor
    World& operator=( const World& ); //!< Assignement operator
    //@}
    
private:
    //! @name Member data
    //@{
    std::auto_ptr< CoordinateManager > pCoordinateManager_;
    //@}
};

} // end namespace esri
}

#endif // __ESRI_World_h_
