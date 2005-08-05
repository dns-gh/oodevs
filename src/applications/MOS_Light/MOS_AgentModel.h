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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentModel.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_AgentModel.h $
//
// *****************************************************************************

#ifndef __MOS_AgentModel_h_
#define __MOS_AgentModel_h_

#include "MOS_Types.h"
#include "MOS_Order_Def.h"

// =============================================================================
/** @class  MOS_AgentModel
    @brief  MOS_AgentModel
    @par    Using example
    @code
    MOS_AgentModel;
    @endcode
*/
// Created: AGN 2003-12-22
// =============================================================================
class MOS_AgentModel
{

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


    typedef std::vector< uint >              T_Mission_Vector;
    typedef T_Mission_Vector::iterator       IT_Mission_Vector;
    typedef T_Mission_Vector::const_iterator CIT_Mission_Vector;

public:
    //! @name Constructors/Destructor
    //@{
    MOS_AgentModel( bool bAutomataModel, const std::string& strName );
    ~MOS_AgentModel();
    //@}

    //! @name Operations
    //@{
    void Initialize( MT_InputArchive_ABC& input );
    bool CanExecuteMission( E_MissionID nId ) const;
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    const T_Mission_Vector& GetAvailableMissions() const;
    //@}


private:
    //! @name Copy/Assignement
    //@{
    MOS_AgentModel( const MOS_AgentModel& );            //!< Copy constructor
    MOS_AgentModel& operator=( const MOS_AgentModel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void InitializeMission( MT_InputArchive_ABC& input );
    //@}

private:
    //! @name Member data
    //@{
    std::string strName_;
    bool bAutomataModel_;
    T_Mission_Vector vAvailableMissions_;
    //@}
};

#ifdef MOS_USE_INLINE
#   include "MOS_AgentModel.inl"
#endif

#endif // __MOS_AgentModel_h_
