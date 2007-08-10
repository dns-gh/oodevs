using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.esriSystem;

namespace crossbow
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
            InitializeComponent();
            m_order = order;
            OrderName = m_order.Name;
            UnitName = m_order.Target + " - [" + m_order.OID + "]";
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
            System.Windows.Forms.TreeNode[] node = m_ParameterTree.Nodes.Find(param.Name, false);
            if (node.Length == 0)
                m_ParameterTree.Nodes.Add(param.Name, param.Value);
        }
        #endregion
    }
}