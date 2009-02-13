using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.esriSystem;

namespace Sword
{
    namespace Crossbow
    {
        public interface IMissionObserver
        {
            void Update(ParameterLimits param, string limit1, string limit2);
            void Update(ParameterLimas param, string key, string value);
            void Update(OrderParameter param);
        }

        public partial class MissionOrderForm : Form, IMissionObserver
        {
            private IOrder m_order;

            public MissionOrderForm(IOrder order)
            {
                if (order == null)
                    return;
                InitializeComponent();
                m_order = order;
                OrderName = m_order.Name;
                UnitName = m_order.Target + " - [" + m_order.Id + "]";
                Show();
            }

            #region IMissionForm implementation
            public string OrderName
            {
                get
                {
                    return base.Text;
                }

                set
                {
                    base.Text = value;
                }
            }
            public string UnitName
            {
                get
                {
                    return m_UnitName.Text;
                }

                set
                {
                    m_UnitName.Text = value;
                }
            }
            #endregion

            #region IMissionObserver Members
            public void Update(ParameterLimits param, string limit1, string limit2)
            {
                m_Limit1.Text = limit1;
                m_Limit2.Text = limit2;
            }

            public void Update(ParameterLimas param, string key, string value)
            {
                System.Windows.Forms.TreeNode[] node = m_ParameterTree.Nodes.Find(key, false);
                if (node.Length == 0)
                    m_ParameterTree.Nodes.Add(key, value);
            }

            public void Update(OrderParameter param)
            {
                if (param == null)
                    return;
                TreeNode[] nodes = m_ParameterTree.Nodes.Find(param.Name, false);
                if (nodes.Length == 0)
                {
                    TreeNode node = m_ParameterTree.Nodes.Add(param.Name, param.Name);
                    node.Checked = param.IsSet(param.Name);
                    node = node.Nodes.Add(param.Value);
                }
                else
                {
                    nodes[0].Checked = true;
                    nodes[0].Nodes[0].Text = param.Value;
                }
            }
            #endregion

        }
    }
}