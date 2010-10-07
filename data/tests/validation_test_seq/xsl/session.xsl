<xsl:stylesheet version = '1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform' exclude-result-prefixes="url">
    <xsl:output method="xml" indent="yes"/>
    <xsl:strip-space elements="*"/>
    <xsl:param name="testname"/>
    <xsl:template match="test">
        <xsl:if test="$testname=@name">
            <session>
                <config>
                    <dispatcher>
                        <network client="localhost:10000" server="10001"/>
                        <plugins>
                            <recorder/>
                        </plugins>
                    </dispatcher>
                    <gaming>
                        <network server="localhost:10001"/>
                    </gaming>
                    <simulation>
                        <checkpoint frequency="100000h" keep="1" usecrc="true"/>
                        <debug decisional="true" diadebugger="false" diadebuggerport="15000" networklogger="true" networkloggerport="20000" pathfind="false"/>
                        <decisional useonlybinaries="false"/>
                        <dispatcher embedded="true"/>
                        <hla enabled="false" federate="Sword OT Power" federation="MyFederation"/>
                        <network port="10000"/>
                        <orbat checkcomposition="false"/>
                        <pathfinder threads="1"/>
                        <profiling enabled="false"/>
                        <time factor="10" step="10">
                            <xsl:attribute name="end-tick">
                                <xsl:value-of select="@ticks"/>
                            </xsl:attribute>
                        </time>
                    </simulation>
                </config>
                <meta>
                    <comment/>
                    <date>20100901T120000</date>
                    <name/>
                </meta>
            </session>
        </xsl:if>
    </xsl:template>
</xsl:stylesheet>