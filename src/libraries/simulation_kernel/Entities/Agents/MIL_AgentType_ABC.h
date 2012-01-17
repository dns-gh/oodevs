// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_AgentType_ABC_h_
#define __MIL_AgentType_ABC_h_

namespace xml
{
    class xistream;
}

class DEC_Model_ABC;
class MIL_Agent_ABC;
class PHY_NatureAtlas;
class PHY_NatureLevel;
class PHY_UnitType;
class TerrainData;

namespace directia
{
namespace brain
{
    class Brain;
}
}

// =============================================================================
// @class  MIL_AgentType_ABC
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentType_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Desctructor
    //@{
             MIL_AgentType_ABC( const std::string& strName, const std::string& strType, xml::xistream& xis );
    virtual ~MIL_AgentType_ABC();
    //@}

    //! @name Getters/Setters
    //@{
    unsigned int GetID() const;
    const std::string& GetName() const;
    const std::string& GetMilPionType() const;
    const PHY_NatureLevel& GetNatureLevel() const;
    const PHY_NatureAtlas& GetNatureAtlas() const;
    virtual bool IsMilitia() const;
    virtual bool IsRefugee() const;
    virtual bool IsTerrorist() const;
    virtual const PHY_UnitType& GetUnitType() const = 0;
    virtual double GetRapForIncreasePerTimeStepValue() const = 0;
    virtual void RegisterFunctions( directia::brain::Brain& brain, MIL_Agent_ABC& agent ) const = 0;
    virtual const DEC_Model_ABC& GetModel() const = 0;
    virtual double GetDistanceAvantPoint ( const TerrainData& nType ) const = 0;
    virtual double GetDistanceAvantLima () const = 0;
    //@}

protected:
    MIL_AgentType_ABC();

private:
    //! @name Init
    //@{
    void InitializeNature( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int nID_;
    const std::string strName_;
    const std::string strType_;
    const PHY_NatureLevel* pNatureLevel_;
    const PHY_NatureAtlas* pNatureAtlas_;
    //@}
};

#endif // __MIL_AgentType_ABC_h_
