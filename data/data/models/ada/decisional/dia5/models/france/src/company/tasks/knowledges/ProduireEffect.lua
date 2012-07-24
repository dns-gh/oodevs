return
{
    getObjective = function( self, params )
        myself.leadData.currentPositionToProduce = myself.leadData.currentPositionToProduce or 0
        myself.leadData.currentPositionToProduce = myself.leadData.currentPositionToProduce % #params.objectives + 1
        return params.objectives[myself.leadData.currentPositionToProduce]
    end,
}
