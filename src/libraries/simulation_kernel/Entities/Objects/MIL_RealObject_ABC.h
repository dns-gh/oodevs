//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_RealObject_ABC.h $
// $Author: Nld $
// $Modtime: 11/05/05 18:15 $
// $Revision: 17 $
// $Workfile: MIL_RealObject_ABC.h $
//
//*****************************************************************************

#ifndef __MIL_RealObject_ABC_h_
#define __MIL_RealObject_ABC_h_

#include "MIL.h"

#include "MIL_Object_ABC.h"
#include "game_asn/Asn.h"
#include "MT_Tools/MT_Random.h"
#include "simulation_terrain/TER_Localisation.h"

class MIL_PopulationElement_ABC;
class MIL_RealObjectType;
class MIL_Army;
class MIL_Agent_ABC;
class MIL_AgentPion;
class DIA_Parameters;
class PHY_DotationCategory;
class DEC_Knowledge_Object;
class NET_ASN_MsgObjectCreation;
class NET_ASN_MsgObjectUpdate;
class HLA_Object_ABC;
class HLA_UpdateFunctor;
class Deserializer;
class AttributeIdentifier;
class TER_DynamicData;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-15
//=============================================================================
class MIL_RealObject_ABC : public MIL_Object_ABC
{
    MT_COPYNOTALLOWED( MIL_RealObject_ABC );

public:
    //! @name Types
    //@{
    typedef std::set< const MIL_AgentPion* > T_AgentSet;
    typedef T_AgentSet::const_iterator       CIT_AgentSet;
    //@}

public:
    explicit MIL_RealObject_ABC( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_RealObject_ABC();
    virtual ~MIL_RealObject_ABC();
    
    //! @name Init
    //@{
    virtual void                      Initialize( MIL_InputArchive& archive );
    virtual ASN1T_EnumObjectErrorCode Initialize( const ASN1T_MagicActionCreateObject& asn );
    virtual bool                      Initialize( DIA_Parameters& diaParameters, uint& nCurrentParamIdx ); 
    virtual bool                      Initialize( const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass ); // HLA
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;

    void WriteODB( MT_XXmlOutputArchive& archive ) const;
    //@}

    //! @name Operations
    //@{
    virtual void UpdateState();
    //@}

    //! @name Actions
    //@{
    void Construct ( MT_Float rDeltaPercentage );
    void Prepare   ( MT_Float rDeltaPercentage );
    void Destroy   ( MT_Float rDeltaPercentage );
    void Mine      ( MT_Float rDeltaPercentage );
    void Demine    ( MT_Float rDeltaPercentage );
    void Bypass    ( MT_Float rDeltaPercentage );
    void Activate  ();

    void Construct();
    void Destroy  ();
    void Mine     ();
    
    virtual bool CanInteractWith( const MIL_Agent_ABC&  agent ) const;
    virtual bool CanInteractWith( const MIL_Population& population ) const;

    bool CanBePerceived                    () const;
    bool CanBePrepared                     () const;
    bool CanBeConstructed                  () const;
    bool CanBeMined                        () const;
    bool CanBeDemined                      () const;
    bool CanBeDestroyed                    () const;
    bool CanBeBypassed                     () const;
    bool CanBeActivated                    () const;
    uint GetDotationNeededForConstruction  ( MT_Float rDeltaPercentage ) const;
    uint GetDotationRecoveredWhenDestroying( MT_Float rDeltaPercentage ) const;
    uint GetNbrDotationForConstruction     () const;
    uint GetNbrDotationForMining           () const;
    //@}

    //! @name Occupation
    //@{
    void AddOccupier    ( const MIL_AgentPion& agent );
    void RemoveOccupier ( const MIL_AgentPion& agent );
    bool CanBeOccupiedBy( const MIL_AgentPion& agent ) const;
    //@}

    //! @name Animation
    //@{
          bool        AddAnimator    ( const MIL_AgentPion& agent );
          void        RemoveAnimator ( const MIL_AgentPion& agent );
          bool        CanBeAnimatedBy( const MIL_AgentPion& agent ) const;
    const T_AgentSet& GetAnimators   () const;
    //@}

    //! @name Population
    //@{
    void     ApplyAttrition               ( MIL_PopulationElement_ABC& target );
    MT_Float GetExitingPopulationDensity  () const;
    void     SetExitingPopulationDensity  ( MT_Float rDensity );
    void     ResetExitingPopulationDensity();
    //@}

    //! @name Speed 
    //@{
    MT_Float ApplySpeedPolicy( MT_Float rAgentSpeedWithinObject, MT_Float rAgentSpeedWithinEnvironment, MT_Float rAgentMaxSpeed ) const;
    //@}

    //! @name Network
    //@{
    ASN1T_EnumObjectErrorCode OnReceiveMagicActionUpdate( const ASN1T_MagicActionUpdateObject& asnMsg );
    void                      SendCreation              () /*$$$const*/; //$$$ BULLSHIT
    void                      SendDestruction           () /*$$$const*/; //$$$ BULLSHIT
    void                      SendFullState             () /*$$$const*/;
    //@}

    //! @name HLA
    //@{
    HLA_Object_ABC* GetHLAView() const;
    void            SetHLAView( HLA_Object_ABC& view );
    virtual void Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer );
    virtual void Serialize  ( HLA_UpdateFunctor& functor ) const;
    //@}

