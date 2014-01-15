<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">

	<xsl:template match="@id">
		<xsl:choose>
            <xsl:when test=".='automat_log_supply_change_quotas' or .='formation_log_supply_change_quotas'"><xsl:attribute name="id">log_supply_change_quotas</xsl:attribute></xsl:when>
            <xsl:when test=".='automat_log_supply_pull_flow' or .='formation_log_supply_pull_flow'"><xsl:attribute name="id">log_supply_pull_flow</xsl:attribute></xsl:when>
            <xsl:when test=".='automat_log_supply_push_flow' or .='formation_log_supply_push_flow'"><xsl:attribute name="id">log_supply_push_flow</xsl:attribute></xsl:when>
            <xsl:otherwise><xsl:copy></xsl:copy></xsl:otherwise>
		</xsl:choose>
    </xsl:template>
    
 	<xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>

</xsl:stylesheet>
