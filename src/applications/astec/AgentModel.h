// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-12-22 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentModel.h $
// $Author: Age $
// $Modtime: 13/04/05 11:45 $
// $Revision: 8 $
// $Workfile: AgentModel.h $
//
// *****************************************************************************

#ifndef __AgentModel_h_
#define __AgentModel_h_

#include "Types.h"

// =============================================================================
/** @class  AgentModel
    @brief  Represents an agent type and the missions that are available to it.
*/
// Created: AGN 2003-12-22
// =============================================================================
class AgentModel
{
    MT_COPYNOTALLOWED( AgentModel )

public:
    class Cmp : public std::unary_function< AgentModel* , bool >
    {
    public:
        Cmp(const std::string& val) : val_(val) {}
        ~Cmp() {}

        bool operator()( AgentModel* tgtnfos ) const
        { return tgtnfos->GetName()==val_; }

    private:

        std::string val_;
    };

public:
    typedef std::set< uint >               T_FragOrderSet;
    typedef T_FragOrderSet::iterator       IT_FragOrderSet;
    typedef T_FragOrderSet::const_iterator CIT_FragOrderSet;

    typedef std::vector< uint >             T_MissionVector;
    typedef T_MissionVector::iterator       IT_MissionVector;
    typedef T_MissionVector::const_iterator CIT_MissionVector;

public:
    //! @name Constructors/Destructor
    //@{
    AgentModel( bool bAutomataModel, const std::string& strName );
    ~AgentModel();
    //@}

    //! @name Operations
    //@{
    void Initialize( InputArchive& input,  bool popu = false );
    void ReadFragOrders( InputArchive& input );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    const std::string& GetDIAType() const;
    const T_MissionVector& GetAvailableMissions() const;
    const T_FragOrderSet&  GetAvailableFragOrders() const;
    //@}

private:
    //! @name Helpers
    //@{

    //@}

    //! @name Member data
    //@{
    std::string strName_;
    std::string strDIAType_;
    bool bAutomataModel_;
    T_MissionVector vAvailableMissions_;
    T_FragOrderSet fragmentaryOrders_;
    //@}
};

#   include "AgentModel.inl"

#endif // __AgentModel_h_
