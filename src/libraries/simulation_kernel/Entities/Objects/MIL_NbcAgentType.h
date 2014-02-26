//*****************************************************************************
//
// $Created: JVT 03-01-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_NbcAgentType.h $
// $Author: Jvt $
// $Modtime: 14/04/05 17:29 $
// $Revision: 4 $
// $Workfile: MIL_NbcAgentType.h $
//
//*****************************************************************************

#ifndef __MIL_NbcAgentType_h_
#define __MIL_NbcAgentType_h_

namespace xml
{
    class xistream;
}

class PHY_HumanWound;
class MIL_IntoxicationEffect;

//*****************************************************************************
// Created: JVT 03-01-06
// Modified: JVT 2004-10-28
//*****************************************************************************
class MIL_NbcAgentType : private boost::noncopyable
{
public:
    //! @name Factory
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate();

    static const MIL_NbcAgentType* Find( const std::string& strName );
    static const MIL_NbcAgentType* Find( unsigned int nID );
    //@}

    //! @name Accessors
    //@{
    unsigned int          GetID                 () const;
    const std::string&    GetName               () const;

    bool                  IsLiquidContaminating () const;
    bool                  IsLiquidPoisonous     () const;
    const PHY_HumanWound& GetLiquidRandomWound  () const;

    bool                  IsGasContaminating    () const;
    bool                  IsGasPoisonous        () const;
    const PHY_HumanWound& GetGasRandomWound     () const;
    double                GetGasLifeTime        () const;
    double                GetGasPropagationAngle() const;
    bool                  CanBeVaporized        () const;
    void                  InitializePopulationEffect( MIL_IntoxicationEffect& effect ) const;
    //@}

    //! @name Accessors
    //@{
    static double GetCoefMaxSpeedModificator     ();
    static double GetCoefReloadingTimeModificator();
    static double GetContaminationDistance       ();
    static double GetMinContaminationQuantity    ();
    static double GetMinPropagationSpeed         ();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_NbcAgentType* > T_NbcAgentTypeMap;
    typedef T_NbcAgentTypeMap::const_iterator              CIT_NbcAgentTypeMap;

    typedef std::vector< double >                    T_HumanPoisonousVector;
    typedef T_HumanPoisonousVector::const_iterator CIT_HumanPoisonousVector;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             MIL_NbcAgentType( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_NbcAgentType();
    //@}

    //! @name Tools
    //@{
    bool                  ReadPoisonousData( xml::xistream& xis, T_HumanPoisonousVector& data );
    const PHY_HumanWound& GetRandomWound   ( const T_HumanPoisonousVector& data ) const;
    void                  ApplyIntoxication( MIL_IntoxicationEffect& effect, const T_HumanPoisonousVector& data ) const;
    //@}

    //! @name Helpers
    //@{
    static void ReadAgent( xml::xistream& xis );
    void SortEffectType  ( xml::xistream& xis );
    void ReadLiquid      ( xml::xistream& xis );
    void ReadGaz         ( xml::xistream& xis );
    void ReadEffect      ( xml::xistream& xis, T_HumanPoisonousVector& data );
    //@}

private:
    //! @name Member data
    //@{
    const std::string strName_;
    unsigned int nID_;
    T_HumanPoisonousVector liquidPoisonous_;
    bool bLiquidPoisonous_;
    bool bLiquidContaminating_;
    bool bCanBeVaporized_;
    T_HumanPoisonousVector gasPoisonous_;
    bool bGasPoisonous_;
    bool bGasContaminating_;
    double nGasLifeTime_;
    double rGasPropagationAngle_;
    static T_NbcAgentTypeMap nbcAgentTypes_;
    static double rCoefMaxSpeedModificator_;
    static double rCoefReloadingTimeModificator_;
    static double rContaminationDistance_;
    static double rContaminationQuantityGiven_;
    static double rMinPropagationSpeed_;
    //@}
};

namespace boost
{
namespace archive
{
    template< class Archive >
    void save( Archive& ar, const MIL_NbcAgentType* t )
    {
        unsigned int id = t ? t->GetID() : std::numeric_limits< unsigned int >::max();
        ar << id;
    }
    template< class Archive >
    void load( Archive& ar, const MIL_NbcAgentType*& t )
    {
        unsigned int id;
        ar >> id;
        t = MIL_NbcAgentType::Find( id );
    }
}
}

#endif // __MIL_NbcAgentType_h_
