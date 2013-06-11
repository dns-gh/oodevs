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
    class ExtensionTypes;
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
             UserProfile( xml::xistream& xis, kernel::Controller& controller, const Model& model,
                          const kernel::ExtensionTypes& extensions );
             UserProfile( const QString& login, kernel::Controller& controller, const Model& model,
                          const kernel::ExtensionTypes& extensions );
             UserProfile( const QString& login, const std::string& role, kernel::Controller& controller,
                          const Model& model, const kernel::ExtensionTypes& extensions );
             UserProfile( const UserProfile& );
    virtual ~UserProfile();
    //@}

    //! @name Accessors
    //@{
    QString GetLogin() const;
    QString GetPassword() const;
    QString GetRole() const;
    bool CanControlTime() const;
    bool IsLowLevel() const;
    bool IsReadable( const kernel::Entity_ABC& entity ) const;
    bool IsWriteable( const kernel::Entity_ABC& entity ) const;
    const std::string& GetUserRole() const;
    void Visit( std::vector< unsigned long >& elements ) const;
    void VisitAllAutomats( std::set< unsigned long >& elements ) const;
    unsigned int GetWriteProfilesCount();
    unsigned int GetAutomatAndPopulationsProfilesCount() const;
    unsigned int GetProfilesCount() const;
    //@}

    //! @name Setters
    //@{
    virtual void SetLogin( const QString& value );
    virtual void SetUserRole( const std::string& role );
    void SetPassword( const QString& value );
    void SetReadable( const kernel::Entity_ABC& entity, bool readable );
    void SetWriteable( const kernel::Entity_ABC& entity, bool writeable );
    void SetTimeControl( bool control );
    void Clear();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos );
    UserProfile& operator=( const UserProfile& );
    void NotifyTeamDeleted( unsigned long teamId );
    void NotifyFormationDeleted( unsigned long formationId );
    void NotifyAutomatDeleted( unsigned long automatId );
    void NotifyPopulationDeleted( unsigned long populationId );
    void NotifyGhostDeleted( unsigned long ghostId );
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
    void ReadAutomatRights( xml::xistream& xis, T_Ids& list, const ExistenceChecker_ABC& checker, T_Ids& ghostList, const ExistenceChecker_ABC& ghostChecker );
    void SerializeRights( xml::xostream& xos, const std::string& tag, const T_Ids& list ) const;
    void SetRight( unsigned long id, T_Ids& ids, bool status );
    bool HasProperty( const std::string& name ) const;
    void ComputeLowLevel();
    void UpdateRightsWithSuperior( T_Ids& ids, T_Ids& teamIds );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const Model& model_;
    const kernel::ExtensionTypes& extensions_;
    QString login_;
    QString password_;
    T_Ids readSides_;
    T_Ids readFormations_;
    T_Ids readAutomats_;
    T_Ids readPopulations_;
    T_Ids readGhosts_;
    T_Ids writeSides_;
    T_Ids writeFormations_;
    T_Ids writeAutomats_;
    T_Ids writePopulations_;
    T_Ids writeGhosts_;
    bool isClone_;
    bool isLowLevel_;
    std::string userRole_;
    bool controlTime_;
    //@}
};

#endif // __UserProfile_h_
