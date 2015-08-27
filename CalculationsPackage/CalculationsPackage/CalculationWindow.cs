using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CalculationsPackage
{
    public partial class CalculationWindow : Form
    { 
        public CalculationWindow()
        {
            InitializeComponent();
        }

        private void RadioButton_CheckChanged(object sender, EventArgs e)
        {
            RadioButton rButton = sender as RadioButton;
            if (rButton.Checked)
            {
                switch (rButton.Tag.ToString())
                {
                    case "LV Mode":
                        MainForm.calculationMode = CalculationModes.LV;
                        break;

                    case "LA_AO Mode":
                        MainForm.calculationMode = CalculationModes.LA_AO;
                        break;

                    case "2D LV Volume & Mass Measurement":
                        MainForm.calculationMode = CalculationModes.TwoDim_LV_Volume;
                        break;

                    case "Aortic Flow":
                        MainForm.calculationMode = CalculationModes.Aortic_Blood_Flow;
                        break;

                    case "Pulmonary Flow":
                        MainForm.calculationMode = CalculationModes.Pulmonary_Blood_Flow;
                        break;

                    case "SVC Flow":
                        MainForm.calculationMode = CalculationModes.SVC_Blood_Flow;
                        break;

                    case "PDA":
                        MainForm.calculationMode = CalculationModes.PDA;
                        break;

                    case "RV Pressure":
                        MainForm.calculationMode = CalculationModes.RVPressure;
                        break;

                    default: break;
                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            MainForm.weight = (double)this.nudWeight.Value;
            this.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.tbName.Text = this.tbID.Text = string.Empty;
            this.nudWeight.Value = this.nudAge.Value = 0;
        }
    }
}
