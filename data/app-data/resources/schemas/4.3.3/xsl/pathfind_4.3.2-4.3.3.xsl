<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>

    <xsl:template match="/pathfind/unit-rules">
		<unit-rules>
			<xsl:apply-templates select="node()|@*"/>
			<rule autonomous="false" flying="true" inherits="base" type="criminal"/>
			<rule autonomous="false" flying="false" inherits="base" type="criminal"/>
			<rule autonomous="true" flying="false" inherits="base" type="criminal"/>
			<rule autonomous="true" flying="true" inherits="base" type="criminal"/>
		</unit-rules>
    </xsl:template>
</xsl:stylesheet>
    