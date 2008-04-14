// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __IntelligenceManager_h_
#define __IntelligenceManager_h_

#include "game_asn/Messenger.h"
#include <map>
#include <set>

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace messenger
{

class Intelligence;
class IdManager;
class Entity_ABC;

// =============================================================================
/** @class  IntelligenceManager
    @brief  IntelligenceManager
*/
// Created: RDS 2008-04-07
// =============================================================================
class IntelligenceManager
{

public:
    //! @name Constructors/Destructor
    //@{
             IntelligenceManager(IdManager&, const kernel::CoordinateConverter_ABC& converter);
    virtual ~IntelligenceManager();
    //@}

    //! @name Network
    //@{
    void OnReceive            ( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgIntelligenceCreationRequest&     asn);
    void OnReceive            ( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgIntelligenceUpdateRequest&       asn);
    void OnReceive            ( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgIntelligenceDestructionRequest&  asn);
    void SendStateToNewClient ( dispatcher::ClientPublisher_ABC& publisher) const;
    //@}

    //! @name xml read / write
    //@{
            void ReadIntelligence(xml::xistream& xis, const ASN1T_Formation& formation);
    virtual void Write           (xml::xostream& xos) ;
    //@}

    typedef std::map< unsigned int , std::set< Entity_ABC* > > T_EntityCollectorMap ;
    typedef T_EntityCollectorMap                               T_FormationMap ;
    void CollectFormations(T_FormationMap&);

private:
    //! @name Copy/Assignment
    //@{
    IntelligenceManager( const IntelligenceManager& );            //!< Copy constructor
    IntelligenceManager& operator=( const IntelligenceManager& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned int, Intelligence* > T_IntelligenceMap;
    typedef T_IntelligenceMap::iterator             IT_IntelligenceMap;
    typedef T_IntelligenceMap::const_iterator       CIT_IntelligenceMap;
    //@}


    //! @name Members
    //@{
    T_IntelligenceMap intelligences_;
    IdManager& idManager_;
    const kernel::CoordinateConverter_ABC& converter_ ;
    //@}
};

}

#endif // __IntelligenceManager_h_
