// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Profile_h_
#define __Profile_h_

#include "Profile_ABC.h"
#include <set>

namespace MsgsAuthenticationToClient
{
    class MsgProfile;
    class MsgProfileDescription;
}

namespace MsgsClientToSim
{
    class MsgClientToSim;
}

namespace MsgsClientToAuthentication
{
    class MsgProfileCreationRequest;
    class MsgProfileUpdateRequest;
}

namespace MsgsClientToReplay
{
    class MsgClientToReplay;
}

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Automat_ABC;
    class Team_ABC;
    class Formation_ABC;
    class Population_ABC;
}

namespace dispatcher
{
    class Model;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  Profile
    @brief  Profile
*/
// Created: NLD 2006-09-19
// =============================================================================
class Profile : public Profile_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Profile( const Model& model, ClientPublisher_ABC& clients, const std::string& strLogin, xml::xistream& xis );
             Profile( const Model& model, ClientPublisher_ABC& clients, const MsgsClientToAuthentication::MsgProfileCreationRequest& message );
    virtual ~Profile();
    //@}

    //! @name Main
    //@{
    bool CheckPassword( const std::string& strPassword ) const;
    bool CheckRights( const MsgsClientToSim::MsgClientToSim& msg ) const;
    bool CheckRights( const MsgsClientToAuthentication::MsgClientToAuthentication& msg ) const;
    bool CheckRights( const MsgsClientToReplay::MsgClientToReplay& msg ) const;
    bool CheckRights( const Common::MsgChatTarget& source, const Common::MsgChatTarget& target ) const;
    //@}

    //! @name Accessors
    //@{
    virtual std::string GetName() const;
    //@}

    //! @name Network
    //@{
    void Send( MsgsAuthenticationToClient::MsgProfile& asn ) const;
    void Send( MsgsAuthenticationToClient::MsgProfileDescription& asn ) const;
    void SendCreation( ClientPublisher_ABC& publisher ) const;
    //@}

    //! @name Operations
    //@{
    void Update( const MsgsClientToAuthentication::MsgProfileUpdateRequest& message );
    void SetRight( const kernel::Automat_ABC& entity, bool readonly, bool readwrite );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< const kernel::Automat_ABC* > T_AutomatSet;
    typedef std::set< const kernel::Team_ABC* > T_SideSet;
    typedef std::set< const kernel::Formation_ABC* > T_FormationSet;
    typedef std::set< const kernel::Population_ABC* > T_PopulationSet;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Profile( const Profile& );            //!< Copy constructor
    Profile& operator=( const Profile& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadAutomatRights( xml::xistream& xis, T_AutomatSet& container );
    void ReadSideRights( xml::xistream& xis, T_SideSet& container );
    void ReadFormationRights( xml::xistream& xis, T_FormationSet& container );
    void ReadPopulationRights( xml::xistream& xis, T_PopulationSet& container );
    void ReadRights( const MsgsAuthenticationToClient::MsgProfile& message );
    //@}

private:
    //! @name Member data
    //@{
    const Model& model_;
    ClientPublisher_ABC& clients_;
    std::string strLogin_;
    std::string strPassword_;
    int role_;

    // Read only
    T_AutomatSet readOnlyAutomats_;
    T_SideSet readOnlySides_;
    T_FormationSet readOnlyFormations_;
    T_PopulationSet readOnlyPopulations_;

    // Read write
    T_AutomatSet readWriteAutomats_;
    T_SideSet readWriteSides_;
    T_FormationSet readWriteFormations_;
    T_PopulationSet readWritePopulations_;

    // Supervision
    bool  bSupervision_;
    //@}
};

}

#endif // __Profile_h_
