// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PHY_PerceptionWithLocation_h_
#define __PHY_PerceptionWithLocation_h_

#include "PHY_Perception_ABC.h"

// =============================================================================
/** @class  PHY_PerceptionWithLocation
    @brief  PHY_PerceptionWithLocation
*/
// Created: LDC 2009-07-29
// =============================================================================
template <typename T > class PHY_PerceptionWithLocation : public PHY_Perception_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_PerceptionWithLocation( PHY_RolePion_Perceiver& perceiver );
    virtual ~PHY_PerceptionWithLocation();
    //@}

    //! @name Operations
    //@{
    void Remove( int id );
    int  Add   ( T* pLocation );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_PerceptionWithLocation( const PHY_PerceptionWithLocation& );            //!< Copy constructor
    PHY_PerceptionWithLocation& operator=( const PHY_PerceptionWithLocation& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

protected:
    //! @name Types
    //@{
    typedef std::vector< T* >                     T_RecoVector;
    typedef typename T_RecoVector::iterator       IT_RecoVector;
    typedef typename T_RecoVector::const_iterator CIT_RecoVector;
    //@}

    //! @name Member data
    //@{
    T_RecoVector recos_;
    //@}
};

#include "PHY_PerceptionWithLocation.inl"

#endif // __PHY_PerceptionWithLocation_h_
