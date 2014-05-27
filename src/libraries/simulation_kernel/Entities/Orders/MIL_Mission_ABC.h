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
#include <boost/optional/optional_fwd.hpp>

class ActionManager;
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
    virtual void Start               ( boost::shared_ptr< MIL_Mission_ABC > self, ActionManager& actions ) = 0;
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
    virtual unsigned int GetOwnerId() const;

    virtual void Send( ActionManager& actions ) const = 0;

    const   std::string&   GetName              () const;
    boost::shared_ptr< MT_Vector2D > GetDirDanger() const;
    const   MIL_Fuseau&    GetFuseau            () const;
    const   T_LimaVector&  GetLimas             () const;
            MIL_LimaOrder* FindLima             ( unsigned int nID );
            MIL_LimaOrder* FindLima             ( const MIL_LimaFunction& function );
            std::vector< MIL_LimaOrder* > FindAllLimas ( const MIL_LimaFunction& function );
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
    MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, uint32_t id, const boost::shared_ptr< MIL_Mission_ABC >& parent );
    MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, uint32_t id, const sword::MissionParameters& parameters,
                     const boost::optional< MT_Vector2D >& orientation );
    MIL_Mission_ABC( const MIL_Mission_ABC& rhs, const DEC_KnowledgeResolver_ABC& knowledgeResolver, uint32_t id );
    //@}

    //! @name Helpers
    //@{
    void FillParameters( int firstIndex, const sword::MissionParameters& parameters );
    void Serialize( sword::MissionParameters& asn ) const;
    uint32_t GetId() const;
    uint32_t GetParentId() const;
    //@}

    //! @name Member Data
    //@{
    const MIL_MissionType_ABC& type_;
    //@}

private:
    //! @name Helpers
    //@{
    void MakeParameters( unsigned int index );
    //@}

private:
    //! @name Member data
    //@{
    const DEC_KnowledgeResolver_ABC& knowledgeResolver_;
    const uint32_t id_;
    const uint32_t parentId_;
    MIL_OrderContext context_;
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters_;
    //@}
};

#endif // __MIL_Mission_ABC_h_
