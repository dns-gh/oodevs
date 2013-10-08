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
#include <tools/Set.h>

namespace sword
{
    class Profile;
    class ProfileDescription;
    class ClientToSim;
    class ProfileCreationRequest;
    class ProfileUpdateRequest;
    class ClientToReplay;
    class ConnectedProfileList;
    class AuthenticationResponse;
}

namespace xml
{
    class xistream;
    class xostream;
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
             Profile( const Model& model, ClientPublisher_ABC& clients, const sword::ProfileCreationRequest& message );
             Profile( const Model& model, ClientPublisher_ABC& clients, const std::string& strLogin, bool createdOnNetwork );
    virtual ~Profile();
    //@}

    //! @name Main
    //@{
    bool CheckPassword( const std::string& strPassword ) const;
    virtual bool CheckRights( const sword::ClientToSim& msg ) const;
    virtual bool CheckRights( const sword::ClientToAuthentication& msg ) const;
    virtual bool CheckRights( const sword::ClientToReplay& msg ) const;
    virtual bool CheckRights( const sword::ChatTarget& source, const sword::ChatTarget& target ) const;
    //@}

    //! @name Accessors
    //@{
    virtual std::string GetName() const;
    //@}

    //! @name Network
    //@{
    virtual void Send( sword::ConnectedProfileList& msg ) const;
    virtual void Send( sword::AuthenticationResponse& msg ) const;
    virtual void Send( sword::ProfileDescription& msg ) const;
    virtual void SendCreation( ClientPublisher_ABC& publisher ) const;
    //@}

    //! @name Operations
    //@{
    void Update( const sword::ProfileUpdateRequest& message );
    void SetRight( const kernel::Automat_ABC& entity, bool readonly, bool readwrite );
    void SerializeProfile( xml::xostream& xos ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef tools::Set< const kernel::Automat_ABC* > T_AutomatSet;
    typedef tools::Set< const kernel::Team_ABC* > T_SideSet;
    typedef tools::Set< const kernel::Formation_ABC* > T_FormationSet;
    typedef tools::Set< const kernel::Population_ABC* > T_PopulationSet;
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
    void ReadRights( const sword::Profile& message );
    void Send( sword::Profile& msg ) const;

    template< typename T >
    void SerializeRights( xml::xostream& xos, const std::string& tag, const T& list ) const;
    //@}

private:
    //! @name Member data
    //@{
    const Model& model_;
    ClientPublisher_ABC& clients_;
    std::string strLogin_;
    std::string strPassword_;

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
    bool bSupervision_;
    bool createdOnNetwork_;
    //@}
};

}

#endif // __Profile_h_
