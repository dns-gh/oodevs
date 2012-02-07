local result = 
{
    fillParameters = function( self, companyTask, params )
        -- $$$$ MIA TODO if task the task exist at company level.
        --  return { meetingPoint =  , positions =  }
        return nil --use by keepBest in Lead skill
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.individual.tasks.knowledges.Ambush"] = t

return result
