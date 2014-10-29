// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef SIMULATION_TERRAIN_PATH_ABC
#define SIMULATION_TERRAIN_PATH_ABC

class TerrainData;
class MT_Vector2D;
class TER_Pathfinder_ABC;

// =============================================================================
// @class  TER_Path_ABC
// Created: NLD 2005-02-22
// =============================================================================
class TER_Path_ABC : private boost::noncopyable
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
    virtual void Cancel() = 0;
    //@}

    //! @name Accessors
    //@{
    unsigned int GetID() const;
    virtual E_State GetState() const = 0;
    virtual double GetLength() const = 0;
    //@}

    //! @name Operators
    //@{
    bool operator==( const TER_Path_ABC& rhs ) const;
    bool operator!=( const TER_Path_ABC& rhs ) const;
    //@}

protected:
    //! @name Constructor / Destructor
    //@{
             TER_Path_ABC();
    virtual ~TER_Path_ABC();
    //@}

private:
    const unsigned int nID_;
};

#endif // SIMULATION_TERRAIN_PATH_ABC
