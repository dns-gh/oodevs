using System;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace Sword
{
    namespace Crossbow
    {
        public class ParameterDirection : OrderParameter
        {
            sealed class ParameterTypeDirection : ParameterType_ABC
            {
                private int m_direction;

                public override void OnContextMenu(int x, int y, IFeature selected)
                {
                    // NOTHING ?
                }
                
                public override void OnSelect(string value)
                {
                    m_direction = int.Parse(value);
                }

                public override string Type
                {
                    get
                    {
                        return "Danger";
                    }
                }

                public override string Value
                {
                    get
                    {
                        return m_direction.ToString();
                    }
                }
            }

            public ParameterDirection()
                : base("Danger direction", new ParameterTypeDirection())
            {
                // NOTHING
            }
        }
    }
}
