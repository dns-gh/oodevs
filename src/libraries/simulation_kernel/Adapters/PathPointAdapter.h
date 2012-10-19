// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PATH_POINT_ADAPTER_H
#define SWORD_PATH_POINT_ADAPTER_H

#include <boost/shared_ptr.hpp>
#include <string>

class MT_Vector2D;

namespace sword
{
namespace movement
{
    class PathPoint;
}
// =============================================================================
/** @class  PathPointAdapter
    @brief  Path point adapter
*/
// Created: MCO 2012-01-26
// =============================================================================
class PathPointAdapter : boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
             PathPointAdapter( unsigned int entity, boost::shared_ptr< movement::PathPoint > point );
    virtual ~PathPointAdapter();
    //@}

public:
    //! @name Accessors
    //@{
    void RemoveFromPointsCategory() const;
    MT_Vector2D* GetRepPoint() const;
    int GetTypePoint() const;
    bool IsAvantPoint() const;
    bool IsPoint() const;
    boost::shared_ptr< PathPointAdapter > GetDestPoint() const;
    int GetTypeLimaPoint() const;
    unsigned int GetLimaPoint() const;
    const std::string& GetDIAType() const;
    const MT_Vector2D& GetPos() const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int entity_;
    boost::shared_ptr< movement::PathPoint > point_;
    mutable std::string type_;
    //@}
};

inline boost::shared_ptr< PathPointAdapter > AdaptPoint( unsigned int entity, boost::shared_ptr< movement::PathPoint > point )
{
    return boost::shared_ptr< PathPointAdapter >( new PathPointAdapter( entity, point ) );
}

}

#endif // SWORD_PATH_POINT_ADAPTER_H
