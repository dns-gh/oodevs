// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLineManager_h_
#define __TacticalLineManager_h_

#include "game_asn/Messenger.h"

#include "TacticalLine_ABC.h"

#include <map>

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace kernel
{
    class CoordinateConverter_ABC ;
}

namespace xml
{
    class xistream ;
    class xostream ;
}

namespace messenger
{

class Lima;
class Limit;
class IdManager;

// =============================================================================
/** @class  TacticalLineManager
    @brief  TacticalLineManager
*/
// Created: NLD 2006-11-10
// =============================================================================
class TacticalLineManager
{
public:
    //! @name Constructors/Destructor
    //@{
             TacticalLineManager(IdManager& idManager, const kernel::CoordinateConverter_ABC& converter);
    virtual ~TacticalLineManager();
    //@}


    //! @name Network
    //@{
    void OnReceiveLimit       ( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgLimitCreationRequest&    asn);
    void OnReceiveLimit       ( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients,const ASN1T_MsgLimitDestructionRequest& asn);
    void OnReceiveLimit       ( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgLimitUpdateRequest&      asn);
    void OnReceiveLima        ( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgLimaCreationRequest&     asn);
    void OnReceiveLima        ( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgLimaDestructionRequest&  asn);
    void OnReceiveLima        ( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgLimaUpdateRequest&       asn);
    void SendStateToNewClient ( dispatcher::ClientPublisher_ABC& publisher ) const;
    //@}

    //! @name xml read / write
    //@{
            void ReadLima  (xml::xistream& xis, const ASN1T_TacticalLinesDiffusion& diffusion);
            void ReadLimit (xml::xistream& xis, const ASN1T_TacticalLinesDiffusion& diffusion);
    virtual void Write     (xml::xostream& xos) ;
    //@}

    typedef std::map< unsigned int , std::set< Entity_ABC* > > T_TacticalLineCollectorMap ;
    typedef T_TacticalLineCollectorMap T_FormationMap ;
    typedef T_TacticalLineCollectorMap T_AutomatMap ;

    void CollectFormations(T_FormationMap&);
    void CollectAutomats(T_AutomatMap&);

private:

    //! @name Copy/Assignement
    //@{
    TacticalLineManager( const TacticalLineManager& );            //!< Copy constructor
    TacticalLineManager& operator=( const TacticalLineManager& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, Limit* > T_LimitMap;
    typedef T_LimitMap::iterator             IT_LimitMap;
    typedef T_LimitMap::const_iterator       CIT_LimitMap;

    typedef std::map< unsigned int, Lima* >  T_LimaMap;
    typedef T_LimaMap::iterator              IT_LimaMap;
    typedef T_LimaMap::const_iterator        CIT_LimaMap;

    //@}


    //! @name Members
    //@{
    IdManager& idManager_;
    const kernel::CoordinateConverter_ABC& converter_ ;
    T_LimitMap      limits_;
    T_LimaMap       limas_;
    //@}

};

}

#endif // __TacticalLineManager_h_
