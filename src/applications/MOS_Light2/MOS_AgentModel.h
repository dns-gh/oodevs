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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentModel.h $
// $Author: Age $
// $Modtime: 13/04/05 11:45 $
// $Revision: 8 $
// $Workfile: MOS_AgentModel.h $
//
// *****************************************************************************

#ifndef __MOS_AgentModel_h_
#define __MOS_AgentModel_h_


// =============================================================================
/** @class  MOS_AgentModel
    @brief  Represents an agent type and the missions that are available to it.
*/
// Created: AGN 2003-12-22
// =============================================================================
class MOS_AgentModel
{
    MT_COPYNOTALLOWED( MOS_AgentModel )

public:
    class Cmp : public std::unary_function< MOS_AgentModel* , bool >
    {
    public:
        Cmp(const std::string& val) : val_(val) {}
        ~Cmp() {}

        bool operator()( MOS_AgentModel* tgtnfos ) const
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
    MOS_AgentModel( bool bAutomataModel, const std::string& strName );
    ~MOS_AgentModel();
    //@}

    //! @name Operations
    //@{
    void Initialize( MT_InputArchive_ABC& input );
    void ReadFragOrders( MT_InputArchive_ABC& input );
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


#ifdef MOS_USE_INLINE
#   include "MOS_AgentModel.inl"
#endif

#endif // __MOS_AgentModel_h_
