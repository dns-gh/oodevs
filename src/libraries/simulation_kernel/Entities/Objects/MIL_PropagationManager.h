// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_PropagationManager_h_
#define __MIL_PropagationManager_h_

#include <set>

class TER_Localisation;

// =============================================================================
/** @class  MIL_PropagationManager
    @brief  MIL_PropagationManager
*/
// Created: RFT 28/04/2008
// Modified: RFT 05/05/2008
// =============================================================================
class MIL_PropagationManager : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_PropagationManager();
    virtual ~MIL_PropagationManager();
    //@}

    //! @name Methods
    //@{
    void Flag( const MT_Vector2D& vOrigin , unsigned int length , unsigned int width );
    void RemoveFlag( const MT_Vector2D& vOrigin , unsigned int length , unsigned int width );
    bool IsFlagged( const TER_Localisation& localisation , unsigned int length , unsigned int width ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< int, int > T_Coordinate;
    typedef std::set< T_Coordinate > T_CoordinatesSet;
    //@}

private:
    T_CoordinatesSet coordinatesSet_;
};

// BOOST_CLASS_EXPORT_KEY( MIL_PropagationManager )

#endif // __MIL_PropagationManager_h_
