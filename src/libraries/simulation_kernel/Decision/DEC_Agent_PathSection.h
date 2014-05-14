// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Agent_PathSection_h_
#define __DEC_Agent_PathSection_h_

#include "Decision/DEC_PathSection_ABC.h"

class DEC_PathResult_ABC;
class DEC_Agent_PathfinderPath;
class TerrainRule_ABC;

// =============================================================================
// @class  DEC_Tools
// Created: NLD 2005-02-22
// =============================================================================
class DEC_Agent_PathSection : public DEC_PathSection_ABC
{
public:
             DEC_Agent_PathSection( DEC_PathResult_ABC& result, const DEC_Agent_PathfinderPath& path,
                 const MT_Vector2D& vStartPoint, const MT_Vector2D& vEndPoint, bool needRefine, bool useStrictClosest );
    virtual ~DEC_Agent_PathSection();

private:
    //! @name Accessors
    //@{
    virtual TerrainRule_ABC& GetRule() const;
    //@}

private:
    std::auto_ptr< TerrainRule_ABC > rule_;
};

#endif // __DEC_Agent_PathSection_h_