<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template> 

  <xsl:template match="reports">
      <xsl:copy>
          <xsl:apply-templates select="node()|@*"/>
          <report category="operational" id="800" key="eRC_CrossedLima" message="%1 crossed by unit : %2">
                  <parameter type="LimaFunction"/>
                  <parameter type="String"/>
          </report>
      </xsl:copy>
  </xsl:template>

</xsl:stylesheet>
