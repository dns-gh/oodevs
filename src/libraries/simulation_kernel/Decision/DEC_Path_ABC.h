//*****************************************************************************
//
// $Created: JDY 03-02-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path_ABC.h $
// $Author: Age $
// $Modtime: 28/02/05 10:04 $
// $Revision: 2 $
// $Workfile: DEC_Path_ABC.h $
//
//*****************************************************************************

#ifndef __DEC_Path_ABC_h_
#define __DEC_Path_ABC_h_

#include "simulation_terrain/TER_PathFindRequest_ABC.h"
#include <boost/noncopyable.hpp>

class TerrainData;
class MT_Vector2D;

// =============================================================================
// @class  DEC_Path_ABC
// Created: NLD 2005-02-22
// =============================================================================
class DEC_Path_ABC : public TER_PathFindRequest_ABC
                   , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    enum E_State
    {
        eInvalid    = 0,
        eComputing  = 1,
        eValid      = 2,
        eImpossible = 4,
        ePartial    = 8,
        eCanceled   = 16
    };
    //@}

public:
    //! @name Operations
    //@{
    virtual double GetLength() const = 0;
    //@}

    //! @name Accessors
    //@{
    unsigned int GetID() const;
    virtual E_State GetState() const = 0;
    //@}

    //! @name Operators
    //@{
    bool operator==( const DEC_Path_ABC& rhs ) const;
    bool operator!=( const DEC_Path_ABC& rhs ) const;
    //@}

protected:
    //! @name Constructor / Destructor
    //@{
             DEC_Path_ABC();
    virtual ~DEC_Path_ABC();
    //@}

private:
    const unsigned int nID_;

private:
    static unsigned int nIDIdx_;
};

#include "DEC_Path_ABC.inl"

#endif // __DEC_Path_ABC_h_
