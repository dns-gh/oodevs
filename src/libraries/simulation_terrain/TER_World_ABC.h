// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef TER_World_ABC_h
#define TER_World_ABC_h

#include "MT_Tools/MT_Vector2D.h"
#include <boost/noncopyable.hpp>

class MT_Rect;

// =============================================================================
/** @class  TER_World_ABC
    @brief  TER_World_ABC
*/
// Created: SLI 2014-07-24
// =============================================================================
class TER_World_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TER_World_ABC() {}
    virtual ~TER_World_ABC() {}
    //@}

    //! @name Coordinates
    //@{
    virtual void MosToSimMgrsCoord( const std::string& strMgrs, MT_Vector2D& pos ) const = 0;
    virtual void SimToMosMgrsCoord( const MT_Vector2D& pos, std::string& strMgrs ) const = 0;

    virtual void MosToSimMgrsCoord( double latitude, double longitude, MT_Vector2D& pos ) const = 0;
    virtual void SimToMosMgrsCoord( const MT_Vector2D& pos, double& latitude, double& longitude ) const = 0;

    virtual double GetWeldValue() const = 0;
    virtual double GetWidth    () const = 0;
    virtual double GetHeight   () const = 0;
    virtual const MT_Rect& GetExtent() const = 0;

    virtual bool IsValidPosition( const MT_Vector2D& pos ) const = 0;
    virtual void ClipPointInsideWorld( MT_Vector2D& pos ) const = 0;
    virtual MT_Vector2D ClipPointInsideWorld( const MT_Vector2D& pos ) const = 0;
    //@}

};

#endif // TER_World_ABC_h
