// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FilterDialogs_h_
#define __FilterDialogs_h_

#include <tools/Resolver.h>

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

class FilterDialog;
class Model;

// =============================================================================
/** @class  FilterDialogs
    @brief  FilterDialogs
*/
// Created: ABR 2011-06-21
// =============================================================================
class FilterDialogs : public QObject
                    , public tools::StringResolver< FilterDialog >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             FilterDialogs( QWidget* parent, const tools::ExerciseConfig& config, Model& model, const kernel::CoordinateConverter_ABC& converter );
    virtual ~FilterDialogs();
    //@}

    //! @name Operations
    //@{
    void Load();
    void Purge();
    //@}

signals:
    //! @name Signals
    //@{
    void AddFilterMenuEntry( const QString name, const QObject* receiver, const char* eMember, QKeySequence keys );
    void RemoveFilterMenuEntry( const QString name );
    //@}

private:
    //! @name Helpers
    //@{
    void Load( xml::xistream& xis );
    void ReadSection( xml::xistream& xis );
    //@}

private:
    //! @name Data member
    //@{
    const tools::ExerciseConfig&           config_;
    Model&                                 model_;
    const kernel::CoordinateConverter_ABC& converter_;
    QWidget*                               parent_;
    //@}
};

#endif // __FilterDialogs_h_
