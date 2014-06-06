// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __AutomateFactory_h_
#define __AutomateFactory_h_

#include "AutomateFactory_ABC.h"
#include "Checkpoints/SerializationTools.h"

namespace sword
{
    class DEC_Logger;
}

class MIL_IDManager;
class MissionController_ABC;

// =============================================================================
/** @class  AutomateFactory
    @brief  AutomateFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class AutomateFactory : public AutomateFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomateFactory( MIL_IDManager& idManager, MissionController_ABC& controller, unsigned int gcPause, unsigned int gcMult, bool logEnabled );
    virtual ~AutomateFactory();
    //@}

    //! @name Operations
    //@{
    virtual MIL_Automate& Create( xml::xistream& xis, MIL_Entity_ABC& parent );
    virtual MIL_Automate& Create( const MIL_AutomateType& type, unsigned int knowledgeGroup, const std::string& name, MIL_Entity_ABC& parent, unsigned int context, const MIL_DictionaryExtensions& extensions );
    //@}

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;

private:
    //! @name CheckPoint
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const AutomateFactory* factory, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, AutomateFactory* factory, const unsigned int /*version*/ );
    AutomateFactory( MIL_IDManager& idManager, MissionController_ABC& controller, unsigned int gcPause, unsigned int gcMult, std::unique_ptr< sword::DEC_Logger > logger );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int gcPause_;
    unsigned int gcMult_;
    std::unique_ptr< sword::DEC_Logger > logger_;
    MIL_IDManager& idManager_;
    MissionController_ABC& controller_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( AutomateFactory )

#endif // __AutomateFactory_h_
