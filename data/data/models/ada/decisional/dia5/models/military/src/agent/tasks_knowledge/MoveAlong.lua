local result =  
{
    fillParameters = function( self, companyTask, params, entity )
      if params.pcObjective and params.pcObjective ~= NIL then
          return { objectives = { params.pcObjective } }
      else
          local objectives = companyTask:getMeetingPoint( params )
          if next( objectives ) then
              return { objectives = objectives }
          end
          return nil
      end
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks_knowledge.MoveAlong"] = t

return result
