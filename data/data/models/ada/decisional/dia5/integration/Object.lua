integration.createResourceLinkWithObject = function( object, resourceNode )
    return DEC_ResourceNetwork_CreateLinkFromObject( object.source, resourceNode.source )
end
integration.requestForDecontamination = function( decontaminationPlot )
    DEC_ConnaissanceObjet_DemandeDeDecontamination( decontaminationPlot.source )
end