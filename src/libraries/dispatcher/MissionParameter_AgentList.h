// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_AgentList_h_
#define __MissionParameter_AgentList_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_AgentList
    @brief  MissionParameter_AgentList
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_AgentList : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_AgentList( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_AgentList();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_AgentList( const MissionParameter_AgentList& );            //!< Copy constructor
    MissionParameter_AgentList& operator=( const MissionParameter_AgentList& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< unsigned int > T_OIDVector;
    //@}

private:
    //! @name Member data
    //@{
    T_OIDVector agents_;
    //@}
};

}

#endif // __MissionParameter_AgentList_h_
