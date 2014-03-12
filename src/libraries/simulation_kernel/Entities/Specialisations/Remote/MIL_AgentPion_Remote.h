// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __MIL_AgentPion_Remote_h_
#define __MIL_AgentPion_Remote_h_

#include "Entities/Agents/MIL_AgentPion.h"

class MIL_AgentPion_Remote : public MIL_AgentPion
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_AgentPion_Remote( const MIL_AgentTypePion& type, const AlgorithmsFactories& algorithmFactories, MissionController_ABC& controller, MIL_Automate& automate, xml::xistream& xis );
             MIL_AgentPion_Remote( const MIL_AgentTypePion& type, const AlgorithmsFactories& algorithmFactories, MissionController_ABC& controller );
    virtual ~MIL_AgentPion_Remote();
    //@}
    //! @name CheckPoints
    //@{
    template<class Archive>
    void serialize( Archive & ar, const unsigned int version )
    {
        boost::serialization::void_cast_register< MIL_AgentPion_Remote, MIL_AgentPion >();
        boost::serialization::split_member( ar, *this, version );
    }

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;

    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( MIL_AgentPion_Remote )
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_AgentPion_Remote )

#endif // __MIL_AgentPion_Remote_h_