    //! @name Accessors
    //@{
    const MIL_RealObjectType& GetType                    () const;
    uint                      GetID                      () const;
    MT_Float                  GetSizeCoef                () const;
    MT_Float                  GetDefaultMaxSpeed         () const;
    bool                      IsBypassed                 () const;
    bool                      IsMined                    () const;
    bool                      IsPrepared                 () const;
    MT_Float                  GetConstructionPercentage  () const;
    MT_Float                  GetMiningPercentage        () const;
    MT_Float                  GetBypassPercentage        () const;
    const TER_Localisation&   GetAvoidanceLocalisation   () const;
    const MIL_AgentPion*      GetOccupier                () const;
    
    virtual bool              IsReal() const;
    //@}

    //! @name Tools
    //@{
    virtual DEC_Knowledge_Object& CreateKnowledge( const MIL_Army& teamKnowing );
    //@}

protected:
    //! @name Types
    //@{
    enum E_AttributeUpdate
    {
        eAttrUpdate_ConstructionPercentage  = 0x01,
        eAttrUpdate_MiningPercentage        = 0x02,
        eAttrUpdate_BypassPercentage        = 0x04,
        eAttrUpdate_Prepared                = 0x08,
        eAttrUpdate_Localisation            = 0x10,
        eAttrUpdate_SpecificAttributes      = 0x20,
        eAttrUpdate_All                     = 0xFF
    };
    //@}
  
    //! @name Tools
    //@{
            void ApplyAttrition    ( MIL_Agent_ABC&             target );    
            void InitializeCommon  ( const TER_Localisation& localisation );
    virtual void UpdateLocalisation( const TER_Localisation& newLocalisation );

    //@}

    //! @name Network
    //@{
    virtual void WriteSpecificAttributes( MT_XXmlOutputArchive& archive ) const;
    virtual void WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg );
    virtual void WriteSpecificAttributes( NET_ASN_MsgObjectUpdate&   asnMsg );
            void NotifyAttributeUpdated ( E_AttributeUpdate nAttrToUpdate );
    //@}

    //! @name Events
    //@{
    virtual void ProcessAgentInside      ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentMovingInside( MIL_Agent_ABC& agent );
    //@}

private:
    //! @name Network
    //@{
    void SendMsgUpdate();
    //@}

    //! @name Tools
    //@{
    void InitializeAvoidanceLocalisation();

    void ChangeConstructionPercentage   ( MT_Float rNewConstructionPercentage );
    void ChangeMiningPercentage         ( MT_Float rNewMiningPercentage       );

    bool IsAttributeUpdated    ( E_AttributeUpdate nAttrToUpdate, MT_Float rValue, uint nLastValue );
    //@}

protected:
    MT_Float rSizeCoef_; // e.g. nb mines / longueur ...

    // Dotations
    uint nFullNbrDotationForConstruction_;
    uint nFullNbrDotationForMining_;

    // Network
            uint8 xAttrToUpdate_;
    mutable uint8 xAttrToUpdateForHLA_;

private:
    const MIL_RealObjectType* pType_;
    uint                      nID_;    
    std::string               strName_;

    // State
    MT_Float rConstructionPercentage_;
    MT_Float rMiningPercentage_;
    MT_Float rBypassPercentage_;

    uint  nLastValueConstructionPercentage_;
    uint  nLastValueMiningPercentage_;
    uint  nLastValueBypassPercentage_;

    bool bPrepared_;

    uint nCurrentNbrDotationForConstruction_;
    uint nCurrentNbrDotationForMining_;            

    // Geometry
    TER_Localisation avoidanceLocalisation_;
    
    // Path finder enhancement
    TER_DynamicData* pPathfindData_;

    // Misc
    const MIL_AgentPion* pOccupier_;
          T_AgentSet     animators_;
          MT_Float       rExitingPopulationDensity_;

    // View
    HLA_Object_ABC* pView_;

    
private:
    static MT_Random random_;
};

#include "MIL_RealObject_ABC.inl"

#endif // __MIL_RealObject_ABC_h_

