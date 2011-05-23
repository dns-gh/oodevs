<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>

    <xsl:template match="@dia-type[.='net.masagroup.sword.military.platoon.combat.support.artillery.roles.Artillery']">
        <xsl:attribute name="dia-type">net.masagroup.sword.france.military.platoon.combat.support.artillery.roles.Artillery</xsl:attribute>
    </xsl:template>
</xsl:stylesheet>

net.masagroup.sword
