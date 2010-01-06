// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/entities/agents/MIL_AgentType_ABC.h $
// $Author: Jvt $
// $Modtime: 14/04/05 17:47 $
// $Revision: 7 $
// $Workfile: MIL_AgentType_ABC.h $
//
// *****************************************************************************

#ifndef __MIL_AgentType_ABC_h_
#define __MIL_AgentType_ABC_h_

namespace xml
{
    class xistream;
}

class PHY_NatureAtlas;
class PHY_NatureLevel;
class PHY_UnitType;

// =============================================================================
// @class  MIL_AgentType_ABC
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentType_ABC : private boost::noncopyable
{

public:
             MIL_AgentType_ABC( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AgentType_ABC();

    //! @name Types
    //@{
          uint             GetID         () const;
    const std::string&     GetName       () const;
    const PHY_NatureLevel& GetNatureLevel() const;
    const PHY_NatureAtlas& GetNatureAtlas() const;

    virtual bool           IsMilitia     () const;
    virtual bool           IsRefugee     () const;
    virtual bool           IsTerrorist   () const;

    virtual const PHY_UnitType& GetUnitType() const = 0;
    //@}

protected:
    MIL_AgentType_ABC();

private:
    //! @name Init
    //@{
    void InitializeNature( xml::xistream& xis );
    //@}

private:
          uint             nID_;
    const std::string      strName_;
    const PHY_NatureLevel* pNatureLevel_;
    const PHY_NatureAtlas* pNatureAtlas_;
};

#endif // __MIL_AgentType_ABC_h_
