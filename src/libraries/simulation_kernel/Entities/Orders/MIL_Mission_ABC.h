// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Mission_ABC_h_
#define __MIL_Mission_ABC_h_

#include "MIL_OrderContext.h"
#include "MIL.h"
#include <boost/shared_ptr.hpp>

class DEC_Gen_Object;
class DEC_KnowledgeResolver_ABC;
class DEC_Knowledge_Object;
class MIL_AgentPion;
class MIL_Automate;
class MIL_FragOrderType;
class MIL_KnowledgeGroup;
class MIL_MissionParameter_ABC;
class MIL_MissionParameterVisitor_ABC;
class MIL_MissionType_ABC;
class MIL_ParameterType_ABC;

// =============================================================================
/** @class  MIL_Mission_ABC
    @brief  MIL_Mission_ABC
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_Mission_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    virtual ~MIL_Mission_ABC();
    //@}

     //! @name Operations
    //@{
    virtual void Start               ( boost::shared_ptr< MIL_Mission_ABC > self ) = 0;
    virtual void Stop                ( boost::shared_ptr< MIL_Mission_ABC > self ) = 0;
    virtual bool IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const = 0;

    void UsedByDIA    ();
    void ReleasedByDIA();
    //@}

    //! @name Accessors
    //@{
    virtual const MIL_MissionType_ABC& GetType() const;
    const std::string& GetDIAType() const;

    virtual MIL_AgentPion& GetPion() const;
    virtual MIL_Automate&  GetAutomate() const;

    virtual void Send() const = 0;

    const   std::string&   GetName              () const;
    const   MT_Vector2D&   GetDirDanger         () const;
    const   MIL_Fuseau&    GetFuseau            () const;
    const   T_LimaVector&  GetLimas             () const;
            MIL_LimaOrder* FindLima             ( unsigned int nID );
            MIL_LimaOrder* FindLima             ( const MIL_LimaFunction& function );
            MIL_LimaOrder* FindNextScheduledLima();
            void           AffectFuseau         ( const MIL_Fuseau& fuseau );
    virtual void           AffectDirection      ( const MT_Vector2D& direction );
            void           SetParameter         ( const std::string& name, boost::shared_ptr< MIL_MissionParameter_ABC > param );
            void           AppendToParameter    ( const std::string& name, boost::shared_ptr< TER_Localisation > pLocation );
            void           AppendToParameter    ( const std::string& name, boost::shared_ptr< DEC_Knowledge_Object > pObjectKnowledge );
            void           AppendToParameter    ( const std::string& name, boost::shared_ptr< DEC_Gen_Object > pGenObject );
    //@}

    //! @name Parameters Management
    //@{
    virtual void Visit( MIL_MissionParameterVisitor_ABC& parameterVisitor ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@}

protected:
    //! @name Constructors/Destructor
    //@{
    MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver );
    MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const sword::MissionParameters& parameters );
    MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const sword::MissionParameters& parameters, const MT_Vector2D& refPosition );
    MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const boost::shared_ptr< MIL_Mission_ABC > parent );
    MIL_Mission_ABC( const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_Mission_ABC& rhs );
    //@}

    //! @name Helpers
    //@{
    void FillParameters( int firstIndex, const sword::MissionParameters& parameters );
    void Serialize( sword::MissionParameters& asn ) const;
    //@}

    //! @name Member Data
    //@{
    const MIL_MissionType_ABC& type_;
    //@}

private:
    //! @name Serialization
    //@{
    template< typename Archive > friend void save_construct_data( Archive& archive, const MIL_Mission_ABC* mission, const unsigned int version );
    template< typename Archive > friend void load_construct_data( Archive& archive, MIL_Mission_ABC* mission, const unsigned int version );
    //@}

private:
    //! @name Member data
    //@{
    const DEC_KnowledgeResolver_ABC& knowledgeResolver_;
    MIL_OrderContext context_;
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters_;
    //@}
};

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_Mission_ABC* mission, const unsigned int /*version*/ )
{
    const DEC_KnowledgeResolver_ABC* const resolver = &mission->knowledgeResolver_;
    archive << resolver
        << mission->type_.GetId();
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_Mission_ABC* mission, const unsigned int /*version*/ )
{
    DEC_KnowledgeResolver_ABC* resolver = 0;
    int id = -1;
    archive >> resolver
        >> id;
    MIL_MissionType_ABC* type = MIL_PionMissionType::Find( id );
    assert( type );
    ::new( mission )MIL_Mission_ABC( *type, *resolver );
}

BOOST_CLASS_EXPORT_KEY( MIL_Mission_ABC )

#endif // __MIL_Mission_ABC_h_
