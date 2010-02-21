// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_ObjectKnowledgeList_h_
#define __MissionParameter_ObjectKnowledgeList_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_ObjectKnowledgeList
    @brief  MissionParameter_ObjectKnowledgeList
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_ObjectKnowledgeList : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_ObjectKnowledgeList( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_ObjectKnowledgeList();
    //@}

    //! @name Operations
    //@{
    virtual void Send  ( Common::MsgMissionParameter& asn ) const;
    virtual void Delete( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_ObjectKnowledgeList( const MissionParameter_ObjectKnowledgeList& );            //!< Copy constructor
    MissionParameter_ObjectKnowledgeList& operator=( const MissionParameter_ObjectKnowledgeList& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< int > objectKnowledges_;
    //@}
};

}

#endif // __MissionParameter_ObjectKnowledgeList_h_
