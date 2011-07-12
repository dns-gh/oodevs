// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "MT_Tools/RoleContainer.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#pragma warning( push )
#pragma warning( disable : 4094 )
#include <boost/serialization/export.hpp>
#pragma warning( pop )
#include <boost/serialization/vector.hpp>
#include <boost/serialization/split_member.hpp>
#include <fstream>

typedef boost::archive::text_oarchive OutputArchive;
typedef boost::archive::text_iarchive InputArchive;

namespace
{
    class MyEntity : public tools::RoleContainer
    {
    public:
                 MyEntity() {}
        virtual ~MyEntity() {}
    };
}

class MyRole : public tools::Role_ABC
{
public:
             MyRole() : entity_( 0 ), id_( 0 ) {}
    explicit MyRole( MyEntity& entity ) : entity_( &entity ), id_( 4242 ) {}
    virtual ~MyRole() {}

    unsigned int GetId() const { return id_; }

private:
    friend class boost::serialization::access;
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( InputArchive& archive, const unsigned int )
    {
        archive >> boost::serialization::base_object< tools::Role_ABC >( *this )
                >> entity_
                >> id_;
        entity_->Attach( *this );
    }
    void save( OutputArchive& archive, const unsigned int ) const
    {
        archive << boost::serialization::base_object< tools::Role_ABC >( *this )
                << entity_
                << id_;
    }
private:
    MyEntity* entity_;
    unsigned int id_;
};

class MyRole2 : public tools::Role_ABC
{
public:
             MyRole2() : entity_( 0 ), value_( 0.f ), name_( "bad food" ) {}
    explicit MyRole2( MyEntity& entity ) : entity_( &entity ), value_( 4.2f ), name_( "my role 2" ) {}
    virtual ~MyRole2() {}

    float GetValue() const { return value_; }
    std::string GetName() const { return name_; }

private:
    friend class boost::serialization::access;
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( InputArchive& archive, const unsigned int )
    {
        archive >> boost::serialization::base_object< tools::Role_ABC >( *this )
                >> entity_
                >> value_
                >> name_;
        entity_->Attach( *this );
    }
    void save( OutputArchive& archive, const unsigned int ) const
    {
        archive << boost::serialization::base_object< tools::Role_ABC >( *this )
                << entity_
                << value_
                << name_;
    }
private:
    MyEntity* entity_;
    float value_;
    std::string name_;
};

BOOST_CLASS_EXPORT( MyRole )
BOOST_CLASS_EXPORT( MyRole2 )

// -----------------------------------------------------------------------------
// Name: SerializationTest_SimpleDerivedRole
// Created: SBO 2009-08-14
// -----------------------------------------------------------------------------
//BOOST_AUTO_TEST_CASE( SerializationTest_SimpleDerivedRole )
//{
//    const std::string filename = BOOST_RESOLVE( "output-simple.bin" );
//    {
//        std::ofstream os( filename.c_str(), std::ios::out );
//        BOOST_CHECK( os && os.is_open() );
//        OutputArchive out( os );
//        MyEntity* entity( new MyEntity() );
//        entity->Attach( *new MyRole( *entity ) );
//        BOOST_CHECK_NO_THROW( out << entity );
//        delete entity;
//    }
//    {
//        std::ifstream is( filename.c_str(), std::ios::in );
//        BOOST_CHECK( is && is.is_open() );
//        InputArchive in( is );
//        MyEntity* entity;
//        BOOST_CHECK_NO_THROW( in >> entity );
//        BOOST_REQUIRE( entity );
//        BOOST_CHECK_NO_THROW( entity->Get< MyRole >() );
//        BOOST_CHECK_EQUAL( entity->Get< MyRole >().GetId(), 4242u );
//        delete entity;
//    }
//}

