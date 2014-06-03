// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __NewClass_h_
#define __NewClass_h_

#include <tools/Resolver_ABC.h>

namespace xml
{
    class xistream;
    class xostream;
}

namespace kernel
{
    class Entity_ABC;

// =============================================================================
/** @class  UserRights
    @brief  UserRights
*/
// Created: LDC 2012-05-07
// =============================================================================
class UserRights
{

public:
    //! @ñame Types
    //@{
    typedef std::vector< unsigned long > T_Ids;
    typedef T_Ids::const_iterator      CIT_Ids;

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

public:
    //! @name Constructors/Destructor
    //@{
             UserRights();
    virtual ~UserRights();
    //@}

    //! @name Operations
    //@{
    bool IsReadable( const kernel::Entity_ABC& entity ) const;
    bool IsWriteable( const kernel::Entity_ABC& entity ) const;
    void SetReadable( const kernel::Entity_ABC& entity, bool readable );
    void SetWriteable( const kernel::Entity_ABC& entity, bool writeable );
    bool FindSide( unsigned long id ) const;
    void Clear();
    void Read( xml::xistream& xis, const ExistenceChecker_ABC& teamChecker, const ExistenceChecker_ABC& formationChecker, const ExistenceChecker_ABC& automatChecker, const ExistenceChecker_ABC& populationChecker, const ExistenceChecker_ABC& ghostChecker );
    void Serialize( xml::xostream& xos ) const;
    void NotifyTeamDeleted( unsigned long teamId );
    void NotifyFormationDeleted( unsigned long formationId );
    void NotifyAutomatDeleted( unsigned long automatId );
    void NotifyPopulationDeleted( unsigned long populationId );
    void NotifyGhostDeleted( unsigned long ghostsId );
    void InsertWriteSides( T_Ids& elements ) const;
    void InsertWriteAutomats( T_Ids& elements ) const;
    void InsertWritePopulations( T_Ids& elements ) const;
    void InsertWriteGhosts( T_Ids& elements ) const;

    const T_Ids& GetWriteSides() const;
    const T_Ids& GetWriteAutomats() const;
    const T_Ids& GetWritePopulations() const;
    const T_Ids& GetWriteFormations() const;
    const T_Ids& GetWriteGhosts() const;
    const T_Ids& GetReadSides() const;
    const T_Ids& GetReadAutomats() const;
    const T_Ids& GetReadPopulations() const;
    const T_Ids& GetReadFormations() const;
    const T_Ids& GetReadGhosts() const;

    template< typename List > void SetWriteSides( const List& ids ) { Set( ids, writeSides_ ); }
    template< typename List > void SetWriteAutomats( const List& ids ) { Set( ids, writeAutomats_ ); }
    template< typename List > void SetWritePopulations( const List& ids ) { Set( ids, writePopulations_ ); }
    template< typename List > void SetWriteFormations( const List& ids ) { Set( ids, writeFormations_ ); }
    template< typename List > void SetWriteGhosts( const List& ids ) { Set( ids, writeGhosts_ ); }
    template< typename List > void SetReadSides( const List& ids ) { Set( ids, readSides_ ); }
    template< typename List > void SetReadAutomats( const List& ids ) { Set( ids, readAutomats_ ); }
    template< typename List > void SetReadPopulations( const List& ids ) { Set( ids, readPopulations_ ); }
    template< typename List > void SetReadFormations( const List& ids ) { Set( ids, readFormations_ ); }
    template< typename List > void SetReadGhosts( const List& ids ) { Set( ids, readGhosts_ ); }

    bool operator==( const UserRights& ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadRights( xml::xistream& xis, T_Ids& list, const ExistenceChecker_ABC& checker );
    void ReadAutomatRights( xml::xistream& xis, T_Ids& automatList, const ExistenceChecker_ABC& automatChecker, T_Ids& ghostList, const ExistenceChecker_ABC& ghostChecker );
    void SerializeRights( xml::xostream& xos, const std::string& tag, const T_Ids& list ) const;
    template< typename List > void Set( const List& from, T_Ids& to )
    {
        to.clear();
        if( from.elem_size() )
            to.reserve( from.elem_size() );
        for( int i = 0; i < from.elem_size(); ++i )
            to.push_back( from.elem(i).id() );
        std::sort( to.begin(), to.end() );
    }
    //@}

private:
    //! @name Member data
    //@{
    T_Ids readSides_;
    T_Ids writeSides_;
    T_Ids readAutomats_;
    T_Ids writeAutomats_;
    T_Ids readPopulations_;
    T_Ids writePopulations_;
    T_Ids readFormations_;
    T_Ids writeFormations_;
    T_Ids readGhosts_;
    T_Ids writeGhosts_;
    //@}
};

}

#endif // __NewClass_h_
