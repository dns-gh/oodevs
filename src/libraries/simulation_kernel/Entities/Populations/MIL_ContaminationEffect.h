// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ContaminationEffect_h_
#define __MIL_ContaminationEffect_h_

class MIL_PopulationHumans;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_ContaminationEffect
    @brief  MIL_ContaminationEffect
*/
// Created: LGY 2011-04-29
// =============================================================================
class MIL_ContaminationEffect : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_ContaminationEffect( MIL_PopulationHumans& humans, double delay, unsigned int time );
             MIL_ContaminationEffect();
    virtual ~MIL_ContaminationEffect();
    //@}

    //! @name Operations
    //@{
    void Update( unsigned int time );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

private:
    //! @name Member data
    //@{
    double delay_;
    MIL_PopulationHumans* humans_;
    unsigned int time_;
    //@}
};

#endif // __MIL_ContaminationEffect_h_
