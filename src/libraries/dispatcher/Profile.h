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

#include "AsnTypes.h"

namespace xml{ class xistream; };

namespace dispatcher
{
class Dispatcher;
class Automat;
class Side;
class Formation;
class Population;
class Publisher_ABC;

// =============================================================================
/** @class  Profile
    @brief  Profile
*/
// Created: NLD 2006-09-19
// =============================================================================
class Profile
{
public:
    //! @name Constructors/Destructor
    //@{
             Profile( Dispatcher& dispatcher, const std::string& strLogin, xml::xistream& xis );
             Profile( Dispatcher& dispatcher, const ASN1T_MsgProfileCreationRequest& message );
    virtual ~Profile();
    //@}

    //! @name Main
    //@{
    bool CheckPassword( const std::string& strPassword ) const;
    bool CheckRights  ( const ASN1T_MsgsOutClient& msg ) const;
    //@}

    //! @name Network
    //@{
    void Send( ASN1T_Profile& asn ) const;
    static void AsnDelete( ASN1T_Profile& asn );
    //@}

    //! @name Operations
    //@{
    void SendCreation( Publisher_ABC& publisher ) const;
    void Update( const ASN1T_MsgProfileUpdateRequest& message );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< Automat* >    T_AutomatSet;
    typedef std::set< Side* >       T_SideSet;
    typedef std::set< Formation* >  T_FormationSet;
    typedef std::set< Population* > T_PopulationSet;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Profile( const Profile& );            //!< Copy constructor
    Profile& operator=( const Profile& ); //!< Assignement operator
    //@}

    //! @name Initialization
    //@{
    void ReadAutomatRights   ( xml::xistream& xis, T_AutomatSet&    container );
    void ReadSideRights      ( xml::xistream& xis, T_SideSet&       container );
    void ReadFormationRights ( xml::xistream& xis, T_FormationSet&  container );
    void ReadPopulationRights( xml::xistream& xis, T_PopulationSet& container );
    //@}

    //! @name Helpers
    //@{
    void ReadRights( const ASN1T_Profile& message );
    //@}

private:
    Dispatcher&     dispatcher_;
    std::string     strLogin_;
    std::string     strPassword_;

    // Read only
    T_AutomatSet    readOnlyAutomats_;
    T_SideSet       readOnlySides_;
    T_FormationSet  readOnlyFormations_;
    T_PopulationSet readOnlyPopulations_;

    // Read write
    T_AutomatSet    readWriteAutomats_;
    T_SideSet       readWriteSides_;
    T_FormationSet  readWriteFormations_;
    T_PopulationSet readWritePopulations_;

    // Supervision
    bool            bSupervision_;
};

}

#endif // __Profile_h_
