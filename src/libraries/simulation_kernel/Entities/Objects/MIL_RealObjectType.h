// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_RealObjectType.h $
// $Author: Nld $
// $Modtime: 5/04/05 12:05 $
// $Revision: 20 $
// $Workfile: MIL_RealObjectType.h $
//
// *****************************************************************************

#ifndef __MIL_RealObjectType_h_
#define __MIL_RealObjectType_h_

#include "MIL.h"

#include "Entities/Agents/Units/Weapons/PHY_AttritionData.h"
#include "game_asn/Asn.h"

class PHY_DotationCategory;
class MIL_RealObject_ABC;
class MIL_Army;
class MIL_RealObjectTypeFilter;
class MIL_MOSIDManager;
class PHY_ConsumptionType;
class PHY_Protection;

// =============================================================================
// @class  MIL_RealObjectType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_RealObjectType
{
    MT_COPYNOTALLOWED( MIL_RealObjectType )

public:
    //! @name Types
    //@{
    typedef std::map< uint, const MIL_RealObjectType* > T_ObjectTypeMap;
    typedef T_ObjectTypeMap::const_iterator             CIT_ObjectTypeMap;

    enum E_Behavior
    {
        eHate,
        eAvoid,
        eIgnore,
        eEnjoy,
        ePrefer
    };
    //@}

    //! @name Statics
    //@{
    static const MIL_RealObjectType& fosseAntiChar_;          
    static const MIL_RealObjectType& abattis_;                 
    static const MIL_RealObjectType& barricade_;              
    static const MIL_RealObjectType& bouchonMines_;           
    static const MIL_RealObjectType& zoneMineeLineaire_;      
    static const MIL_RealObjectType& zoneMineeParDispersion_; 
    static const MIL_RealObjectType& eboulement_;             
    static const MIL_RealObjectType& destructionRoute_;       
    static const MIL_RealObjectType& destructionPont_;        
    static const MIL_RealObjectType& pontFlottantContinu_;
    static const MIL_RealObjectType& pontFlottantDiscontinu_;
    static const MIL_RealObjectType& posteTir_;               
    static const MIL_RealObjectType& zoneProtegee_;           
    static const MIL_RealObjectType& zoneImplantationCanon_;  
    static const MIL_RealObjectType& zoneImplantationCOBRA_;  
    static const MIL_RealObjectType& zoneImplantationLRM_;    
    static const MIL_RealObjectType& siteFranchissement_;     
    static const MIL_RealObjectType& nuageNBC_;               
    static const MIL_RealObjectType& siteDecontamination_;    
    static const MIL_RealObjectType& plotRavitaillement_;  
    static const MIL_RealObjectType& zoneBrouillageBrod_;
    static const MIL_RealObjectType& zoneBrouillageBromure_;
    static const MIL_RealObjectType& rota_;    
    static const MIL_RealObjectType& zoneNBC_;
	static const MIL_RealObjectType& airePoser_;
	static const MIL_RealObjectType& piste_;
	static const MIL_RealObjectType& plateForme_;
	static const MIL_RealObjectType& zoneMobiliteAmelioree_;
	static const MIL_RealObjectType& zonePoserHelicoptere_;
    static const MIL_RealObjectType& aireLogistique_;
    static const MIL_RealObjectType& campPrisonniers_;
    static const MIL_RealObjectType& campRefugies_;
    static const MIL_RealObjectType& itineraireLogistique_;
    static const MIL_RealObjectType& posteControle_;
    static const MIL_RealObjectType& terrainLargage_;
    static const MIL_RealObjectType& zoneForbiddenFire_;
    static const MIL_RealObjectType& zoneForbiddenMove_;
    static const MIL_RealObjectType& zoneImplantationMortier_;
    static const MIL_RealObjectType& installation_;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize       ( MIL_InputArchive& archive );
    static void Terminate        ();

    static const T_ObjectTypeMap&    GetObjectTypes();
    static const MIL_RealObjectType* Find( const std::string& strName );
    static const MIL_RealObjectType* Find( uint nID );
    static const MIL_RealObjectType* Find( ASN1T_EnumObjectType nAsnID );

    static const MIL_RealObjectTypeFilter& GetHatedObjectTypes();
    //@}

    //! @name Accessors
    //@{
          uint                  GetID                             () const;
          ASN1T_EnumObjectType  GetAsnID                          () const;
    const std::string&          GetName                           () const;
          MIL_MOSIDManager&     GetIDManager                      () const;
          MT_Float              GetAvoidanceDistance              () const;
          bool                  CanBeMined                        () const;
          bool                  CanBePrepared                     () const;
          bool                  CanBeBypassed                     () const;
          MT_Float              GetMaxInteractionHeight           () const;
          MT_Float              GetDefaultSpeedWhenBypassed       () const;
          MT_Float              GetDefaultSpeedWhenNotBypassed    () const;
    const PHY_ConsumptionType&  GetDefaultConsumptionMode         () const;
          uint                  GetNbrMaxAnimators                () const;
          E_Behavior            GetBehavior                       () const;
          MT_Float              GetExitingPopulationDensity       () const;

    // Dotations
    const PHY_DotationCategory* GetDotationCategoryForConstruction() const;
    const PHY_DotationCategory* GetDotationCategoryForMining      () const;
          uint                  GetNbrDotationForConstruction     () const;
          uint                  GetNbrDotationForMining           () const;

    // Attrition
    const PHY_AttritionData&    GetPionAttritionData              ( const PHY_Protection& protection ) const;
          MT_Float              GetPopulationAttritionPH          () const;
          MT_Float              GetPopulationAttritionSurface     () const;
    //@}

    //! @name Operations
    //@{
    MT_Float            ApplySpeedPolicy     ( MT_Float rAgentSpeedWithinObject, MT_Float rAgentSpeedWithinEnvironment, MT_Float rAgentMaxSpeed ) const;
    int                 ComputePlacementScore( const MT_Vector2D& pos, const TerrainData& nPassability ) const;
    MIL_RealObject_ABC& InstanciateObject    ( uint nID, MIL_Army& army ) const;
    bool                operator==           ( const MIL_RealObjectType& rhs ) const;
    bool                operator!=           ( const MIL_RealObjectType& rhs ) const;
    //@}

protected:
    //! @name Types
    //@{
    enum E_ObjectType
    {
        eObjectTypeBouchonMines             = 0,
        eObjectTypeZoneMineeLineaire        = 1,
        eObjectTypeZoneMineeParDispersion   = 2,
        eObjectTypeFosseAntiChar            = 3,
        eObjectTypeAbattis                  = 4,
        eObjectTypeBarricade                = 5,
        eObjectTypeEboulement               = 6,
        eObjectTypeDestructionRoute         = 7,
        eObjectTypeDestructionPont          = 8,
        eObjectTypePontFlottantContinu      = 9,
        eObjectTypePosteTir                 = 10,
        eObjectTypeZoneProtegee             = 11,
        eObjectTypeZoneImplantationCanon    = 12,
        eObjectTypeZoneImplantationCOBRA    = 13,
        eObjectTypeZoneImplantationLRM      = 14,
        eObjectTypeSiteFranchissement       = 15,
        eObjectTypeNuageNBC                 = 16,
        eObjectTypePlotRavitaillement       = 17,
        eObjectTypeSiteDecontamination      = 18,        
        eObjectTypeZoneBrouillageBrod       = 19,
        eObjectTypeRota                     = 20,
        eObjectTypeZoneNBC                  = 21,
        eObjectTypeZoneBrouillageBromure    = 22,
		eObjectTypeAirePoser				= 23,
		eObjectTypePiste					= 24,
		eObjectTypePlateForme				= 25,
		eObjectTypeZoneMobiliteAmelioree	= 26,
		eObjectTypeZonePoserHelicoptere     = 27,
        eObjectTypeAireLogistique           = 28,
        eObjectTypeItineraireLogistique     = 29,
        eObjectTypeCampPrisonniers          = 30,
        eObjectTypeCampRefugies             = 31,
        eObjectTypePosteControle            = 32,
        eObjectTypeTerrainLargage           = 33,
        eObjectTypeZoneForbiddenMove        = 34,
        eObjectTypeZoneForbiddenFire        = 35,
        eObjectTypeZoneImplantationMortier  = 36,
        eObjectTypePontFlottantDiscontinu   = 37,
        eObjectTypeInstallation             = 38
    };

    enum E_SpeedPolicy
    {
        eSpeedPolicy_Slowest,
        eSpeedPolicy_AgentMaxSpeed,
        eSpeedPolicy_ObjectMaxSpeed
    };

    typedef MIL_RealObject_ABC& (*T_ObjectInstanciator)( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
    //@}

protected:
    MIL_RealObjectType( const std::string& strName, E_ObjectType nType, ASN1T_EnumObjectType nAsnID, T_ObjectInstanciator objectInstanciator, E_Behavior nBehavior );
    virtual ~MIL_RealObjectType();

    //! @name Init
    //@{
    virtual void Read( MIL_InputArchive& archive );
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair < TerrainData, int >          T_ScorePair;
    typedef std::vector< T_ScorePair >              T_EnvironmentScoreMap;
    typedef T_EnvironmentScoreMap::const_iterator CIT_EnvironmentScoreMap;

    typedef std::vector< PHY_AttritionData >  T_AttritionVector;
    //@}

private:
    //! @name Init tools
    //@{
    bool IsInitialized                    () const;
    void InitializeSpeedData              ( MIL_InputArchive& archive );
    void InitializePlacementScores        ( MIL_InputArchive& archive );
    void InitializePionAttritionData      ( MIL_InputArchive& archive );
    void InitializePopulationAttritionData( MIL_InputArchive& archive );
    void InitializeDotations              ( MIL_InputArchive& archive );
    void InitializeDotation               ( MIL_InputArchive& archive, const std::string& strSection, const PHY_DotationCategory*& pDotationCategory, uint& rDotationValue ) const;
    //@}

private:
    const E_ObjectType          nType_;
    const ASN1T_EnumObjectType  nAsnID_;
    const std::string           strName_;
    T_ObjectInstanciator        objectInstanciator_;
    bool                        bCanBePrepared_;
    bool                        bCanBeMined_;
    bool                        bCanBeBypassed_;
    MT_Float                    rAvoidanceDistance_;
    MT_Float                    rDefaultSpeedWhenBypassed_;
    MT_Float                    rDefaultSpeedWhenNotBypassed_;
    MT_Float                    rMaxInteractionHeight_;
    T_EnvironmentScoreMap       environmentScores_;
    E_SpeedPolicy               nSpeedPolicy_;
    MT_Float                    rSpeedPolicyMaxSpeedAgentFactor_;
    bool                        bInitialized_;
    const PHY_DotationCategory* pDotationCategoryForConstruction_;
    uint                        nNbrDotationForConstruction_;
    const PHY_DotationCategory* pDotationCategoryForMining_;
    uint                        nNbrDotationForMining_;
    const PHY_ConsumptionType*  pDefaultConsumptionMode_;
    uint                        nNbrMaxAnimators_;
    MIL_MOSIDManager*           pIDManager_;
    E_Behavior                  nBehavior_; //$$$ a renommer 
    MT_Float                    rExitingPopulationDensity_;

    // Attrition
    T_AttritionVector           pionAttritions_;
    MT_Float                    rPopulationAttritionPH_;
    MT_Float                    rPopulationAttritionSurface_;

private:
    static T_ObjectTypeMap           objectTypes_;
    static MIL_RealObjectTypeFilter* pHatedObjectTypes_;
};

#include "MIL_RealObjectType.inl"

#endif // __MIL_RealObjectType_h_
