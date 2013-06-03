// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __FilterArgument_h_
#define __FilterArgument_h_

namespace tools
{
    class ExerciseConfig;
}

class FilterInputArgument;

// =============================================================================
/** @class  FilterArgument
    @brief  FilterArgument
*/
// Created: ABR 2011-06-17
// =============================================================================
class FilterArgument : public QObject
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
     FilterArgument( xml::xistream& xis, const tools::ExerciseConfig& config, const std::string& value );
    ~FilterArgument();
    //@}

    bool IsValid() const;
    void Update();
    void AddWidget( QWidget* widget, QGridLayout* grid, int row );
    bool IsInputArgument() const;
    std::string ToCommandLine() const;

signals:
    void ValueChanged( const QString& text );

public slots:
    void OnValueChanged( const QString& );

private:
    std::string name_;
    std::string displayName_;
    std::string value_;
    std::auto_ptr< FilterInputArgument > input_;
};

#endif // __FilterCommand_h_
