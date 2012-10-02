// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CreateExercise_h_
#define __CreateExercise_h_

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{
    struct ExerciseCopyParameters
    {
        ExerciseCopyParameters() {}
        virtual ~ExerciseCopyParameters() {}

        std::string from_;
        std::string to_;
        std::string checkpoint_;
        std::string terrain_;
        std::string model_;
        std::string physical_;
        const QStandardItemModel* itemModel_;
    };

    void Copy( const std::string& from, const std::string& to );

    void CreateExercise( const tools::GeneralConfig& config, const std::string& name,
                         const std::string& terrain, const std::string& model, const std::string& physical = "" );
    void EditExerciseParameters( const tools::GeneralConfig& config, const std::string& name,
                         const std::string& terrain, const std::string& model, const std::string& physical = "" );
    void CreateExerciseAsCopyOf( const tools::GeneralConfig& config, const ExerciseCopyParameters& parameters );
    void CreateExerciseAsCopyOfCheckpoint( const tools::GeneralConfig& config, const ExerciseCopyParameters& params );
}

#endif // __CreateExercise_h_
