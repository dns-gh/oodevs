<?xml version="1.0" encoding="UTF-8"?><xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
   
    <xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>
   
    <xsl:template match="unit-rules">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
			<rule autonomous="false" flying="false" inherits="base" type="criminal">
		  <preferred-altitude value="-0.001"/>
		  <preferred-terrains strength="50">
			<preferred-terrain type="forest"/>
			<preferred-terrain type="urban area"/>
		  </preferred-terrains>
		  <avoided-terrains strength="50">
			<avoided-terrain type="country road"/>
			<avoided-terrain type="secondary road"/>
			<avoided-terrain type="main road"/>
			<avoided-terrain type="highway"/>
		  </avoided-terrains>
		  <object-costs avoid="true" threshold="10000">
			<object-cost type="checkpoint" value="10000"/>
		  </object-costs>
		  <population-costs maximum="100" outside-of-population="100" security-range="50">
			<population-cost attitude="calme" contact-cost="0"/>
			<population-cost attitude="agitee" contact-cost="0"/>
			<population-cost attitude="excitee" contact-cost="0"/>
			<population-cost attitude="agressive" contact-cost="0"/>
		  </population-costs>
		</rule>
		<rule autonomous="false" flying="true" inherits="base" type="criminal"/>
		<rule autonomous="true" flying="false" inherits="base" type="criminal"/>
		<rule autonomous="true" flying="true" inherits="base" type="criminal"/>

        </xsl:copy>
    </xsl:template>

</xsl:stylesheet>
