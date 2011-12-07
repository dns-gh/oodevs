// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfile_h_
#define __UserProfile_h_

namespace kernel
{
    class Controller;
    class Entity_ABC;
}

namespace xml
{
    class xistream;
    class xostream;
}

class Model;

// =============================================================================
/** @class  UserProfile
    @brief  UserProfile
*/
// Created: SBO 2007-01-16
// =============================================================================
class UserProfile
{
public:
    //! @name Constructors/Destructor
    //@{
             UserProfile( xml::xistream& xis, kernel::Controller& controller, const Model& model );
             UserProfile( const QString& login, kernel::Controller& controller, const Model& model );
             UserProfile( const UserProfile& );
    virtual ~UserProfile();
    //@}

    //! @name Accessors
    //@{
    QString GetLogin() const;
    QString GetPassword() const;
    bool IsSupervisor() const;
    bool IsReadable( const kernel::Entity_ABC& entity ) const;
    bool IsWriteable( const kernel::Entity_ABC& entity ) const;
    void Visit( std::vector< unsigned long >& elements ) const;
    //@}

    //! @name Setters
    //@{
    virtual void SetLogin( const QString& value );
    void SetPassword( const QString& value );
    void SetSupervisor( bool value );
    void SetReadable( const kernel::Entity_ABC& entity, bool readable );
    void SetWriteable( const kernel::Entity_ABC& entity, bool writeable );
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    UserProfile& operator=( const UserProfile& );
    void NotifyTeamDeleted( unsigned long teamId );
    void NotifyFormationDeleted( unsigned long formationId );
    void NotifyAutomatDeleted( unsigned long automatId );
    void NotifyPopulationDeleted( unsigned long populationId );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< unsigned long > T_Ids;

    struct ExistenceChecker_ABC
    {
        virtual bool operator()( unsigned long id ) const = 0;
    };

    template< typename Resolver >
    struct ExistenceChecker : public ExistenceChecker_ABC
    {
        explicit ExistenceChecker( const Resolver& resolver ) : resolver_( &resolver ) {}
        virtual bool operator()( unsigned long id ) const
        {
            return resolver_->Find( id ) != 0;
        }
        const Resolver* resolver_;
    };
    //@}

    //! @name Helpers
    //@{
    void ReadRights( xml::xistream& xis, T_Ids& list, const ExistenceChecker_ABC& checker );
    void SerializeRights( xml::xostream& xos, const std::string& tag, const T_Ids& list ) const;
    void SetRight( unsigned long id, T_Ids& ids, bool status );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const Model& model_;
    QString login_;
    QString password_;
    bool supervisor_;
    T_Ids readSides_;
    T_Ids readFormations_;
    T_Ids readAutomats_;
    T_Ids readPopulations_;
    T_Ids writeSides_;
    T_Ids writeFormations_;
    T_Ids writeAutomats_;
    T_Ids writePopulations_;
    bool isClone_;
    //@}
};

#endif // __UserProfile_h_
