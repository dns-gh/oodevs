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
        std::string terrain_;
        std::string model_;
        std::string physical_;
        bool copyScript_;
        bool copyScores_;
        bool copyDrawing_;
        bool copyMetaData_;
        bool copySuccessFactors_;
    };

    void CreateExercise( const tools::GeneralConfig& config, const std::string& name,
                         const std::string& terrain, const std::string& model, const std::string& physical = "" );
    void EditExerciseParameters( const tools::GeneralConfig& config, const std::string& name,
                         const std::string& terrain, const std::string& model, const std::string& physical = "" );
    void CreateExerciseAsCopyOf( const tools::GeneralConfig& config, const ExerciseCopyParameters& parameters );
}

#endif // __CreateExercise_h_
