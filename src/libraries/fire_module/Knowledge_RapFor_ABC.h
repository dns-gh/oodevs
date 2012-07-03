// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_Knowledge_RapForLocal_ABC_h
#define fire_module_Knowledge_RapForLocal_ABC_h

#include "boost/noncopyable.hpp"

namespace xml
{
    class xistream;
    class xisubstream;
}

namespace sword
{
namespace wrapper
{
    class View;
}
namespace fire
{
// =============================================================================
/** @class  Knowledge_RapFor_ABC
    @brief  Knowledge RapFor
*/
// Created: NLD 2004-04-07
// =============================================================================
class Knowledge_RapFor_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Knowledge_RapFor_ABC();
    virtual ~Knowledge_RapFor_ABC();
    //@}

    //! @name Accessors
    //@{
    double GetValue( const wrapper::View& model, const wrapper::View& entity );
    //@}

    //! @name Tools
    //@{
    static void Initialize( xml::xisubstream xis, double tickDuration );
    //@}

protected:
    //! @name Tools
    //@{
    void ApplyValue( double rTotalFightScoreFriend, double rTotalFightScoreEnemy, double rFeedbackTime );
    bool NeedUpdate() const;
    //@}

protected:
    //! @name Member data
    //@{
    unsigned int nLastCacheUpdateTick_;
    //@}

private:
    //! @name Operations
    //@{
    virtual void Update( const wrapper::View& model, const wrapper::View& entity ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    double rRapForValue_;
    static double rRapForIncreasePerTimeStepDefaultValue_;
    //@}
};

}
}

#endif // fire_module_Knowledge_RapForLocal_ABC_h
