// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_PopulationMission_h_
#define __MIL_PopulationMission_h_

#include "MIL_Mission_ABC.h"

namespace sword
{
    class CrowdOrder;
}

class MIL_Population;

// =============================================================================
/** @class  MIL_PopulationMission
    @brief  MIL_PopulationMission
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_PopulationMission : public MIL_Mission_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
            MIL_PopulationMission( const MIL_MissionType_ABC& type, MIL_Population& population, const sword::CrowdOrder& asn );
            MIL_PopulationMission( const MIL_MissionType_ABC& type, MIL_Population& population );
    virtual ~MIL_PopulationMission();
    //@}


    //! @name Accessors
    //@{
    virtual unsigned int GetOwnerId() const;
    //@}

    //! @name Operations
    //@{
    virtual void Start( boost::shared_ptr< MIL_Mission_ABC > self );
    virtual void Stop( boost::shared_ptr< MIL_Mission_ABC > self );
    virtual bool IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const;
    virtual void Send() const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@}

private:
    //! @name Serialization
    //@{
    template< typename Archive > friend void save_construct_data( Archive& archive, const MIL_PopulationMission* mission, const unsigned int version );
    template< typename Archive > friend void load_construct_data( Archive& archive, MIL_PopulationMission* mission, const unsigned int version );
    //@}

    //! @name Helpers
    //@{
    static void SendNoMission( const MIL_Population& population );
    //@}

private:
    //! @name Member data
    //@{
    MIL_Population& population_;
    bool            bDIABehaviorActivated_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_PopulationMission )

#endif // __MIL_PopulationMission_h_