// -----------------------------------------------------------------------------
// Name: SerializationTest_MultipleDerivedRoles
// Created: SBO 2009-08-14
// -----------------------------------------------------------------------------
//BOOST_AUTO_TEST_CASE( SerializationTest_MultipleDerivedRoles )
//{
//    const std::string filename = BOOST_RESOLVE( "output-multiple.bin" );
//    {
//        std::ofstream os( filename.c_str(), std::ios::out );
//        BOOST_CHECK( os && os.is_open() );
//        OutputArchive out( os );
//        MyEntity* entity = new MyEntity();
//        entity->Attach( *new MyRole ( *entity ) );
//        entity->Attach( *new MyRole2( *entity ) );
//        BOOST_CHECK_NO_THROW( out << entity );
//        delete entity;
//    }
//    {
//        std::ifstream is( filename.c_str(), std::ios::in );
//        BOOST_CHECK( is && is.is_open() );
//        InputArchive in( is );
//        MyEntity* entity;
//        BOOST_CHECK_NO_THROW( in >> entity );
//        BOOST_REQUIRE( entity );
//        BOOST_CHECK_NO_THROW( entity->Get< MyRole >() );
//        BOOST_CHECK_NO_THROW( entity->Get< MyRole2 >() );
//        BOOST_CHECK_EQUAL( entity->Get< MyRole >().GetId(), 4242u );
//        BOOST_CHECK_EQUAL( entity->Get< MyRole2 >().GetValue(), 4.2f );
//        BOOST_CHECK_EQUAL( entity->Get< MyRole2 >().GetName(), "my role 2" );
//        delete entity;
//    }
//}
//
//class MyRole3 : public tools::Role_ABC
//{
//public:
//             MyRole3( MyEntity& entity, unsigned int id ) : entity_( entity ), id_( id ), value_( 4.2f ), name_( "My role 3" ) {}
//    virtual ~MyRole3() {}
//
//    float GetValue() const { return value_; }
//    std::string GetName() const { return name_; }
//
//private:
//    friend class boost::serialization::access;
//    BOOST_SERIALIZATION_SPLIT_MEMBER()
//    void load( InputArchive& archive, const unsigned int )
//    {
//        archive >> boost::serialization::base_object< tools::Role_ABC >( *this )
//                >> value_
//                >> name_;
//        entity_.Attach( *this );
//    }
//    void save( OutputArchive& archive, const unsigned int ) const
//    {
//        archive << boost::serialization::base_object< tools::Role_ABC >( *this )
//                << value_
//                << name_;
//    }
//
//private:
//    template< typename Archive > friend void save_construct_data( Archive& archive, const MyRole3* element, const unsigned int version );
//    template< typename Archive > friend void load_construct_data( Archive& archive, MyRole3* element, const unsigned int version );
//
//private:
//    MyEntity& entity_;
//    const unsigned int id_;
//    float value_;
//    std::string name_;
//};
//
//template< typename Archive >
//void save_construct_data( Archive& archive, const MyRole3* element, const unsigned int /*version*/ )
//{
//    MyEntity* const ref = &element->entity_;
//    archive << ref << element->id_;
//}
//
//template< typename Archive >
//void load_construct_data( Archive& archive, MyRole3* element, const unsigned int /*version*/ )
//{
//    MyEntity* entity;
//    unsigned int id;
//    archive >> entity >> id;
//    BOOST_CHECK( entity );
//    ::new( element )MyRole3( *entity, id );
//}

//BOOST_CLASS_EXPORT( MyRole3 )

// -----------------------------------------------------------------------------
// Name: SerializationTest_NoDefaultConstructor
// Created: SBO 2009-08-14
// -----------------------------------------------------------------------------
//BOOST_AUTO_TEST_CASE( SerializationTest_NoDefaultConstructor )
//{
//    const std::string filename = BOOST_RESOLVE( "output-no-default-constructor.bin" );
//    {
//        std::ofstream os( filename.c_str(), std::ios::out );
//        BOOST_CHECK( os && os.is_open() );
//        OutputArchive out( os );
//        MyEntity* entity = new MyEntity();
//        entity->Attach( *new MyRole3( *entity, 4242 ) );
//        BOOST_CHECK_NO_THROW( out << entity );
//        delete entity;
//    }
//    {
//        std::ifstream is( filename.c_str(), std::ios::in );
//        BOOST_CHECK( is && is.is_open() );
//        InputArchive in( is );
//        MyEntity* entity;
//        BOOST_CHECK_NO_THROW( in >> entity );
//        BOOST_CHECK( entity );
//        BOOST_CHECK_NO_THROW( entity->Get< MyRole3 >() );
//        BOOST_CHECK_EQUAL( entity->Get< MyRole3 >().GetValue(), 4.2f );
//        BOOST_CHECK_EQUAL( entity->Get< MyRole3 >().GetName(), "My role 3" );
//        delete entity;
//    }
//}
