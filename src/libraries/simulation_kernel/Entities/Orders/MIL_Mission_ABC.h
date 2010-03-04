// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Mission_ABC_h_
#define __MIL_Mission_ABC_h_

#include "MIL_OrderContext.h"
#include <boost/shared_ptr.hpp>

class DEC_KnowledgeResolver_ABC;
class DEC_Knowledge_Object;
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
    virtual void Start               () = 0;
    virtual bool IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const = 0;

    void UsedByDIA    ();
    void ReleasedByDIA();
    //@}

    //! @name Accessors
    //@{
    virtual const MIL_MissionType_ABC& GetType() const;
    const std::string& GetDIAType() const;

    const   std::string&   GetName              () const;
    const   MT_Vector2D&   GetDirDanger         () const;
    const   MIL_Fuseau&    GetFuseau            () const;
    const   T_LimaVector&  GetLimas             () const;
            MIL_LimaOrder* FindLima             ( unsigned int nID );
            MIL_LimaOrder* FindLima             ( const MIL_LimaFunction& function );
            MIL_LimaOrder* FindNextScheduledLima();
            void           AffectFuseau         ( const MIL_Fuseau& fuseau );
    virtual void           AffectDirection      ( const MT_Vector2D& direction );
            void           Accept               ( MIL_IntelligenceOrdersVisitor_ABC& visitor ) const;
            void           SetParameter         ( const std::string& name, boost::shared_ptr< MIL_MissionParameter_ABC > param );
            void           AppendToParameter    ( const std::string& name, boost::shared_ptr< TER_Localisation > pLocation );
            void           AppendToParameter    ( const std::string& name, boost::shared_ptr< DEC_Knowledge_Object > pObjectKnowledge );
    //@}

    //! @name Parameters Management
    virtual void Visit( MIL_MissionParameterVisitor_ABC& parameterVisitor ) const;
    //@}

protected:
    //! @name Constructors/Destructor
    //@{
    MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver );
    MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const Common::MsgMissionParameters& parameters );
    MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const Common::MsgMissionParameters& parameters, const MT_Vector2D& refPosition );
    MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_Mission_ABC& parent );
    MIL_Mission_ABC( const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_Mission_ABC& rhs );
    //@}

    //! @name Network
    //@{
    void Serialize( Common::MsgMissionParameters& asn ) const;
    //@}

private:
    const DEC_KnowledgeResolver_ABC& knowledgeResolver_;
    const MIL_MissionType_ABC&       type_;
          MIL_OrderContext           context_;

          std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters_;
};

#endif // __MIL_Mission_ABC_h_
